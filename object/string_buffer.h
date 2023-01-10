#ifndef __string_buffer_h
#define __string_buffer_h

#ifdef __cplusplus
extern "C" {
#endif

#include "object.h"

typedef struct string_buffer
{
    object base;
    void        (*ncat)(struct string_buffer *obj, const char *str, const int len);
    void        (*cat)(struct string_buffer *obj, const char *str);
    void        (*cat_string_buffer)(struct string_buffer *obj, const struct string_buffer *src);
    const char *(*ptr)(const struct string_buffer *obj);
    int         (*len)(const struct string_buffer *obj);
} string_buffer;

string_buffer *string_buffer_create();

#ifdef __cplusplus
}
#endif

#endif