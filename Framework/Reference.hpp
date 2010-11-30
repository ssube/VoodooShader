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

		template<typename U, typename V>
		friend bool operator<(const Reference<U> & me, const Reference<V> & other);

	private:
		T * mObject;
		int * mCount;

		void release()
		{
			--(*mCount);

			if ( (*mCount) <= 0 )
			{
				// If the count has reached zero, destroy the counter and object.
				delete mCount;
				delete mObject;
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