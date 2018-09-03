#pragma once

#include "StdInclude.h"
#include "RedLiliumObject.h"

namespace RED_LILIUM_NAMESPACE
{

#if RED_LILIUM_POINTER_COUNTERS

template<class T>
class RedLiliumWeakPointer
{
public:
	~RedLiliumWeakPointer()
	{
	}

	RedLiliumWeakPointer(std::nullptr_t)
		: m_existingHandler(RedLiliumObject::m_nullExistingHandler)
		, m_ptr(nullptr)
	{}

	template<class U>
	RedLiliumWeakPointer(U* pointer)
	{
		static_assert(std::is_convertible<U*, T*>::value, "Cannot convert pointer from U to T");
		static_assert(std::is_convertible<U*, const RedLiliumObject*>::value, "Smart pointer supports only RedLiliumObject class");

		if (pointer)
		{
			m_existingHandler = static_cast<const RedLiliumObject*>(pointer)->m_existingHandler;
			m_ptr = static_cast<T*>(pointer);
			RED_LILIUM_ASSERT(IsValid());
		}
		else
		{
			m_existingHandler = RedLiliumObject::m_nullExistingHandler;
			m_ptr = nullptr;
		}
	}

	RedLiliumWeakPointer(const RedLiliumWeakPointer& r)
	{
		RED_LILIUM_ASSERT(r.IsValid());
		m_existingHandler = r.m_existingHandler;
		m_ptr = r.m_ptr;
	}

	template<class U>
	RedLiliumWeakPointer(const RedLiliumWeakPointer<U>& r)
	{
		static_assert(std::is_convertible<U*, T*>::value, "Cannot convert pointer from U to T");
		RED_LILIUM_ASSERT(r.IsValid());

		m_existingHandler = r.m_existingHandler;
		m_ptr = static_cast<T*>(r.m_ptr);
	}

	RedLiliumWeakPointer(RedLiliumWeakPointer&& r)
	{
		RED_LILIUM_ASSERT(r.IsValid());
		m_existingHandler = r.m_existingHandler;
		m_ptr = r.m_ptr;
	}

	template<class U>
	RedLiliumWeakPointer(RedLiliumWeakPointer<U>&& r)
	{
		static_assert(std::is_convertible<U*, T*>::value, "Cannot convert pointer from U to T");
		RED_LILIUM_ASSERT(r.IsValid());

		m_existingHandler = r.m_existingHandler;
		m_ptr = static_cast<T*>(r.m_ptr);
	}

	RedLiliumWeakPointer& operator=(const RedLiliumWeakPointer& r)
	{
		RED_LILIUM_ASSERT(r.IsValid());

		if (r.m_existingHandler != m_existingHandler || r.m_ptr != m_ptr)
		{
			m_existingHandler = r.m_existingHandler;
			m_ptr = r.m_ptr;
		}
		return *this;
	}

	template<class U>
	RedLiliumWeakPointer& operator=(const RedLiliumWeakPointer<U>& r)
	{
		static_assert(std::is_convertible<U*, T*>::value, "Cannot convert pointer from U to T");
		RED_LILIUM_ASSERT(r.IsValid());

		if (r.m_existingHandler != m_existingHandler || r.m_ptr != m_ptr)
		{
			m_existingHandler = r.m_existingHandler;
			m_ptr = static_cast<T*>(r.m_ptr);
		}
		return *this;
	}

	RedLiliumWeakPointer& operator=(RedLiliumWeakPointer&& r)
	{
		RED_LILIUM_ASSERT(r.IsValid());

		if (r.m_existingHandler != m_existingHandler || r.m_ptr != m_ptr)
		{
			m_existingHandler = r.m_existingHandler;
			m_ptr = r.m_ptr;
		}
		return *this;
	}

	template<class U>
	RedLiliumWeakPointer& operator=(RedLiliumWeakPointer<U>&& r)
	{
		static_assert(std::is_convertible<U*, T*>::value, "Cannot convert pointer from U to T");
		RED_LILIUM_ASSERT(r.IsValid());

		if (r.m_existingHandler != m_existingHandler || r.m_ptr != m_ptr)
		{
			m_existingHandler = r.m_existingHandler;
			m_ptr = static_cast<T*>(r.m_ptr);
		}
		return *this;
	}

	RedLiliumWeakPointer& operator=(std::nullptr_t) noexcept
	{
		RED_LILIUM_ASSERT(IsValid());
        m_existingHandler = RedLiliumObject::m_nullExistingHandler;
        m_ptr = nullptr;
		return *this;
	}

	T* operator->() const noexcept
	{
		RED_LILIUM_ASSERT(IsValid());
		return m_ptr;
	}

	T& operator*() const noexcept
	{
		RED_LILIUM_ASSERT(IsValid());
		return *m_ptr;
	}

	bool operator!() const noexcept
	{
		RED_LILIUM_ASSERT(IsValid());
		return (m_ptr == nullptr);
	}

	operator bool() const noexcept
	{
		RED_LILIUM_ASSERT(IsValid());
		return (m_ptr != nullptr);
	}

	void swap(RedLiliumWeakPointer& r)
	{
		RED_LILIUM_ASSERT(IsValid());
		RED_LILIUM_ASSERT(r.IsValid());
		std::swap(m_existingHandler, r.m_existingHandler);
		std::swap(m_ptr, r.m_ptr);
	}

	T* Get() const noexcept
	{
		RED_LILIUM_ASSERT(IsValid());
		return m_ptr;
	}

	bool IsValid() const noexcept
	{
		return (*m_existingHandler == true);
	}

