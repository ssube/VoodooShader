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
			Throw("Voodoo Core: Could not add shader, invalid position (< -1).", this->mParent);
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
			Throw("Voodoo Core: Could not remove shader, list size <= position.", this->mParent);
		} else {
			ShaderVector::iterator shaderPosition = this->mShaders.begin() + position;
			this->mShaders.erase(shaderPosition);
		}
	}

	void FullscreenManager::Render(unsigned int size, int count)
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

		ShaderVector::iterator start, current, end;

		start = this->mShaders.begin() + size; 

		if ( count == -1 )
		{
			end = this->mShaders.end();
		} else {
			end = start + count;
		}

		for ( current = start; current != end; ++current )
		{
			ShaderRef shader = *current;
			TechniqueRef tech = shader->GetDefaultTechnique();
			for ( size_t passNum = 0; passNum < tech->NumPasses(); ++passNum )
			{
				PassRef pass = tech->GetPass(passNum);
				adapter->BindPass(pass);
				adapter->DrawQuad();
				adapter->UnbindPass();
			}
		}
	}
}
