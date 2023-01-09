#include "byte_buffer.h"
#include <stdlib.h>
#include <string.h>
#include <limits.h>

typedef struct byte_buffer_t
{
    byte_buffer base;
    char *ptr;
    int len;
} byte_buffer_t;

static void byte_buffer_t_dealloc(byte_buffer_t *obj);
static void byte_buffer_t_init(byte_buffer_t *obj);
static void byte_buffer_t_deinit(byte_buffer_t *obj);
static void byte_buffer_cat(struct byte_buffer *obj, const char *src, const int len);
static const char *byte_buffer_ptr(struct byte_buffer *obj);
static int byte_buffer_len(struct byte_buffer *obj);

static void byte_buffer_t_init(byte_buffer_t *p)
{
    object_init(&p->base.base);
    p->base.base.dealloc = (object_dealloc) byte_buffer_t_dealloc;
    p->base.cat = byte_buffer_cat;
    p->base.ptr = byte_buffer_ptr;
    p->base.len = byte_buffer_len;
    p->ptr = malloc(1);
    p->len = 0;
}

static void byte_buffer_t_deinit(byte_buffer_t *obj)
{
    if (obj->ptr) {
        free(obj->ptr);
        obj->ptr = NULL;
    }
}

byte_buffer *byte_buffer_create()
{
    byte_buffer_t *p = (byte_buffer_t *)malloc(sizeof(byte_buffer_t));
    byte_buffer_t_init(p);
    return &p->base;
}

static void byte_buffer_t_dealloc(byte_buffer_t *obj)
{
    byte_buffer_t_deinit(obj);
    free(obj);
}

static void byte_buffer_cat(struct byte_buffer *obj, const char *src, const int len)
{
    byte_buffer_t *p = (byte_buffer_t *)obj;
    if (len == 0) return;

    char *ptr = realloc(p->ptr, p->len + len);
    if (ptr) {
        p->ptr = ptr;
        memcpy(p->ptr + p->len, src, len);
        p->len += len;
    }
}

static const char *byte_buffer_ptr(struct byte_buffer *obj)
{
    byte_buffer_t *p = (byte_buffer_t *)obj;
    return p->ptr;
}

static int byte_buffer_len(struct byte_buffer *obj)
{
    byte_buffer_t *p = (byte_buffer_t *)obj;
    return p->len;
}