#ifndef __PCF_INTERNAL__
#define __PCF_INTERNAL__
#include <stdint.h>

/*|========================|*
 *|====| MAGIC NUMBER |====|*
 *|========================|* 
 */

#define PCF_MAGIC (('p'<<24)|('c'<<16)|('f'<<8)|1)

/*|=======================|*
 *|====| TABLE TYPES |====|*
 *|=======================|* 
 */

#define PCF_TYPE_PROPERTIES       (1<<0)
#define PCF_TYPE_ACCELERATORS     (1<<1)
#define PCF_TYPE_METRICS          (1<<2)
#define PCF_TYPE_BITMAPS          (1<<3)
#define PCF_TYPE_INK_METRICS      (1<<4)
#define PCF_TYPE_BDF_ENCODINGS    (1<<5)
#define PCF_TYPE_SWIDTHS          (1<<6)
#define PCF_TYPE_GLYPH_NAMES      (1<<7)
#define PCF_TYPE_BDF_ACCELERATORS (1<<8)

/*|========================|*
 *|====| TABLE FORMAT |====|*
 *|========================|* 
 */

#define PCF_FORMAT_DEFAULT            (0<<0)
#define PCF_FORMAT_INKBOUNDS          (1<<9)
#define PCF_FORMAT_ACCEL_W_INKBOUNDS  (1<<8)
#define PCF_FORMAT_COMPRESSED_METRICS (1<<8)
#define PCF_FORMAT_MASK               ((0xff << 24) | (0xff << 16) | (0xff << 8))

#define PCF_ENDIAN_BIG     (1<<2)
#define PCF_ENDIAN_LITTLE  (0<<2)

#define PCF_ORDER_MSB      (1<<3)
#define PCF_ORDER_LSB      (0<<3)

#define PCF_SCANLINE_PAD_MASK  (3<<0)
#define PCF_SCANLINE_UNIT_MASK (3<<4)

/*|========================|*
 *|=======| MACROS |=======|*
 *|========================|* 
 */

#define PCF_CHECK_ENDIANESS(fmt) ((fmt) & PCF_ENDIAN_BIG)
#define PCF_CHECK_BIT_ORDER(fmt) ((fmt) & PCF_ORDER_MSB)

#define PCF_CHECK_FORMAT(fmt) ((fmt) & PCF_FORMAT_MASK)

#define PCF_CHECK_SCANLINE_PAD(fmt) (8 << ((fmt) & PCF_SCANLINE_PAD_MASK))
#define PCF_CHECK_SCANLINE_UNIT(fmt) (8 << ((fmt) & PCF_SCANLINE_UNIT_MASK))

/*|========================|*
 *|=====| PROTOTYPES |=====|*
 *|========================|* 
 */

int8_t pcf_parse_8(void *ptr, int32_t fmt);
int16_t pcf_parse_16(void *ptr, int32_t fmt);
int32_t pcf_parse_32(void *ptr, int32_t format);

#endif