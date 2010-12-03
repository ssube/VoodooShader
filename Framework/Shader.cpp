
#include "Shader.hpp"
#include "Core.hpp"
#include "Exception.hpp"
#include "Adapter.hpp"
#include "Converter.hpp"

namespace VoodooShader
{
	Shader::Shader()
		: mCore(NULL), mEffect(NULL)
	{	}

	ShaderRef Shader::Create(Core * parent, std::string filename, const char ** args)
	{
		Shader * shader = new Shader();

		shader->mCore = parent;
		shader->mName = filename;
		shader->mEffect = cgCreateEffectFromFile(parent->GetCGContext(), filename.c_str(), args);

		if ( !cgIsEffect(shader->mEffect) )
		{
			Throw("Voodoo Core: Could not create shader.", parent);
			//return;
		}

		cgSetEffectName(shader->mEffect, shader->mName.c_str());

		return ShaderRef(shader);
	}
		
	ShaderRef Shader::Create(Core * parent, CGeffect effect)
	{
		Shader * shader = new Shader();

		shader->mCore = parent;
		shader->mName = cgGetEffectName(effect);
		shader->mEffect = effect;

		return ShaderRef(shader);
	}

	Technique * Shader::GetDefaultTechnique()
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
		this->mDefaultTechnique = NULL;
		this->mTechniques.clear();

		CGtechnique cTech = cgGetFirstTechnique(mEffect);
		while ( cgIsTechnique(cTech) )
		{
			CGbool valid = cgValidateTechnique(cTech);
			if ( valid == CG_TRUE )
			{
				// Insert the technique into the map
				Technique * tech = Technique::Create(this, cTech);

				mTechniques[tech->mName] = tech;

				// The first valid technique is the default one
				if ( !mDefaultTechnique )
				{
					this->mDefaultTechnique = tech;
				}

				tech->Link();
			}

			cTech = cgGetNextTechnique(cTech);
		}
	}

	Technique * Technique::Create(Shader * parent, CGtechnique cgTech)
	{
		Technique * tech = new Technique();

		tech->mTechnique = cgTech;
		tech->mParent = parent;
		tech->mCore = tech->mParent->GetCore();

		const char * techName = cgGetTechniqueName(tech->mTechnique);
		if ( techName )
		{
			tech->mName = techName;
		} else {
			char nameBuffer[16];
			itoa((int)(&tech->mTechnique), nameBuffer, 16);

			tech->mName = "tech_";
			tech->mName += nameBuffer;
		}

		return tech;
	}

	Technique::Technique()
		: mParent(NULL), mTechnique(NULL), mCore(NULL)
	{	}

	Pass * Technique::GetPass(size_t index)
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
			Pass * pass = Pass::Create(this, cPass);

			//! @todo This causes segfaults, fix
			mPasses.push_back(pass);

			pass->Link();

			cPass = cgGetNextPass(cPass);
		}
	}

	Pass * Pass::Create(Technique * parent, CGpass cgPass)
	{
		Pass * pass = new Pass();

		pass->mPass = cgPass;
		pass->mParent = parent;
		pass->mCore = pass->mParent->GetCore();

		const char * passName = cgGetPassName(pass->mPass);
		if ( passName )
		{
			pass->mName = passName;
		} else {
			char nameBuffer[16];
			itoa((int)(&pass->mPass), nameBuffer, 16);

			pass->mName = "pass_";
			pass->mName += nameBuffer;
		}

		return pass;
	}

	Pass::Pass()
		: mParent(NULL), mPass(NULL)
	{ }

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

		bool prepResults = adapter->LoadPass(this);

		if ( !prepResults )
		{
			Throw("Voodoo Core: Failed to prepare pass for use.", mCore);
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
