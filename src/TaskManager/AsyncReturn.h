#pragma once

#include "TaskManagerForward.h"

namespace RED_LILIUM_NAMESPACE
{

template<class T>
class AsyncResult
{
public:
	AsyncResult(const T& waitingValue);
	const T& Get() const;
	bool IsReady() const;

private:
    template<class I>
	friend class AsyncReturn;

	using SharedResult = std::pair<T, std::atomic<bool>>;
	sptr<SharedResult> m_result;
};

template<class T>
class AsyncReturn
{
public:
	AsyncReturn(const T& waitingValue);
	//AsyncReturn(AsyncReturn&& asyncReturn);
	//AsyncReturn(const AsyncReturn&) = delete; // non construction-copyable
	//AsyncReturn& operator=(const AsyncReturn&) = delete; // non copyable

	const AsyncResult<T>& GetResult();
	void SetReturn(const T& value);
	void SetReturn(T&& value);

private:
	AsyncResult<T> m_result;
};

template<class T>
inline AsyncResult<T>::AsyncResult(const T& waitingValue)
{
	m_result = smake<SharedResult>(waitingValue, false);
}

//template<class T>
//inline AsyncReturn<T>::AsyncReturn(AsyncReturn && asyncReturn)
//{
//	m_result = asyncReturn.m_result;
//	asyncReturn.m_result.m_result = nullptr;
//}

//template<class T>
//inline AsyncReturn<T>::AsyncReturn(AsyncReturn&& asyncReturn)
//{
//	m_result = asyncReturn.m_result;
//	asyncReturn.m_result.m_result = nullptr;
//}

template<class T>
inline const T& AsyncResult<T>::Get() const
{
	RED_LILIUM_ASSERT(IsReady());
	return m_result->first;
}

template<class T>
inline bool AsyncResult<T>::IsReady() const
{
	return m_result->second;
}

template<class T>
inline AsyncReturn<T>::AsyncReturn(const T& waitingValue)
	: m_result(waitingValue)
{ }

template<class T>
inline const AsyncResult<T>& AsyncReturn<T>::GetResult()
{
	return m_result;
}

template<class T>
inline void AsyncReturn<T>::SetReturn(const T& value)
{
	RED_LILIUM_ASSERT(!m_result->IsReady());

	m_result->m_result->first = value;
	m_result->m_result->second = true;
}

template<class T>
inline void AsyncReturn<T>::SetReturn(T&& value)
{
	RED_LILIUM_ASSERT(!m_result->IsReady());

	m_result->m_result->first = std::move(value);
	m_result->m_result->second = true;
}

} // namespace RED_LILIUM_NAMESPACE
