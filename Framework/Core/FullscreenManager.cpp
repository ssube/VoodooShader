#include "FullscreenManager.hpp"

#include "Adapter.hpp"
#include "Core.hpp"
#include "Exception.hpp"
#include "Logger.hpp"
 
namespace VoodooShader
{
    FullscreenManager::FullscreenManager(Core * parent)
        : mParent(parent) 
    {
        mShaders.clear();
    };

    size_t FullscreenManager::Add(ShaderRef shader, int position)
    {
        if ( position < 0 )
        {
            if ( position < -1 )
            {
                Throw
                (
                    VOODOO_CORE_NAME, 
                    "Could not add shader, invalid position (< -1).",
                    this->mParent
                );
            } else {
                this->mShaders.push_back(shader);
                return this->mShaders.size() - 1;
            }
        } else {
            size_t upos = (size_t)position;
            size_t prevsize = this->mShaders.size();

            if ( upos > prevsize ) 
            {
                Throw
                (
                    VOODOO_CORE_NAME,
                    "Could not add shader, position beyond end of list.",
                    this->mParent
                );
            } else if ( upos == prevsize ) {
                this->mShaders.push_back(shader);
                return prevsize;
            } else {
                ShaderVector::iterator shaderPosition = this->mShaders.begin() + upos;
                this->mShaders.insert(shaderPosition, shader);
                return upos; 
           }
        }
    }

    void FullscreenManager::Remove(_In_ size_t position)
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
            this->mShaders.erase(this->mShaders.begin() + position);
        }
    }

    void FullscreenManager::Remove(_In_ ShaderRef shader)
    {
        std::remove_if
        (
            mShaders.begin(),
            mShaders.end(),
            [&]
            (ShaderRef item)
            {
                return ( item.get() == shader.get() );
            }
        );
    }

    void FullscreenManager::Render(size_t start, int count)
    {
        // Run a number of basic checks first, shouldn't have much performance
        // hit, but covers a good few errors.
        if ( count < 1 && count != -1 )
        {
            this->mParent->GetLogger()->Log
            (
                LL_Warning,
                VOODOO_CORE_NAME, 
                "Could not render %i fullscreen shaders.",
                count
            );

            return;
        }

        IAdapterRef adapter = this->mParent->GetAdapter();
        size_t total = mShaders.size();

        if ( start > total )
        {
            this->mParent->GetLogger()->Log
            (
                LL_Warning,
                VOODOO_CORE_NAME, 
                "Start position %u invalid, past end of chain.",
                start
            );
        }

        // Prep and trigger the rendering
        size_t end;

        if ( count == -1 )
        {
            end = total;
        } else {
            end = start + count;

            if ( end > total )
            {
                this->mParent->GetLogger()->Log
                (
                    LL_Warning, 
                    VOODOO_CORE_NAME, 
                    "Adjusted shader chain would have extended beyond available shaders (%d of %d), correcting.", 
                    end, total
                );
                end = total;
            }
        }

        std::for_each
        (
            mShaders.begin() + start, 
            mShaders.begin() + end,
            [&]
            (ShaderRef shader)
            {
                adapter->DrawShader(shader);
            }
        );
    }
}
