
#include "Adapter.hpp"

namespace VoodooShader
{
	bool Prepare(PassRef pass)
	{
		Throw("Voodoo Core: Adapter function called.", NULL);
		return false;
	}

	void Adapter::Bind(PassRef pass)
	{
		Throw("Voodoo Core: Adapter function called.", NULL);
	}

	void Adapter::Unbind()
	{
		Throw("Voodoo Core: Adapter function called.", NULL);
	}

	void Adapter::DrawFSQuad()
	{
		Throw("Voodoo Core: Adapter function called.", NULL);
	}

	void Adapter::Apply(ParameterRef param)
	{
		Throw("Voodoo Core: Adapter function called.", NULL);
	}

	TextureRef Adapter::CreateTexture(std::string name, size_t width, size_t height, size_t depth, 
		bool mipmaps, TextureFormat format)
	{
		Throw("Voodoo Core: Adapter function called.", NULL);
		return TextureRef();
	}

	void Adapter::HandleError(CGcontext context, CGerror error, void * core)
	{
		Core * actualCore = reinterpret_cast<Core*>(core);
		Core::CGErrorHandler(context, error, core);
	}
}