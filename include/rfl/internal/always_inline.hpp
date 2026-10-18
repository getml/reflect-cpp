#ifndef RFL_INTERNAL_ALWAYS_INLINE_HPP_
#define RFL_INTERNAL_ALWAYS_INLINE_HPP_

#if defined(_MSC_VER)
#define RFL_ALWAYS_INLINE __forceinline
#elif defined(__GNUC__) || defined(__clang__)
#define RFL_ALWAYS_INLINE inline __attribute__((always_inline))
#else
#define RFL_ALWAYS_INLINE inline
#endif

#endif
