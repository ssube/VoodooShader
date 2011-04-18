#include "Technique.hpp"

namespace VoodooShader
{
    Technique::Technique(Shader * parent, CGtechnique cgTech)
        : mParent(parent), mTechnique(cgTech)
    {
        this->mCore = this->mParent->GetCore();

        const char * techName = cgGetTechniqueName(this->mTechnique);
        if ( techName )
        {
            this->mName = techName;
        } else {
            char nameBuffer[16];
            sprintf_s(nameBuffer, "tech_%p", mTechnique);
            this->mName = nameBuffer;
        }
    }

    Technique::~Technique()
    {
        mTarget = NULL;
        mPasses.clear();
    }

    Core * Technique::GetCore()
    {
        return mCore;
    }

    String Technique::GetName()
    {
        String name = mParent->GetName();
        name += "::";
        name += mName;
        return name;
    };

    PassRef Technique::GetPass(size_t index)
    {
        if ( index < this->mPasses.size() )
        {
            return this->mPasses[index];
        } else {
            Throw(VOODOO_CORE_NAME, "Voodoo Core: Invalid pass index (> pass count).", mCore);
        }
    }

    TextureRef Technique::GetTarget()
    {
        return mTarget;
    }

    size_t Technique::GetPassCount()
    {
        return this->mPasses.size();
    }

    CGtechnique Technique::GetCgTechnique()
    {
        return mTechnique;
    }

    void Technique::Link()
    {
        this->mTarget = TextureRef();

        // Process the technique's target annotation
        CGannotation targetAnnotation = cgGetNamedTechniqueAnnotation(this->mTechnique, "target");

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

                    this->mTarget = mCore->GetTexture(TT_ShaderTarget);
                }
            } else {
                mCore->GetLogger()->Log
                    (
                    LL_Warning,
                    VOODOO_CORE_NAME,
                    "Pass %s has annotation \"target\" of invalid type.",
                    this->GetName().c_str()
                    );

                this->mTarget = mCore->GetTexture(TT_ShaderTarget);
            }
        } else {
            mCore->GetLogger()->Log
                (
                LL_Debug,
                VOODOO_CORE_NAME,
                "Pass %s has no target annotation.",
                this->GetName().c_str()
                );

            this->mTarget = mCore->GetTexture(TT_ShaderTarget);
        }


        this->mPasses.clear();

        CGpass cPass = cgGetFirstPass(mTechnique);

        while ( cgIsPass(cPass) )
        {
            // Insert the pass into the vector
            PassRef pass(new Pass(this, cPass));

            mPasses.push_back(pass);

            pass->Link();

            cPass = cgGetNextPass(cPass);
        }
    }
}
