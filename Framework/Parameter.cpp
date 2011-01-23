
#include "Parameter.hpp"

#include "Exception.hpp"
#include "Core.hpp"
#include "Converter.hpp"

namespace VoodooShader
{
	Parameter::Parameter(Core * core, String name, ParameterType type)
		: mType(type), mParent(NULL), mVirtual(true), mCore(core)
	{ 
		mParam = cgCreateParameter(core->GetCGContext(), Converter::ToCGType(type));

		switch ( this->mType )
		{
		case PT_Sampler1D:
		case PT_Sampler2D:
		case PT_Sampler3D:
			this->mValueTexture = TextureRef();
			break;
		case PT_Matrix:
		case PT_Float4:
		case PT_Float3:
		case PT_Float2:
		case PT_Float1:
			memset(this->mValueFloat, 0, sizeof(float)*16);
			break;
		case PT_Unknown:
		default:
			Throw("Invalid parameter type.", NULL);
			break;
		}
	}

	Parameter::Parameter(Shader * parent, CGparameter param)
		: mParent(parent), mParam(param), mVirtual(false), mCore(parent->GetCore())
	{
		mType = Converter::ToParameterType(cgGetParameterType(param));

		switch ( this->mType )
		{
		case PT_Sampler1D:
		case PT_Sampler2D:
		case PT_Sampler3D:
			this->mValueTexture = TextureRef();
			break;
		case PT_Matrix:
		case PT_Float4:
		case PT_Float3:
		case PT_Float2:
		case PT_Float1:
			memset(this->mValueFloat, 0, sizeof(float)*16);
			break;
		case PT_Unknown:
		default:
			Throw("Invalid parameter type.", NULL);
			break;
		}
	}

	std::string Parameter::Name()
	{
		std::string name;
		if ( mParent )
		{
			name += this->mParent->Name();
		}

		name += ":";

		if ( cgIsParameter(mParam) )
		{
			name += cgGetParameterName(this->mParam);
		} else {
			name += "[invalid_param]";
		}

		return name;
	}

	CGparameter Parameter::GetParameter(void)
	{
		return this->mParam;
	}

	void Parameter::Attach(ParameterRef param)
	{
		if ( !this->mVirtual )
		{
			Throw("Voodoo Core: Cannot attach to a non-virtual parameter.", mCore);
		}
		cgConnectParameter(param->GetParameter(), this->mParam);
	}

	ParameterType Parameter::GetType(void)
	{
		return this->mType;
	}

	void Parameter::Set(TextureRef newTex)
	{
		this->mValueTexture = newTex;
	}

	void Parameter::Set(float newX)
	{
		this->mValueFloat[0] = newX;
	}

	void Parameter::Set(float newX, float newY)
	{
		this->mValueFloat[0] = newX;
		this->mValueFloat[1] = newY;
	}

	void Parameter::Set(float newX, float newY, float newZ)
	{
		this->mValueFloat[0] = newX;
		this->mValueFloat[1] = newY;
		this->mValueFloat[2] = newZ;
	}

	void Parameter::Set(float newX, float newY, float newZ, float newW)
	{
		this->mValueFloat[0] = newX;
		this->mValueFloat[1] = newY;
		this->mValueFloat[2] = newZ;
		this->mValueFloat[3] = newW;
	}

	void Parameter::Get(TextureRef & param)
	{
		param = this->mValueTexture;
	}

	void Parameter::Get(float & paramX)
	{
		paramX = this->mValueFloat[0];
	}

	void Parameter::Get(float & paramX, float & paramY)
	{
		paramX = this->mValueFloat[0];
		paramY = this->mValueFloat[1];
	}

	void Parameter::Get(float & paramX, float & paramY, float & paramZ)
	{
		paramX = this->mValueFloat[0];
		paramY = this->mValueFloat[1];
		paramZ = this->mValueFloat[2];
	}

	void Parameter::Get
	(
		float & paramX, float & paramY, float & paramZ, float & paramW
	)
	{
		paramX = this->mValueFloat[0];
		paramY = this->mValueFloat[1];
		paramZ = this->mValueFloat[2];
		paramW = this->mValueFloat[3];
	}

	void Parameter::ForceUpdate()
	{
		if ( mCore )
		{
			mCore->Log("Voodoo Core: Force updating parameter %s.\n", this->Name().c_str());
		}

		switch ( mType )
		{
		case PT_Float1:
			cgSetParameter1fv(mParam, mValueFloat);
			break;
		case PT_Float2:
			cgSetParameter2fv(mParam, mValueFloat);
			break;
		case PT_Float3:
			cgSetParameter3fv(mParam, mValueFloat);
			break;
		case PT_Float4:
			cgSetParameter4fv(mParam, mValueFloat);
			break;
		case PT_Matrix:
			cgSetMatrixParameterfc(mParam, mValueFloat);
			break;
		case PT_Sampler1D:
			if ( mCore )
			{
				mCore->Log("Voodoo Core: Unable to force update sampler type parameter (%s).\n", this->Name().c_str());
			}
			break;
		case PT_Unknown:
		default:
			if ( mCore )
			{
				mCore->Log("Voodoo Core: Cannot force update parameter %s with unknown type.\n", this->Name().c_str());
			}
			break;
		}
	}
}
