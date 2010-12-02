/**************************************************************************************************\
* This file is part of the Voodoo Shader Framework, a comprehensive shader support library.
* Copyright (C) 2010 by Sean Sube
*
*
* This program is free software; you can redistribute it and/or modify it under the terms of the 
* GNU General Public License as published by the Free Software Foundation; either version 2 of the 
* License, or (at your option) any later version.
* 
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
* even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
* General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with this program; 
* if  not, write to the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, 
* Boston, MA  02110-1301 US
*
* Support and more information may be found at http://www.voodooshader.com, or by contacting the
* developer at peachykeen@voodooshader.com
\**************************************************************************************************/

#ifndef VOODOO_REFERENCE_HPP
#define VOODOO_REFERENCE_HPP

namespace VoodooShader
{
	/**
	 * Simple reference-counter pointer class.
	 *
	 * @note This class is not thread-safe at this point.
	 */
	template<typename T>
	class Reference
	{
	public:
		Reference()
			: mObject(NULL)
		{
			mCount = new int(1);
		}

		Reference(const T & object)
		{
			mObject = new T(object);
			mCount = new int(1);
		}

		Reference(T * object, int count = 1)
			: mObject(object)
		{
			mCount = new int(count);
		}

		Reference(void * object)
			: mObject(reinterpret_cast<T*>(object))
		{
			mCount = new int(1);
		}

		Reference(const Reference<T> & other)
			: mObject(other.mObject), mCount(other.mCount)
		{
			++(*mCount);
		}

		~Reference()
		{
			this->release();
		}

		Reference<T> & operator= (const Reference<T> & other)
		{
			if ( this != &other )
			{
				release();
				mObject = other.mObject;
				mCount = other.mCount;
				++(*mCount);
			}
			return *this;
		}

		Reference<T> & operator= (T * object)
		{
			release();
			mObject = object;
			mCount = new int(1);

			return *this;
		}

		T & operator*()
		{
			return (*mObject);
		}

		T * operator->()
		{
			return mObject;
		}

		T * get()
		{
			return mObject;
		}

		int count()
		{
			return (*mCount);
		}

		template<typename U, typename V>
		friend bool operator<(const Reference<U> & me, const Reference<V> & other);

	private:
		T * mObject;
		int * mCount;

		void release()
		{
			if ( mObject )
			{
				--(*mCount);

				if ( (*mCount) <= 0 )
				{
					// If the count has reached zero, destroy the counter and object.
					delete mCount;
					mCount = NULL;
					delete mObject;
					mObject = NULL;
				}
			}
		}
	};

	template<typename U, typename V>
	bool operator<(const Reference<U> & me, const Reference<V> & other)
	{
		return ( me.mObject < other.mObject );
	}
}

#endif /* VOODOO_REFERENCE_HPP */
