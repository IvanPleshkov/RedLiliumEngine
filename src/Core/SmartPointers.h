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
		: m_handler(RedLiliumObject::m_nullHandler)
	{}

	template<class U>
	RedLiliumWeakPointer(U* pointer)
	{
		static_assert(std::is_convertible<U*, T*>::value, "Cannot convert pointer from U to T");
		if (pointer)
		{
			m_handler = reinterpret_cast<const RedLiliumObject*>(pointer)->m_handler;
			RED_LILIUM_ASSERT(pointer == m_handler->ptr);
		}
		else
		{
			m_handler = RedLiliumObject::m_nullHandler;
		}
	}

	RedLiliumWeakPointer(const RedLiliumWeakPointer& r)
	{
		RED_LILIUM_ASSERT(r.IsValid());
		m_handler = r.m_handler;
	}

	template<class U>
	RedLiliumWeakPointer(const RedLiliumWeakPointer<U>& r)
	{
		RED_LILIUM_ASSERT(r.IsValid());
		m_handler = r.m_handler;
	}

	RedLiliumWeakPointer(RedLiliumWeakPointer&& r)
	{
		RED_LILIUM_ASSERT(r.IsValid());
		m_handler = std::move(r.m_handler);
		r.m_handler = nullptr;
	}

	template<class U>
	RedLiliumWeakPointer(RedLiliumWeakPointer<U>&& r)
	{
		static_assert(std::is_convertible<U*, T*>::value, "Cannot convert pointer from U to T");

		RED_LILIUM_ASSERT(r.IsValid());
		m_handler = r.m_handler;
	}

	RedLiliumWeakPointer& operator=(const RedLiliumWeakPointer& r)
	{
		RED_LILIUM_ASSERT(r.IsValid());

		if (&r != this)
		{
			m_handler = r.m_handler;
		}
		return *this;
	}

	template<class U>
	RedLiliumWeakPointer& operator=(const RedLiliumWeakPointer<U>& r)
	{
		static_assert(std::is_convertible<U*, T*>::value, "Cannot convert pointer from U to T");

		RED_LILIUM_ASSERT(r.IsValid());

		if (&r != this)
		{
			m_handler = r.m_handler;
		}
		return *this;
	}

	RedLiliumWeakPointer& operator=(RedLiliumWeakPointer&& r)
	{
		RED_LILIUM_ASSERT(r.IsValid());

		if (&r != this)
		{
			m_handler = r.m_handler;
		}
		return *this;
	}

	template<class U>
	RedLiliumWeakPointer& operator=(RedLiliumWeakPointer<U>&& r)
	{
		static_assert(std::is_convertible<U*, T*>::value, "Cannot convert pointer from U to T");

		RED_LILIUM_ASSERT(r.IsValid());

		if (&r != this)
		{
			m_handler = r.m_handler;
		}
		return *this;
	}

	RedLiliumWeakPointer& operator=(std::nullptr_t) noexcept
	{
		RED_LILIUM_ASSERT(IsValid());
		m_handler = RedLiliumObject::m_nullHandler;
		return *this;
	}

	T* operator->() const noexcept
	{
		RED_LILIUM_ASSERT(IsValid());
		RED_LILIUM_ASSERT(m_handler->ptr != nullptr);
		return reinterpret_cast<T*>(m_handler->ptr);
	}

	T& operator*() const noexcept
	{
		RED_LILIUM_ASSERT(IsValid());
		RED_LILIUM_ASSERT(m_handler->ptr != nullptr);
		return *reinterpret_cast<T*>(m_handler->ptr);
	}

	bool operator!() const noexcept
	{
		RED_LILIUM_ASSERT(IsValid());
		return m_handler->ptr == nullptr;
	}

	operator bool() const noexcept
	{
		RED_LILIUM_ASSERT(IsValid());
		return (m_handler->ptr != nullptr);
	}

	void swap(RedLiliumWeakPointer& r)
	{
		RED_LILIUM_ASSERT(IsValid());
		RED_LILIUM_ASSERT(r.IsValid());
		std::swap(m_handler, r.m_handler);
	}

	T* Get() const noexcept
	{
		RED_LILIUM_ASSERT(IsValid());
		return reinterpret_cast<T*>(m_handler->ptr);
	}

	bool IsValid() const noexcept
	{
		return m_handler->exists;
	}

	template<typename U>
	friend class RedLiliumWeakPointer;
	friend class RedLiliumObject;
private:

	sptr<RedLiliumPtrHandler> m_handler;
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
ptr<T> Cast(const ptr<R>& ref)
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
