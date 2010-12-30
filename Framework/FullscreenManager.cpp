// Framework.cpp : Defines the exported functions for the DLL application.
//
// 

#include "FullscreenManager.hpp"
#include "Adapter.hpp"
#include "Exception.hpp"
#include "Core.hpp"

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
			Throw("Voodoo Core: Could not add shader, invalid position (< -1).",
				this->mParent);
		} else if ( position == -1 ) {
			// Append to the end of the list
			this->mShaders.push_back(shader);
			return this->mShaders.size() - 1;
		} else {
			ShaderVector::iterator shaderPosition = this->mShaders.begin() + position;
			this->mShaders.insert(shaderPosition, shader);
			return position;
		}
	}

	void FullscreenManager::Remove(size_t position)
	{
		if ( this->mShaders.size() <= position )
		{
			Throw(
				"Voodoo Core: Could not remove shader, list size <= position.", 
				this->mParent);
		} else {
			ShaderVector::iterator shaderPosition = this->mShaders.begin() + position;
			this->mShaders.erase(shaderPosition);
		}
	}

	void FullscreenManager::Render(unsigned int start, int count)
	{
		if ( count == 0 )
		{
			Throw("Voodoo Core: Could not render 0 fullscreen shaders.", this->mParent);
		}

		Adapter * adapter = this->mParent->GetAdapter();

		if ( !adapter )
		{
			Throw("Voodoo Core: Parent Core has no Adapter.", this->mParent);
		}

		size_t total = mShaders.size();

		if ( start > total )
		{
			Throw("Voodoo Core: Invalid start position (past end of chain).", this->mParent);
		}

		if ( count > total )
		{
			Throw("Voodoo Core: Attempted to render more shaders than in the chain.", this->mParent);
		}
		//ShaderVector::iterator chainStart, chainCurrent, chainEnd;

		//unsigned int chainStart = this->mShaders.begin() + start; 
		unsigned int chainEnd, chainCurrent;

		if ( count == -1 )
		{
			chainEnd = total;
		} else {
			chainEnd = start + count;

			if ( chainEnd > total )
			{
				Throw("Voodoo Core: With start offset, end shader is past the end of the chain.", this->mParent);
			}
		}

		for ( chainCurrent = start; chainCurrent < chainEnd; ++chainCurrent )
		{
			//ShaderRef shader = *current;
			adapter->DrawShader(mShaders[chainCurrent]);
		}
	}
}
