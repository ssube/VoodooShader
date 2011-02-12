#include "Includes.hpp"
#include "MaterialManager.hpp"
#include "Adapter.hpp"
#include "Core.hpp"
#include "Exception.hpp"
#include "Shader.hpp"

namespace VoodooShader
{
	MaterialManager::MaterialManager(Core * parent)
		: mParent(parent), mDefault(), mBound()
	{ };

	void MaterialManager::Add(TextureRef texture, ShaderRef shader, bool global)
	{
		// Search the map for the texture
		MaterialMap::iterator material = this->mMaterials.find(texture);

		if ( material != this->mMaterials.end() )
		{
			// Rebind it if found
			material->second = shader;
		} else {
			// Insert this material into the map
			this->mMaterials[texture] = shader;
		}

		// If the global parameter is set, set this as the global shader.
		if ( global )
		{
			this->mDefault = shader;
		}
	}

	void MaterialManager::Remove(TextureRef texture)
	{
		MaterialMap::iterator material = this->mMaterials.find(texture);

		if ( material != this->mMaterials.end() )
		{
			this->mMaterials.erase(material);	
		} else {
			Throw("Voodoo Core: Texture not present in material map.", this->mParent);
		}
	}

	void MaterialManager::Bind(TextureRef texture)
	{
		Adapter * adapter = this->mParent->GetAdapter();

		if ( !adapter )
		{
			Throw(
				"Voodoo Core: Attempted to bind material with no adapter set.", 
				this->mParent);
		}

		MaterialMap::iterator material = this->mMaterials.find(texture);

		if ( material != this->mMaterials.end() )
		{
			ShaderRef shader = material->second;
			TechniqueRef tech = shader->GetDefaultTechnique();
			//! @todo Support multiple passes for material shaders.
			//for ( size_t passNum = 0; passNum < tech->NumPasses(); ++passNum )
			//{
				PassRef pass = tech->GetPass(0);
				adapter->BindPass(pass);
			//	adapter->DrawFSQuad();
			//}
			//adapter->BindPass(pass);
			this->mBound = shader;		
		} else {
			if ( this->mDefault.get() )
			{
				TechniqueRef tech = this->mDefault->GetDefaultTechnique();
				PassRef pass = tech->GetPass(0);
				adapter->BindPass(pass);
				this->mBound = this->mDefault;
			} else {
				Throw("Voodoo Core: Could not find material to bind.", this->mParent);
			}
		}
	}

	void MaterialManager::Unbind()
	{
		if ( this->mBound.get() )
		{
			Adapter * adapter = this->mParent->GetAdapter();

			if ( !adapter )
			{
				Throw(
				  "Voodoo Core: Attempted to unbind material with no adapter set.", 
				  this->mParent);
			}

			adapter->UnbindPass();
			this->mBound = ShaderRef();
		}
	}


	ShaderRef MaterialManager::Find(TextureRef texture)
	{
		MaterialMap::iterator material = this->mMaterials.find(texture);

		if ( material != this->mMaterials.end() )
		{
			return material->second;
		} else {
			return ShaderRef();
		}
	}
}
