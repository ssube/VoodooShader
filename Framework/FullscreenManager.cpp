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
        } else if ( (size_t)position > mShaders.size() ) {
            Throw
            (
                VOODOO_CORE_NAME,
                "Could not add shader, position beyond end of list.",
                this->mParent
            );
        } else {
            //ShaderVector::iterator shaderPosition = this->mShaders[(size_t)position];
            //this->mShaders.insert(shaderPosition, shader);
            return (size_t)position; // Safe, because we've already handled all negative cases
        }
    }

    void FullscreenManager::Remove(size_t position)
    {
        /*if ( position < -1 )
        {
            Throw
            (
                VOODOO_CORE_NAME,
                "Could not remove shader, invalid index (< -1).",
                this->mParent
            );
        }
        else if ( this->mShaders.size() == -1 )
        {
            //this->mShaders.erase(this->mShaders());
        }* /
        else* /
        if ( this->mShaders.size() <= position )
        {
            Throw
            (
                VOODOO_CORE_NAME, 
                "Could not remove shader, list size <= position.", 
                this->mParent
            );
        } else {
            this->mShaders.erase(this->mShaders.at((size_t)position));
        }*/
    }

    void FullscreenManager::Remove(ShaderRef shader)
    {
        /*std::remove
        (
            mShaders.begin()
            
            )
        this->mShaders.reremove_if    
        (
            [&]
            (ShaderList::iterator item)
            {
                return ( (*item).get() == shader.get() );
            }
        );*/
    }

    void FullscreenManager::Render(size_t start, int count)
    {
        // Run a number of basic checks first, shouldn't have much performance
        // hit, but covers a good few errors.
        if ( count == 0 || count < -1 )
        {
            this->mParent->Log
            (
                LL_Warning,
                VOODOO_CORE_NAME, 
                "Could not render %i fullscreen shaders.",
                count
            );

            return;
        }

        Adapter * adapter = this->mParent->GetAdapter();

        if ( !adapter )
        {
            Throw(VOODOO_CORE_NAME, "Parent Core has no Adapter.", this->mParent);
        }

        size_t total = mShaders.size();

        if ( start > total )
        {
            this->mParent->Log
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
                this->mParent->Log
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
            mShaders[start], mShaders[end],
            [&]
            (ShaderList::iterator shader)
            {
                adapter->DrawShader(*shader);
            }
        );
    }
}
