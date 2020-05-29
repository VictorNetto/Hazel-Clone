#ifndef _CORE_H
#define _CORE_H

#ifdef HZ_ENABLE_ASSERTS
    #define HZ_CORE_ASSERT(x, ...) { if (!(x)) { HZ_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
    #define HZ_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (x << x)

#endif // _CORE_H