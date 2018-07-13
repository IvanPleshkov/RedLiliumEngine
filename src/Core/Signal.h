#pragma once

#include "StdInclude.h"
#include <signals/Delegate.h>
#include <signals/Signal.h>

namespace RED_LILIUM_NAMESPACE
{

template<int count, class ...T>
class SignalImpl;

template<class ...T>
class SignalImpl<0, T...> : public Gallant::Signal0<> {};

template<class ...T>
class SignalImpl<1, T...> : public Gallant::Signal1<T...> {};

template<class ...T>
class SignalImpl<2, T...> : public Gallant::Signal2<T...> {};

template<class ...T>
class SignalImpl<3, T...> : public Gallant::Signal3<T...> {};

template<class ...T>
class SignalImpl<4, T...> : public Gallant::Signal4<T...> {};

template<class ...T>
class SignalImpl<5, T...> : public Gallant::Signal5<T...> {};

template<class ...T>
class SignalImpl<6, T...> : public Gallant::Signal6<T...> {};

template<class ...T>
class SignalImpl<7, T...> : public Gallant::Signal7<T...> {};

template<class ...T>
class SignalImpl<8, T...> : public Gallant::Signal8<T...> {};

template<class ...T>
class Signal : public SignalImpl<sizeof...(T), T...> {};

template<int count, class ...T>
class DelegateImpl;

template<class ...T>
class DelegateImpl<0, T...> : public Gallant::Delegate0<> {};

template<class ...T>
class DelegateImpl<1, T...> : public Gallant::Delegate1<T...> {};

template<class ...T>
class DelegateImpl<2, T...> : public Gallant::Delegate2<T...> {};

template<class ...T>
class DelegateImpl<3, T...> : public Gallant::Delegate3<T...> {};

template<class ...T>
class DelegateImpl<4, T...> : public Gallant::Delegate4<T...> {};

template<class ...T>
class DelegateImpl<5, T...> : public Gallant::Delegate5<T...> {};

template<class ...T>
class DelegateImpl<6, T...> : public Gallant::Delegate6<T...> {};

template<class ...T>
class DelegateImpl<7, T...> : public Gallant::Delegate7<T...> {};

template<class ...T>
class DelegateImpl<8, T...> : public Gallant::Delegate8<T...> {};

template<class ...T>
class Delegate : public DelegateImpl<sizeof...(T), T...> {};

} // RED_LILIUM_NAMESPACE
