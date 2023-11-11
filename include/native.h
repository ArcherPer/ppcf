/* Helpers for parsing pcf structures natively without any standard library */
#ifndef __PCF_NATIVE__
#define __PCF_NATIVE__
#include "internal.h"

/*|========================|*
 *|=====| STRUCTURES |=====|*
 *|========================|* 
 */

/* 
Commented fields are inaccessible through the native implementation
due to the nature of the pcf format and must be user gathered.
Higher level abstractions such as io fix this and are recommended.
*/

typedef struct {
    int32_t type;
    int32_t format;
    int32_t size;
    int32_t offset;
} pcf_native_table_entry_t;

typedef struct {
    int32_t magic;
    int32_t table_count;
//  pcf_native_table_entry_t table[table_count];
} pcf_native_header_t;

typedef struct {
    int32_t string_offset;
    int8_t string_valid;
    int32_t value;
} __attribute__((packed)) pcf_native_property_t;

typedef struct {
    int32_t format;
    int32_t nprops;
//  pcf_native_property_t prop[];
//  pad to next int32 boundary
//  pcf_native_strings_table strings;
} pcf_native_properties_table_t;

typedef struct {
    uint8_t bearing_left;
    uint8_t bearing_right;
    uint8_t width;
    uint8_t ascent;
    uint8_t descent;
} pcf_native_metrics_data_compressed_t;

typedef struct {
    int16_t bearing_left;
    int16_t bearing_right;
    int16_t width;
    int16_t ascent;
    int16_t descent;
    int16_t attributes;
} pcf_native_metrics_data_uncompressed_t;

typedef struct {
    int32_t format;
    uint8_t no_overlap;
    uint8_t constant_metrics;
    uint8_t font_terminal;
    uint8_t constant_width;
    uint8_t ink_inside;
    uint8_t ink_metrics;
    uint8_t draw_direction;
    int32_t font_ascent;
    int32_t font_descent;
    int32_t max_overlap;
//  pcf_native_metrics_data_uncompressed_t bounds[];
/*  bounds[0] = norm_min;
    bounds[1] = norm_max;
    if ink_metrics = 1
        bounds[2] = ink_min
        bounds[3] = ink_max */
} pcf_native_accelerator_table_t;

typedef struct {
    int32_t format;
    int32_t metrics_count;
/*    union {
        pcf_native_metrics_data_compressed_t compressed[];
        pcf_native_metrics_data_uncompressed_t uncompressed[];
    } metrics; */
} pcf_native_metrics_table_t;

typedef struct {
    int32_t format;
    int32_t glyph_count;
//  int32_t offset[];
//  int32_t bitmap_sizes[4];
//  char bitmap_data[];
} pcf_native_bitmap_table_t;

typedef struct {
    int32_t format;
    int16_t byte2_min;
    int16_t byte2_max;
    int16_t byte1_min;
    int16_t byte1_max;
    int16_t default_char;
} __attribute__((packed)) pcf_native_encoding_table_t;

typedef struct {
    int32_t format;
    int32_t glyph_count;
//  int32_t swidths[];
} pcf_native_swidths_table_t;

typedef struct {
    int32_t format;
    int32_t glyph_count;
//  int32_t offset[];
//  int32_t string_size;
//  char string[];
} pcf_native_glyph_names_table_t;

/*|========================|*
 *|=====| PROTOTYPES |=====|*
 *|========================|* 
 */

#define PCF_CALC_STRINGS_OFFSET(nprops) (((sizeof(pcf_native_properties_table_t) + (sizeof(pcf_native_property_t) * nprops)) + 3) & ~3)

void pcf_native_parse_header(void *ptr, pcf_native_header_t *dst);
void pcf_native_parse_table_entry(void *ptr, pcf_native_table_entry_t *dst);
void pcf_native_parse_properties_table(void *ptr, pcf_native_properties_table_t *dst);
void pcf_native_parse_property(void *ptr, pcf_native_property_t *dst, int32_t format);
void pcf_native_parse_metrics_data_uncompressed(void *ptr, pcf_native_metrics_data_uncompressed_t *dst, int32_t format);
void pcf_native_parse_metrics_data_compressed(void *ptr, pcf_native_metrics_data_compressed_t *dst, int32_t format);
void pcf_native_parse_accelerator_table(void *ptr, pcf_native_accelerator_table_t *dst);
void pcf_native_parse_metrics_table(void *ptr, pcf_native_metrics_table_t *dst);
void pcf_native_parse_bitmap_table(void *ptr, pcf_native_bitmap_table_t *dst);
void pcf_native_parse_encoding_table(void *ptr, pcf_native_encoding_table_t *dst);
void pcf_native_parse_swidths_table(void *ptr, pcf_native_swidths_table_t *dst);
void pcf_native_parse_glyph_names_table(void *ptr, pcf_native_glyph_names_table_t *dst);

#endif