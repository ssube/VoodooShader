
#ifndef VOODOO_PARAMETER_HPP
#define VOODOO_PARAMETER_HPP

#include "Meta.hpp"
#include "Reference.hpp"

namespace VoodooShader
{
	class VOODOO_API Parameter
	{
	public:
		Parameter(Core * parent, String name, ParameterType type);

		/**
		 * Binds two parameters, forcing their values to be identical. This should
		 * be used with care, as it has some requirements and restrictions. It is
		 * used to control some of the more common variables found in shaders.
		 *
		 * @warning This <em>cannot</em> be used to bind one effect's parameter to
		 *		another. It can only be used to bind effect parameters to virtual
		 *		or global parameters.
		 */
		void Link(ParameterRef param);

		CGparameter Param(void);
		ParameterType Type(void);

		void Set(TextureRef newTex);
		void Set(float newX);
		void Set(float newX, float newY);
		void Set(float newX, float newY, float newZ);
		void Set(float newX, float newY, float newZ, float newW);

		void Get(TextureRef & param);
		void Get(float & paramX);
		void Get(float & paramX, float & paramY);
		void Get(float & paramX, float & paramY, float & paramZ);
		void Get(float & paramX, float & paramY, float & paramZ, float & paramW);

	private:
		bool mVirtual;
		CGparameter mParam;
		ParameterType mType;

		// Value
		TextureRef mValueTexture;
		float mValueFloat[4];
	};
}

#endif /*VOODOO_PARAMETER_HPP*/