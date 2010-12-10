
#include "Shader.hpp"
#include "Core.hpp"
#include "Exception.hpp"
#include "Adapter.hpp"
#include "Converter.hpp"

namespace VoodooShader
{
	Shader::Shader(Core * parent, std::string filename, const char ** args)
		: mCore(parent), mName(filename), mDefaultTechnique()
	{
		this->mEffect = cgCreateEffectFromFile(parent->GetCGContext(), filename.c_str(), args);

		if ( !cgIsEffect(this->mEffect) )
		{
			Throw("Voodoo Core: Failed to create shader.", parent);
			return;
		}

		cgSetEffectName(this->mEffect, this->mName.c_str());
	}
		
	Shader::Shader(Core * parent, CGeffect effect)
		: mCore(parent), mEffect(effect)
	{
		this->mName = cgGetEffectName(effect);
	}

	TechniqueRef Shader::GetDefaultTechnique()
	{
		return this->mDefaultTechnique;
	}

	void Shader::Link()
	{
		this->SetupParameters();
		this->SetupTechniques();
	}

	void Shader::SetupParameters()
	{
		// Setup and link a shader to the core.
		CGparameter cParam = cgGetFirstEffectParameter(this->mEffect);

		while ( cgIsParameter(cParam) )
		{
			CGtype cgType = cgGetParameterType(cParam);
			ParameterType type = Converter::ToParameterType(cgType);

			const char * paramName = cgGetParameterName(cParam);

			ParameterRef param(new Parameter(this, paramName, type));

			ParameterRef globalParam = this->mCore->GetParameter(paramName, type);
			if ( globalParam )
			{
				globalParam->Attach(param);
			}

			this->mParameters[paramName] = param;

			// Branch on parameter general type
			if (  Converter::ToParameterCategory(type) == PC_Sampler )
			{
				// Link to a texture
				CGannotation texnameAnnotation = cgGetNamedParameterAnnotation(cParam, "texture");
				const char * texName = cgGetStringAnnotationValue(texnameAnnotation);
				if ( texName )
				{
					TextureRef texture = mCore->GetTexture(texName);
					if ( texture.get() )
					{
						mCore->GetAdapter()->ConnectTexture(param, texture);
					} else {
						// Need to create a texture

					}
				}
			}

			cParam = cgGetNextParameter(cParam);
		}
	}

	void Shader::SetupTechniques()
	{
		CGtechnique cTech = cgGetFirstTechnique(mEffect);
		while ( cgIsTechnique(cTech) )
		{
			CGbool valid = cgValidateTechnique(cTech);

			if ( valid == CG_TRUE )
			{
				mCore->GetLog()->Format("Voodoo Core: Validated technique %s.\n")
					.With(cgGetTechniqueName(cTech)).Done();

				// Insert the technique into the map
				TechniqueRef tech(new Technique(this, cTech));

				//! @todo Possibly keep a map of valid techniques and allow changing at runtime

				// The first valid technique is the default one
				if ( !mDefaultTechnique.get() )
				{
					this->mDefaultTechnique = tech;
				}

				tech->Link();
			} else {
				mCore->GetLog()->Format("Voodoo Core: Technique failed to validate: %s.\n")
					.With(cgGetTechniqueName(cTech)).Done();
			}

			cTech = cgGetNextTechnique(cTech);
		}
	}

	Technique::Technique(Shader * parent, CGtechnique cgTech)
		: mParent(parent), mTechnique(cgTech)
	{
		this->mCore = this->mParent->GetCore();

		const char * techName = cgGetTechniqueName(this->mTechnique);
		if ( techName )
		{
			this->mName = techName;
		} else {
			char nameBuffer[16];
			itoa((int)(&this->mTechnique), nameBuffer, 16);

			this->mName = "tech_";
			this->mName += nameBuffer;
		}
	}

	PassRef Technique::GetPass(size_t index)
	{
		if ( index < this->mPasses.size() )
		{
			return this->mPasses[index];
		} else {
			//! @todo Fix NULL in Throw
			Throw("Voodoo Core: Invalid pass index (> pass count).", NULL);
		}
	}

	size_t Technique::NumPasses()
	{
		return this->mPasses.size();
	}

	void Technique::Link()
	{
		this->mPasses.clear();

		CGpass cPass = cgGetFirstPass(mTechnique);

		while ( cgIsPass(cPass) )
		{
			// Insert the pass into the vector
			PassRef pass(new Pass(this, cPass));

			mPasses.push_back(pass);

			pass->Link();

			cPass = cgGetNextPass(cPass);
		}
	}

	Pass::Pass(Technique * parent, CGpass cgPass)
		: mParent(parent), mPass(cgPass)
	{
		this->mCore = this->mParent->GetCore();

		const char * passName = cgGetPassName(this->mPass);
		if ( passName )
		{
			this->mName = passName;
		} else {
			char nameBuffer[16];
			itoa((int)(&this->mPass), nameBuffer, 16);

			this->mName = "pass_";
			this->mName += nameBuffer;
		}
	}

	void Pass::Link()
	{
		this->mVertexProgram = cgGetPassProgram(this->mPass, CG_VERTEX_DOMAIN);
		this->mFragmentProgram = cgGetPassProgram(this->mPass, CG_FRAGMENT_DOMAIN);
		this->mGeometryProgram = cgGetPassProgram(this->mPass, CG_GEOMETRY_DOMAIN);

		this->mTarget = TextureRef();
		CGannotation targetAnnotation = cgGetNamedPassAnnotation(this->mPass, "target");
		if ( cgIsAnnotation(targetAnnotation) )
		{
			if ( cgGetAnnotationType(targetAnnotation) == CG_STRING )
			{
				const char * targetName = cgGetStringAnnotationValue(targetAnnotation);

				this->mTarget = mCore->GetTexture(targetName);

				if ( !this->mTarget.get() )
				{
					mCore->GetLog()->Format("Voodoo Core: Pass %s cannot find target %s.")
						.With(this->Name()).With(targetName).Done();
				}
			} else {
				//! @todo Fix this nested mess.
				mCore->GetLog()->Format("Voodoo Core: Pass %s has annotation \"target\" of invalid type.")
					.With(this->Name()).Done();
			}
		}

		// Load the programs
		Adapter * adapter = mCore->GetAdapter();

		if ( !adapter->LoadPass(this) )
		{
			mCore->GetLog()->Format("Voodoo Core: Failed to load pass %s.\n")
				.With(this->Name()).Done();
		}
	}

	CGprogram Pass::GeometryProgram()
	{
		return this->mGeometryProgram;
	}

	CGprogram Pass::FragmentProgram()
	{
		return this->mFragmentProgram;
	}

	CGprogram Pass::VertexProgram()
	{
		return this->mVertexProgram;
	}
}
