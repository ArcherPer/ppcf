/* Compiler specific builtins are preferred for bit / byte manipulation */

#if defined __GNUC__ && __GNUC_MINOR__
    #if __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 3)
        #define BYTE_SWAP_16(val) __builtin_bswap16(val)
        #define BYTE_SWAP_32(val) __builtin_bswap32(val)
    #endif

#elif defined __clang__ && __clang_major__ && __clang_minor
    #if __clang_major__ > 1 || (__clang_major__ == 1 && __clang_minor__ >= 7)
        #define BYTE_SWAP_16(val) __builtin_bswap16(val)
        #define BYTE_SWAP_32(val) __builtin_bswap_32(val)
    #endif
    #if __clang_major__ > 3 || (__clang_major__ == 3 && __clang_minor__ >= 8)
        #define BIT_REVERSE_8(val)  __builtin_bitreverse8(val)
        #define BIT_REVERSE_16(val) __builtin_bitreverse16(val)
        #define BIT_REVERSE_32(val) __builtin_bitreverse32(val)
    #endif
#endif

/* Default definitions */

#ifndef BYTE_SWAP_16
    #define BYTE_SWAP_16(val) pcf_bswap16(val)
#endif

#ifndef BYTE_SWAP_32
    #define BYTE_SWAP_32(val) pcf_bswap32(val)
#endif

#ifndef BIT_REVERSE_8
    #define BIT_REVERSE_8(val) pcf_bitreverse8(val)
#endif

#ifndef BIT_REVERSE_16
    #define BIT_REVERSE_16(val) pcf_bitreverse16(val)
#endif

#ifndef BIT_REVERSE_32
    #define BIT_REVERSE_32(val) pcf_bitreverse32(val)
#endif