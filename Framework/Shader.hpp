
#ifndef VOODOO_SHADER_HPP
#define VOODOO_SHADER_HPP

#include "Meta.hpp"

namespace VoodooShader
{
	class VOODOO_API Shader
	{
	public:
		Shader(Core * parent, std::string filename, const char ** args = NULL);
		Shader(Core * parent, CGeffect effect);

		TechniqueRef GetDefaultTechnique();

		size_t NumParams();
		ParameterRef GetParam(size_t index);

		inline std::string Name()
		{
			return mName;
		};

		inline Core * GetCore()
		{
			return mCore;
		}

	protected:
		void LinkShader();

		void SetupTechniques();

	private:

		Core * mCore;
		std::string mName;
		CGeffect mEffect;

		TechniqueRef mDefaultTechnique;
		TechniqueMap mTechniques;
		ParameterVector mParameters;
	};

	class VOODOO_API Technique
	{
	public:
		Technique(ShaderRef shader, CGtechnique technique);

		inline std::string Name()
		{
			return (mParent->Name() + "::" + mName);
		};

		inline Core * GetCore()
		{
			return mCore;
		}

		size_t NumPasses();
		PassRef GetPass(size_t index);

	private:
		void SetupPasses();

		Core * mCore;
		ShaderRef mParent;
		std::string mName;
		PassVector mPasses;
		CGtechnique mTechnique;
	};

	class VOODOO_API Pass
	{
	public:
		Pass(TechniqueRef tech, CGpass pass);


		inline std::string Name()
		{
			return (mParent->Name() + "::" + mName);
		};

		inline Core * GetCore()
		{
			return mCore;
		}

		TextureRef Target();

		CGprogram GeometryProgram();
		CGprogram VertexProgram();
		CGprogram FragmentProgram();

	private:
		Core * mCore;
		TechniqueRef mParent;

		std::string mName;
		TextureRef mTarget;
		CGpass mPass;

		CGprogram mGeometryProgram;
		CGprogram mVertexProgram;
		CGprogram mFragmentProgram;
	};
}

#endif /*VOODOO_SHADER_HPP*/