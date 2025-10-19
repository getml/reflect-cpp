#ifndef RFL_COMMON_HPP_
#define RFL_COMMON_HPP_


#ifdef RFL_BUILD_SHARED
    #ifdef _WIN32
        #ifdef reflectcpp_EXPORTS
            #define RFL_API __declspec(dllexport)
        #else
            #define RFL_API __declspec(dllimport)
        #endif
    #else
        #define RFL_API __attribute__((visibility("default")))
    #endif
#else
    #define RFL_API
#endif

#endif