#include "native.h"

/*
assumes ptr is pointing to the start of the data structure to parse

for memory mapped shared:
pass offset in mem map to ptr and user allocated structure for dst

for memory mapped private:
pass offset in mem map as both ptr and dst

for file io
fseek the offset and read the data into a buffer then pass the buffer as ptr and dst

why is ptr void?
so for mem map you dont have to keep doing type cast gymnastics to work with the functions
*/

void pcf_native_parse_header(void *ptr, pcf_native_header_t *dst) {
    uint8_t *src = (uint8_t*)ptr;
    if (!ptr || !dst) {
        return;
    }

    dst->magic = *(int32_t*)src;
    src += 4;

    dst->table_count = pcf_parse_32(src,PCF_ENDIAN_LITTLE|PCF_ORDER_MSB);
    return;
}

void pcf_native_parse_table_entry(void *ptr, pcf_native_table_entry_t *dst) {
    uint8_t *src = (uint8_t*)ptr;
    if (!ptr || !dst) {
        return;
    }

    int32_t format = PCF_ENDIAN_LITTLE|PCF_ORDER_MSB;
    int32_t val[4];
    for (int8_t i = 0; i < 4; i++) {
        val[i] = pcf_parse_32(src,format);
        src += 4;
    }

    dst->type   = val[0];
    dst->format = val[1];
    dst->size   = val[2];
    dst->offset = val[3];
    return;
}

void pcf_native_parse_properties_table(void *ptr, pcf_native_properties_table_t *dst) {
    uint8_t *src = (uint8_t*)ptr;
    if (!ptr || !dst) {
        return;
    }

    dst->format = *(int32_t*)src;
    src += 4;

    dst->nprops = pcf_parse_32(src,dst->format);

    return;
}

void pcf_native_parse_property(void *ptr, pcf_native_property_t *dst, int32_t format) {
    uint8_t *src = (uint8_t*)ptr;
    if (!ptr || !dst) {
        return;
    }

    dst->string_offset = pcf_parse_32(src,format);
    src += 4;

    dst->string_valid = pcf_parse_8(src,format);
    src += 1;

    dst->value = pcf_parse_32(src,format);

    return;
}

void pcf_native_parse_metrics_data_uncompressed(void *ptr, pcf_native_metrics_data_uncompressed_t *dst, int32_t format) {
    uint8_t *src = (uint8_t*)ptr;
    if (!ptr || !dst) {
        return;
    }

    int16_t val[6];
    for (uint8_t i = 0; i < 6; i++) {
        val[i] = pcf_parse_16(src,format);
        src += 2;
    }

    dst->bearing_left  = val[0];
    dst->bearing_right = val[1];
    dst->width         = val[2];
    dst->ascent        = val[3];
    dst->descent       = val[4];
    dst->attributes    = val[5];
    return;
}

void pcf_native_parse_metrics_data_compressed(void *ptr, pcf_native_metrics_data_compressed_t *dst, int32_t format) {
    uint8_t *src = (uint8_t*)ptr;
    if (!ptr || !dst) {
        return;
    }

    uint8_t val[5];
    for (uint8_t i = 0; i < 5; i++) {
        val[i] = pcf_parse_8(src,format);
        src += 1;
    }

    /* data is offset for some reason */
    dst->bearing_left  = val[0]-0x80;
    dst->bearing_right = val[1]-0x80;
    dst->width         = val[2]-0x80;
    dst->ascent        = val[3]-0x80;
    dst->descent       = val[4]-0x80;
}

void pcf_native_parse_accelerator_table(void *ptr, pcf_native_accelerator_table_t *dst) {
    uint8_t *src = (uint8_t*)ptr;
    if (!ptr || !dst) {
        return;
    }

    dst->format = *(int32_t*)src;
    src += 4;

    uint8_t val8[7];
    for (uint8_t i = 0; i < 7; i++) {
        val8[i] = pcf_parse_8(src,dst->format);
        src += 1;
    }

    src += 1; /* padding */

    int32_t val32[3];
    for (uint8_t i = 0; i < 3; i++) {
        val32[i] = pcf_parse_32(src,dst->format);
        src += 4;
    }

    dst->no_overlap       = val8[0];
    dst->constant_metrics = val8[1];
    dst->font_terminal    = val8[2];
    dst->constant_width   = val8[3];
    dst->ink_inside       = val8[4];
    dst->ink_metrics      = val8[5];
    dst->draw_direction   = val8[6];
    dst->font_ascent      = val32[0];
    dst->font_descent     = val32[1];
    dst->max_overlap      = val32[2];
    return;
}

void pcf_native_parse_metrics_table(void *ptr, pcf_native_metrics_table_t *dst) {
    uint8_t *src = (uint8_t*)ptr;
    if (!ptr || !dst) {
        return;
    }

    dst->format = *(int32_t*)src;
    src += 4;

    if (PCF_CHECK_FORMAT(dst->format) == PCF_FORMAT_COMPRESSED_METRICS) {
        dst->metrics_count = (int32_t)pcf_parse_16(src,dst->format);
    }
    else {
        dst->metrics_count = pcf_parse_32(src,dst->format);
    }
    return;
}

void pcf_native_parse_bitmap_table(void *ptr, pcf_native_bitmap_table_t *dst) {
    uint8_t *src = (uint8_t*)ptr;
    if (!ptr || !dst) {
        return;
    }

    dst->format = *(int32_t*)src;
    src += 4;

    dst->glyph_count = pcf_parse_32(src,dst->format);
    return;
}

void pcf_native_parse_encoding_table(void *ptr, pcf_native_encoding_table_t *dst) {
    uint8_t *src = (uint8_t*)ptr;
    if (!ptr || !dst) {
        return;
    }

    dst->format = *(int32_t*)src;
    src += 4;

    int16_t val[5];
    for (uint8_t i = 0; i < 5; i++) {
        val[i] = pcf_parse_16(src,dst->format);
        src += 2;
    }

    dst->byte2_min    = val[0];
    dst->byte2_max    = val[1];
    dst->byte1_min    = val[2];
    dst->byte1_max    = val[3];
    dst->default_char = val[4];
    return;
}

void pcf_native_parse_swidths_table(void *ptr, pcf_native_swidths_table_t *dst) {
    uint8_t *src = (uint8_t*)ptr;
    if (!ptr || !dst) {
        return;
    }

    dst->format = *(int32_t*)src;
    src += 4;

    dst->glyph_count = pcf_parse_32(src,dst->format);
    return;
}

void pcf_native_parse_glyph_names_table(void *ptr, pcf_native_glyph_names_table_t *dst) {
    uint8_t *src = (uint8_t*)ptr;
    if (!ptr || !dst) {
        return;
    }

    dst->format = *(int32_t*)src;
    src += 4;

    dst->glyph_count = pcf_parse_32(src,dst->format);
    return;
}