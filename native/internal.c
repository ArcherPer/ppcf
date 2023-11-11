#include "internal.h"
#include "compiler.h"

/* Bit manipulation fill-ins (only used if no __builtins are found. See compiler.h for more details) */

static inline uint16_t pcf_bswap16(uint16_t val) {
    return ((val >> 8) | (val << 8));
}

static inline uint32_t pcf_bswap32(uint32_t val) {
    return ((val >> 24) & 0xFF) | ((val >> 8) & 0xFF00) | ((val << 8) & 0xFF0000) | ((val << 24) & 0xFF000000);
}

static inline uint8_t pcf_bitreverse8(uint8_t val) {
    val = (val & 0xAA) >> 1 | (val & 0x55) << 1;
    val = (val & 0xCC) >> 2 | (val & 0x33) << 2;
    val = (val & 0xF0) >> 4 | (val & 0x0F) << 4;
    return val;
}

static inline uint16_t pcf_bitreverse16(uint16_t val) {
    val = (val & 0xAAAA) >> 1 | (val & 0x5555) << 1;
    val = (val & 0xCCCC) >> 2 | (val & 0x3333) << 2;
    val = (val & 0xF0F0) >> 4 | (val & 0x0F0F) << 4;
    val = (val & 0xFF00) >> 8 | (val & 0x00FF) << 8;
    return val;
}

static inline uint32_t pcf_bitreverse32(uint32_t val) {
    val = (val & 0xAAAAAAAA) >> 1 | (val & 0x55555555) << 1;
    val = (val & 0xCCCCCCCC) >> 2 | (val & 0x33333333) << 2;
    val = (val & 0xF0F0F0F0) >> 4 | (val & 0x0F0F0F0F) << 4;
    val = (val & 0xFF00FF00) >> 8 | (val & 0x00FF00FF) << 8;
    val = (val >> 16) | (val << 16);
    return val;
}

/* Simple parsing */

#if defined __BYTE_ORDER__ && (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
    #define IS_NATIVE_ENDIAN(fmt) (PCF_CHECK_ENDIANESS(fmt)?1:0)
#else 
    #define IS_NATIVE_ENDIAN(fmt) (PCF_CHECK_ENDIANESS(fmt)?0:1)
#endif

int8_t pcf_parse_8(void *ptr, int32_t fmt) {
    int8_t val = *(int8_t*)ptr;
    if (PCF_CHECK_BIT_ORDER(fmt) == PCF_ORDER_LSB) {
        val = BIT_REVERSE_8(val);
    }
    return val;
}

int16_t pcf_parse_16(void *ptr, int32_t fmt) {
    int16_t val = *(int16_t*)ptr;
    if (!IS_NATIVE_ENDIAN(fmt)) {
        val = BYTE_SWAP_16(val);
    }
    if (PCF_CHECK_BIT_ORDER(fmt) == PCF_ORDER_LSB) {
        val = BIT_REVERSE_16(val);
    }
    return val;
}

int32_t pcf_parse_32(void *ptr, int32_t format) {
    int32_t val = *(int32_t*)ptr;
    if (!IS_NATIVE_ENDIAN(format)) {
        val = BYTE_SWAP_32(val);
    }
    if (PCF_CHECK_BIT_ORDER(format) == PCF_ORDER_LSB) {
        val = BIT_REVERSE_32(val);
    }
    return val;
}