#ifndef __byte_buffer_h
#define __byte_buffer_h

#ifdef __cplusplus
extern "C" {
#endif

#include "object.h"

typedef struct byte_buffer
{
    object base;
    void(*cat)(struct byte_buffer *obj, const char *src, const int len);
    const char *(*ptr)(struct byte_buffer *obj);
    int(*len)(struct byte_buffer *obj);
} byte_buffer;

byte_buffer *byte_buffer_create();

#ifdef __cplusplus
}
#endif

#endif