
#include "Shader.hpp"
#include "Core.hpp"
#include "Exception.hpp"
#include "Adapter.hpp"
#include "Converter.hpp"

namespace VoodooShader
{
	Shader::Shader(Core * parent, std::string filename, const char ** args)
		: mCore(parent), mName(filename)
	{
		this->mEffect = cgCreateEffectFromFile(parent->GetCGContext(), filename.c_str(), args);

		if ( !cgIsEffect(this->mEffect) )
		{
			Throw("Voodoo Core: Could not create shader.", this->mCore);
			//return;
		}

		this->SetupTechniques();
	}
		
	Shader::Shader(Core * parent, CGeffect effect)
		: mCore(parent)
	{
		this->mEffect = effect;
		mName = cgGetEffectName(effect);

		this->SetupTechniques();
		this->LinkShader();
	}

	TechniqueRef Shader::GetDefaultTechnique()
	{
		return this->mDefaultTechnique;
	}

	void Shader::LinkShader()
	{
		// Setup and link a shader to the core.
		CGparameter cParam = cgGetFirstEffectParameter(this->mEffect);

		while ( cgIsParameter(cParam) )
		{
			CGtype cgType = cgGetParameterType(cParam);
			ParameterType type = Converter::ToParameterType(cgType);

			ParameterRef param(new Parameter(this, cgGetParameterName(cParam), type));
			this->mParameters.push_back(param);

			// Branch on parameter general type
			if ( ( type == PT_Sampler1D ) || ( type == PT_Sampler2D ) || ( type == PT_Sampler3D ) )
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
				TechniqueRef tech = new Technique(mCore, cTech);
				const char * nameC = cgGetTechniqueName(cTech);
				std::string name(nameC);
				mTechniques[name] = tech;

				// The first valid technique is the default one
				if ( !mDefaultTechnique.get() )
				{
					this->mDefaultTechnique = tech;
				}
			}

			cTech = cgGetNextTechnique(cTech);
		}
	}

	Technique::Technique(ShaderRef shader, CGtechnique technique)
		: mParent(shader), mTechnique(technique)
	{
		this->mCore = shader->GetCore();

		mName = cgGetTechniqueName(technique);
		this->SetupPasses();
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

	void Technique::SetupPasses()
	{
		this->mPasses.clear();

		CGpass cPass = cgGetFirstPass(mTechnique);
		while ( cgIsPass(cPass) )
		{
			// Insert the pass into the vector
			PassRef pass = new Pass(this, cPass);
			const char * name = cgGetPassName(cPass);
			mPasses.push_back(pass);

			cPass = cgGetNextPass(cPass);
		}
	}

	Pass::Pass(TechniqueRef tech, CGpass pass)
		: mParent(tech), mPass(pass)
	{
		this->mName = cgGetPassName(pass);
		this->mCore = tech->GetCore();

		this->mVertexProgram = cgGetPassProgram(pass, CG_VERTEX_DOMAIN);
		this->mFragmentProgram = cgGetPassProgram(pass, CG_FRAGMENT_DOMAIN);
		this->mGeometryProgram = cgGetPassProgram(pass, CG_GEOMETRY_DOMAIN);

		this->mTarget = NULL;
		CGannotation targetAnnotation = cgGetNamedPassAnnotation(pass, "target");
		if ( cgIsAnnotation(targetAnnotation) )
		{
			if ( cgGetAnnotationType(targetAnnotation) == CG_STRING )
			{
				const char * targetName = cgGetStringAnnotationValue(targetAnnotation);
				//! @todo Core::FindTexture needs added and implemented
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
		if ( !adapter->Prepare(this) )
		{
			Throw("Voodoo Core: Failed to prepare pass for use.", mCore);
		}
	}

	TextureRef Pass::Target()
	{
		return this->mTarget;
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