	template<typename U>
	friend class RedLiliumWeakPointer;
	friend class RedLiliumObject;
public:

	T* m_ptr;
	sptr<std::atomic_bool> m_existingHandler;
};

template <typename T>
inline void swap(RedLiliumWeakPointer<T>& a, RedLiliumWeakPointer<T>& b) noexcept
{
	a.swap(b);
}

template <typename T, typename U>
inline bool operator==(const T* const a, const RedLiliumWeakPointer<U>& b) noexcept
{
	return a == b.Get();
}

template <typename T, typename U>
inline bool operator==(const RedLiliumWeakPointer<T>& a, const U* const b) noexcept
{
	return a.Get() == b;
}

template <typename T, typename U>
inline bool operator==(const RedLiliumWeakPointer<T>& a, const RedLiliumWeakPointer<U>& b) noexcept
{
	return a.Get() == b.Get();
}

template <typename T, typename U>
inline bool operator!=(const RedLiliumWeakPointer<T>& a, const RedLiliumWeakPointer<U>& b) noexcept
{
	return a.Get() != b.Get();
}

template <typename T, typename U>
inline bool operator<(const RedLiliumWeakPointer<T>& a, const RedLiliumWeakPointer<U>& b) noexcept
{
	return (a.Get() < b.Get());
}

template <typename T, typename U>
inline bool operator>(const RedLiliumWeakPointer<T>& a, const RedLiliumWeakPointer<U>& b) noexcept
{
	return (b < a);
}

template <typename T, typename U>
inline bool operator<=(const RedLiliumWeakPointer<T>& a, const RedLiliumWeakPointer<U>& b) noexcept
{
	return !(b < a);
}

template <typename T, typename U>
inline bool operator>=(const RedLiliumWeakPointer<T>& a, const RedLiliumWeakPointer<U>& b) noexcept
{
	return !(a < b);
}

template <typename T>
inline bool operator==(const RedLiliumWeakPointer<T>& a, std::nullptr_t) noexcept
{
	return !a;
}

template <typename T>
inline bool operator==(std::nullptr_t, const RedLiliumWeakPointer<T>& b) noexcept
{
	return !b;
}

template <typename T>
inline bool operator!=(const RedLiliumWeakPointer<T>& a, std::nullptr_t) noexcept
{
	return static_cast<bool>(a);
}

template <typename T>
inline bool operator!=(std::nullptr_t, const RedLiliumWeakPointer<T>& b) noexcept
{
	return static_cast<bool>(b);
}

template <typename T>
inline bool operator<(const RedLiliumWeakPointer<T>& a, std::nullptr_t) noexcept
{
	return (a.Get() < 0);
}

template <typename T>
inline bool operator<(std::nullptr_t, const RedLiliumWeakPointer<T>& b) noexcept
{
	return (0 < b.Get());
}

template <typename T>
inline bool operator>(const RedLiliumWeakPointer<T>& a, std::nullptr_t) noexcept
{
	return (nullptr < a);
}

template <typename T>
inline bool operator>(std::nullptr_t, const RedLiliumWeakPointer<T>& b) noexcept
{
	return (b < nullptr);
}

template <typename T>
inline bool operator<=(const RedLiliumWeakPointer<T>& a, std::nullptr_t) noexcept
{
	return !(nullptr < a);
}

template <typename T>
inline bool operator<=(std::nullptr_t, const RedLiliumWeakPointer<T>& b) noexcept
{
	return !(b < nullptr);
}

template <typename T>
inline bool operator>=(const RedLiliumWeakPointer<T>& a, std::nullptr_t) noexcept
{
	return !(a < nullptr);
}

template <typename T>
inline bool operator>=(std::nullptr_t, const RedLiliumWeakPointer<T>& b) noexcept
{
	return !(nullptr < b);
}

template<class T>
using ptr = RedLiliumWeakPointer<T>;

template<class T, class R>
ptr<T> Cast(ptr<R> ref)
{
	T* pointer = dynamic_cast<T*>(ref.Get());
	return ptr<T>(pointer);
}

template<class T, class R>
ptr<T> Cast(const uptr<R>& ref)
{
	T* pointer = dynamic_cast<T*>(ref.get());
	return ptr<T>(pointer);
}

template<class T, class R>
ptr<T> ConstCast(ptr<R> ref)
{
	T* pointer = const_cast<T*>(ref.Get());
	return ptr<T>(pointer);
}

#else

template<class T>
using ptr = T*;

template<class T, class R>
ptr<T> Cast(ptr<R> ref)
{
	T* pointer = dynamic_cast<T*>(ref);
	return ptr<T>(pointer);
}

template<class T, class R>
ptr<T> Cast(uptr<R>& ref)
{
	T* pointer = dynamic_cast<T*>(ref.get());
	return ptr<T>(pointer);
}

template<class T, class R>
ptr<T> ConstCast(ptr<R> ref)
{
	T* pointer = const_cast<T*>(ref);
	return ptr<T>(pointer);
}

#endif // RED_LILIUM_POINTER_COUNTERS

} // RED_LILIUM_NAMESPACE


#if RED_LILIUM_POINTER_COUNTERS

namespace std
{
	template <class T> struct hash<RED_LILIUM_NAMESPACE::RedLiliumWeakPointer<T>>
	{
		size_t operator()(const RED_LILIUM_NAMESPACE::RedLiliumWeakPointer<T>& a) const
		{
			// TODO: если есть инвариантность, то хеш не постоянен - исправить
			return std::hash<T*>()(a.Get());
		}
	};
}
#endif // RED_LILIUM_POINTER_COUNTERS
