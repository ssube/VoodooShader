
#include "Adapter.hpp"

namespace VoodooShader
{
	bool Adapter::PreparePass(PassRef pass)
	{
		Throw("Voodoo Core: Adapter function called.", NULL);
		return false;
	}

	void Adapter::BindPass(PassRef pass)
	{
		Throw("Voodoo Core: Adapter function called.", NULL);
	}

	void Adapter::UnbindPass()
	{
		Throw("Voodoo Core: Adapter function called.", NULL);
	}

	void Adapter::DrawQuad(bool fullscreen, float * coords)
	{
		Throw("Voodoo Core: Adapter function called.", NULL);
	}

	void Adapter::ApplyParameter(ParameterRef param)
	{
		Throw("Voodoo Core: Adapter function called.", NULL);
	}

	bool Adapter::ConnectTexture(ParameterRef param, TextureRef texture)
	{
		Throw("Voodoo Core: Adapter function called.", NULL);
		return false;
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
