#pragma once

#include "StdInclude.h"
#include "ExternalInclude.h"

#define RED_LILIUM_POINTER_COUNTERS 1
// #define RED_LILIUM_USE_RESTORABLE_POINTERS 0
#define RED_LILIUM_USE_ASSERTS 1
#define RED_LILIUM_USE_TELEMETRY 1
#define RED_LILIUM_USE_GPU_MEMORY_PROFILER 1
#define RED_LILIUM_USE_MEMORY_PROFILER 1
#define RED_LILIUM_RENDER_DEBUG 1
#define RED_LILIUM_USE_DUMPS 1

#define RED_LILIUM_GUARD(LEVEL)
#define RED_LILIUM_SCOPE(LEVEL, NAME)

#define RED_LILIUM_NAMESPACE rl

namespace RED_LILIUM_NAMESPACE
{

using i8 = int8_t;
using u8 = uint8_t;
using i16 = int16_t;
using u16 = uint16_t;
using i32 = int32_t;
using u32 = uint32_t;
using i64 = int64_t;
using u64 = uint64_t;
using f32 = float;
using f64 = double;

const i8  i8_max = std::numeric_limits<i8>::max();
const u8  u8_max = std::numeric_limits<u8>::max();
const i16 i16_max = std::numeric_limits<i16>::max();
const u16 u16_max = std::numeric_limits<u16>::max();
const i32 i32_max = std::numeric_limits<i32>::max();
const u32 u32_max = std::numeric_limits<u32>::max();
const i64 i64_max = std::numeric_limits<i64>::max();
const u64 u64_max = std::numeric_limits<u64>::max();

template<class T>
using ptr = T*;
template<class T>
using sptr = std::shared_ptr<T>;
template<class T>
using wptr = std::weak_ptr<T>;
template<class T>
using uptr = std::unique_ptr<T>;

using json = nlohmann::json;

template<class T>
using span = nonstd::span_lite::span<T>;

template<class T, class... TArgs>
[[nodiscard]] inline uptr<T> umake(TArgs&&... args)
{
	return std::make_unique<T>(std::forward<TArgs>(args)...);
}

template<class T, class... TArgs>
[[nodiscard]] inline sptr<T> smake(TArgs&&... args)
{
	return std::make_shared<T>(std::forward<TArgs>(args)...);
}

/*
template<class T>
using vector = std::vector<T>;
template<class T>
using set = std::set<T>;
*/

using vec2 = glm::vec2;
using vec3 = glm::vec3;
using vec4 = glm::vec4;
using mat4 = glm::mat4;
using mat3 = glm::mat3;
using mat2 = glm::mat2;
using quat = glm::quat;

using dvec2 = glm::dvec2;
using dvec3 = glm::dvec3;
using dvec4 = glm::dvec4;
using dmat4 = glm::dmat4;
using dmat3 = glm::dmat3;
using dmat2 = glm::dmat2;

struct Rect
{
	vec2 position;
	vec2 size;
};

using Color32 = u32;

#if RED_LILIUM_USE_ASSERTS
#define RED_LILIUM_ASSERT(condition) assert(condition);
// #define RED_LILIUM_ASSERT(condition) if(!(condition)) { throw std::exception(); }
#define RED_LILIUM_NOT_IMPLEMENTED() assert(false && "Not Implemented!");
#else
#define RED_LILIUM_ASSERT(condition)
#define RED_LILIUM_NOT_IMPLEMENTED(message)
#endif // RED_LILIUM_USE_ASSERTS

#define RED_LILIUM_CRITICAL_ERROR() assert(false)

#define RED_LILIUM_INTERNAL public

template<typename Test, template<typename...> class Ref>
struct IsSpecialization : std::false_type {};

template<template<typename...> class Ref, typename... Args>
struct IsSpecialization<Ref<Args...>, Ref> : std::true_type {};

template <class T>
inline void ClearStruct(T* data) {
	std::memset(data, 0, sizeof(T));
}

class RedLiliumObject
{
public:
	virtual ~RedLiliumObject() {}
};

class RestorableObject : public RedLiliumObject
{
public:
	~RestorableObject() override = default;
};

class NonCopyable
{
public:
	NonCopyable(const NonCopyable&) = delete;
	NonCopyable& operator=(const NonCopyable&) = delete;

	NonCopyable(NonCopyable&&) = default;
	NonCopyable& operator=(NonCopyable&&) = default;

protected:
	NonCopyable() = default;
};

class NonMovable
{
public:
	NonMovable(const NonMovable&) = default;
	NonMovable& operator=(const NonMovable&) = default;

	NonMovable(NonMovable&&) = delete;
	NonMovable& operator=(NonMovable&&) = delete;

protected:
	NonMovable() = default;
};

#define RED_LILIUM_LOG_INFO(message) spdlog::info(message);
#define RED_LILIUM_LOG_WARNING(message) spdlog::warn(message);
#define RED_LILIUM_LOG_ERROR(message) spdlog::error(message);
#define RED_LILIUM_LOG_CRITICAL(message) spdlog::critical(message);
#define RED_LILIUM_LOG_DEBUG(message) spdlog::debug(message);


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

template<class T, class R>
ptr<T> ConstCast(uptr<R> ref)
{
	T* pointer = const_cast<T*>(ref.get());
	return ptr<T>(pointer);
}

}
