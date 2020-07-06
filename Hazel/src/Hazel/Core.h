#ifndef _CORE_H
#define _CORE_H

#ifdef HZ_ENABLE_ASSERTS
    #define HZ_CORE_ASSERT(x, ...) { if (!(x)) { HZ_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
    #define HZ_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (x << x)

#define HZ_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

#endif // _CORE_H