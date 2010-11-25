
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
		Technique(Core * core, CGtechnique technique);

		inline std::string Name()
		{
			return (mParent->Name() + "::" + mName);
		};

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
		Pass(Core * core, CGpass pass);


		inline std::string Name()
		{
			return (mParent->Name() + "::" + mName);
		};

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