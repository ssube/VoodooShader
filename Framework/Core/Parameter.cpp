#include "Parameter.hpp"

#include "Core.hpp"
#include "Converter.hpp"
#include "Exception.hpp"
#include "Shader.hpp"

namespace VoodooShader
{
    Parameter::Parameter(Core * core, String name, ParameterType type)
        : mType(type), mParent(NULL), mVirtual(true), mCore(core)
    { 
        mParam = cgCreateParameter
        (
            core->GetCgContext(), 
            Converter::ToCGType(mType)
        );

        switch ( mType )
        {
        case PT_Float1:
        case PT_Float2:
        case PT_Float3:
        case PT_Float4:
        case PT_Matrix:
            memset(this->mValueFloat, 0, sizeof(float)*16);
            break;
        case PT_Sampler1D:
        case PT_Sampler2D:
        case PT_Sampler3D:
            this->mValueTexture = TextureRef();
            break;
        case PT_Unknown:
        default:
            //Throw(VOODOO_CORE_NAME, "Invalid parameter type.", mCore);
            break;
        }
    }

    Parameter::Parameter(Shader * parent, CGparameter param)
        : mParent(parent), mParam(param), mVirtual(false), mCore(parent->GetCore())
    {
        mType = Converter::ToParameterType(cgGetParameterType(param));

        switch ( this->mType )
        {
        case PT_Float1:
        case PT_Float2:
        case PT_Float3:
        case PT_Float4:
        case PT_Matrix:
            memset(this->mValueFloat, 0, sizeof(float)*16);
            break;
        case PT_Sampler1D:
        case PT_Sampler2D:
        case PT_Sampler3D:
            this->mValueTexture = TextureRef();
            break;
        case PT_Unknown:
        default:
            Throw(VOODOO_CORE_NAME, "Invalid parameter type.", mCore);
            break;
        }
    }

    String Parameter::GetName()
    {
        String name;
        if ( mParent )
        {
            name += this->mParent->GetName();
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

    CGparameter Parameter::GetCgParameter()
    {
        return this->mParam;
    }

    void Parameter::Attach(ParameterRef param)
    {
        if ( !this->mVirtual )
        {
            Throw(VOODOO_CORE_NAME, "Cannot attach to a non-virtual parameter.", mCore);
        }

        cgConnectParameter(param->GetCgParameter(), this->mParam);
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

    TextureRef Parameter::GetTexture()
    {
        return mValueTexture;
    };

    float * Parameter::GetFloat()
    {
        return mValueFloat;
    };

    void Parameter::ForceUpdate()
    {
        if ( mCore )
        {
            mCore->Log(LL_Debug, VOODOO_CORE_NAME, "Force updating parameter %s.", this->GetName().c_str());
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
        case PT_Sampler2D:
        case PT_Sampler3D:
            if ( mCore )
            {
                mCore->Log
                (
                    LL_Warning, 
                    VOODOO_CORE_NAME, 
                    "Unable to force update sampler type parameter (%s).", 
                    this->GetName().c_str()
                );
            }
            break;
        case PT_Unknown:
        default:
            if ( mCore )
            {
                mCore->Log
                (
                    LL_Warning,
                    VOODOO_CORE_NAME,
                    "Cannot force update parameter %s with unknown type.", 
                    this->GetName().c_str()
                );
            }
            break;
        }
    }
}
