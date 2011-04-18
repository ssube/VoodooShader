#include "Pass.hpp"

namespace VoodooShader
{
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
            _itoa_s((int)(&this->mPass), nameBuffer, 16, 16);

            this->mName = "pass_";
            this->mName += nameBuffer;
        }
    }

    Pass::~Pass()
    {
        mTarget = NULL;

        mCore->GetAdapter()->UnloadPass(this);
    }

    String Pass::GetName()
    {
        String name = mParent->GetName();
        name += "::";
        name += mName;
        return name;
    };

    Core * Pass::GetCore()
    {
        return mCore;
    }

    TextureRef Pass::GetTarget()
    {
        return mTarget;
    }

    CGprogram Pass::GetProgram(ProgramStage stage)
    {
        switch ( stage )
        {
        case PS_Vertex:
            return mVertexProgram;
        case PS_Fragment:
            return mFragmentProgram;
        case PS_Geometry:
            return mGeometryProgram;
        case PS_Domain:
        case PS_Hull:
        case PS_Unknown:
        default:
            return NULL;
        }
    }

    CGpass Pass::GetCgPass()
    {
        return mPass;
    }

    void Pass::Link()
    {
        this->mVertexProgram   = cgGetPassProgram(this->mPass, CG_VERTEX_DOMAIN  );
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
                    mCore->GetLogger()->Log
                        (
                        LL_Warning,
                        VOODOO_CORE_NAME,
                        "Pass %s cannot find target %s.", 
                        this->GetName().c_str(), targetName
                        );

                    this->mTarget = mCore->GetTexture(TT_PassTarget);
                }
            } else {
                mCore->GetLogger()->Log
                    (
                    LL_Warning,
                    VOODOO_CORE_NAME,
                    "Pass %s has annotation \"target\" of invalid type.",
                    this->GetName().c_str()
                    );

                this->mTarget = mCore->GetTexture(TT_PassTarget);
            }
        } else {
            mCore->GetLogger()->Log
                (
                LL_Debug,
                VOODOO_CORE_NAME,
                "Pass %s has no target annotation.", 
                this->GetName().c_str()
                );

            this->mTarget = mCore->GetTexture(TT_PassTarget);
        }

        // Load the programs
        IAdapterRef adapter = mCore->GetAdapter();

        if ( !adapter )
        {
            mCore->GetLogger()->Log
                (
                LL_Warning,
                VOODOO_CORE_NAME,
                "No adapter found, pass %s must be explicitly loaded later.", 
                this->GetName().c_str()
                );
        } else {
            if ( !adapter->LoadPass(this) )
            {
                mCore->GetLogger()->Log(LL_Error, VOODOO_CORE_NAME, "Failed to load pass %s.", this->GetName().c_str());
            } else {
                mCore->GetLogger()->Log(LL_Info, VOODOO_CORE_NAME, "Successfully loaded pass %s.", this->GetName().c_str());
            }
        }
    }
}