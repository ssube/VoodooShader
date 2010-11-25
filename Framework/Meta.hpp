
#ifndef VOODOO_META_HPP
#define VOODOO_META_HPP

#ifndef VOODOO_IMPORT
#	define VOODOO_API __declspec(dllexport)
#else
#	define VOODOO_API __declspec(dllimport)
#endif

// Hide the DLL-interface warnings
#pragma warning(disable:4251)

#include "Includes.hpp"
#include "Reference.hpp"

namespace VoodooShader
{
	class Core;
	class Adapter;
	class Logger;

	class FullscreenManager;
	class MaterialManager;

	class Shader;
	class Texture;
	
	class Technique;
	class Pass;
	class Parameter;

	class Exception;

	class Converter;

#define Throw(x, y) throw Exception(x, y, __FILE__, __FUNCTION__, __LINE__);

	typedef std::string String;

	// Reference-counted pointer types
	typedef Reference<Shader>					ShaderRef;
	typedef Reference<Texture>					TextureRef;
	typedef Reference<Technique>				TechniqueRef;
	typedef Reference<Pass>						PassRef;
	typedef Reference<Parameter>				ParameterRef;

	// Shader collections
	typedef std::map<std::string, ShaderRef>	ShaderMap;
	typedef std::list<ShaderRef>				ShaderList;
	typedef std::vector<ShaderRef>				ShaderVector;

	// Technique collections
	typedef std::map<std::string, TechniqueRef>	TechniqueMap;
	typedef std::list<TechniqueRef>				TechniqueList;
	typedef std::vector<TechniqueRef>			TechniqueVector;

	// Pass collections
	typedef std::map<std::string, PassRef>		PassMap;
	typedef std::list<PassRef>					PassList;
	typedef std::vector<PassRef>				PassVector;

	// Parameter collections
	typedef std::map<std::string, ParameterRef>	ParameterMap;
	typedef std::list<ParameterRef>				ParameterList;
	typedef std::vector<ParameterRef>			ParameterVector;

	// Texture collections
	typedef std::map<std::string, TextureRef>	TextureMap;
	typedef std::list<TextureRef>				TextureList;
	typedef std::vector<TextureRef>				TextureVector;

	// Miscellaneous collections
	typedef std::map<TextureRef, ShaderRef>		MaterialMap;
	typedef std::map<std::string, CGeffect>		CGEffectMap;

	/**
	 * Texture formats for use by @ref Texture "Textures". These may not be 
	 * implemented by the underlying graphics API exactly as they are indicated 
	 * here, but the available components and sizes are guaranteed to be equal 
	 * to the indicated values. Further information on texture formats and depth
	 * may be found on the @ref texturesanddepth "texture formats page".
	 */
	enum TextureFormat
	{
		TF_Unknown = 0,
		// Backbuffer formats
		TF_RGB5,
		TF_RGBA1,
		TF_RGB8,
		TF_RGBA8,
		TF_RGB10A2,
		// Extended-depth texture formats
		TF_RGBA16F,
		TF_RGBA32F,
		// Depth-buffer formats
		TF_D16,
		TF_D32,
		// Max
		TF_Count
	};

	/**
	 * Parameter types for use by @ref Parameter "Parameters." These are generally
	 * handled within the Cg runtime and rarely change in hardware, the common ones
	 * (float4 and such) are identical across the board.
	 */
	enum ParameterType
	{
		PT_Unknown = 0,
		// Float-vectors
		PT_Float1,
		PT_Float2, 
		PT_Float3,
		PT_Float4,
		// Samplers
		PT_Sampler1D,
		PT_Sampler2D,
		PT_Sampler3D,
		// Max
		PT_Count
	};
}

#endif /*VOODOO_META_HPP*/