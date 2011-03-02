#include "Includes.hpp"
#include "FullscreenManager.hpp"
#include "Adapter.hpp"
#include "Exception.hpp"
#include "Core.hpp"
#include "Version.hpp"

namespace VoodooShader
{
    FullscreenManager::FullscreenManager(Core * parent)
        : mParent(parent) 
    {
        mShaders.clear();
    };

    size_t FullscreenManager::Add(ShaderRef shader, int position)
    {
        if ( position < -1 )
        {
            Throw
            (
                VOODOO_CORE_NAME, 
                "Could not add shader, invalid position (< -1).",
                this->mParent
            );
        } else if ( position == -1 ) {
            // Append to the end of the list
            this->mShaders.push_back(shader);
            return this->mShaders.size() - 1;
        } else {
            ShaderVector::iterator shaderPosition = this->mShaders.begin() + position;
            this->mShaders.insert(shaderPosition, shader);
            return (size_t)position;
        }
    }

    void FullscreenManager::Remove(size_t position)
    {
        if ( this->mShaders.size() <= position )
        {
            Throw
            (
                VOODOO_CORE_NAME, 
                "Could not remove shader, list size <= position.", 
                this->mParent
            );
        } else {
            ShaderVector::iterator shaderPosition = this->mShaders.begin() + position;
            this->mShaders.erase(shaderPosition);
        }
    }

    void FullscreenManager::Render(unsigned int start, int count)
    {
        // Run a number of basic checks first, shouldn't have much performance
        // hit, but covers a good few errors.

        if ( count == 0 )
        {
            Throw(VOODOO_CORE_NAME, "Could not render 0 fullscreen shaders.", this->mParent);
        }

        Adapter * adapter = this->mParent->GetAdapter();

        if ( !adapter )
        {
            Throw(VOODOO_CORE_NAME, "Parent Core has no Adapter.", this->mParent);
        }

        size_t total = mShaders.size();

        if ( start > total )
        {
            Throw(VOODOO_CORE_NAME, "Invalid start position (past end of chain).", this->mParent);
        }

        // Prep and trigger the rendering
        unsigned int chainEnd, chainCurrent;

        if ( count == -1 )
        {
            chainEnd = total;
        } else {
            chainEnd = start + count;

            if ( chainEnd > total )
            {
                this->mParent->Log
                (
                    LL_Warning, VOODOO_CORE_NAME, 
                    "Adjusted shader chain would have extended beyond available shaders (%d of %d), correcting.", 
                    chainEnd, total
                );
                chainEnd = total;
            }
        }

        for ( chainCurrent = start; chainCurrent < chainEnd; ++chainCurrent )
        {
            adapter->DrawShader(mShaders[chainCurrent]);
        }
    }
}
