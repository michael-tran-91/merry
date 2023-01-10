#include "string_buffer.h"
#include <stdlib.h>
#include <string.h>
#include <limits.h>

typedef struct string_buffer_t
{
    string_buffer base;
    char *ptr;
    int len;
} string_buffer_t;

static void string_buffer_t_init(string_buffer_t *obj);
static void string_buffer_t_deinit(string_buffer_t *obj);
static void string_buffer_t_dealloc(string_buffer_t *obj);
static void string_buffer_ncat(string_buffer *obj, const char *str, const int len);
static void string_buffer_cat(string_buffer *obj, const char *str);
static void string_buffer_cat_string_buffer(string_buffer *obj, const string_buffer *src);
static const char *string_buffer_ptr(const string_buffer *obj);
static int string_buffer_len(const string_buffer *obj);

void string_buffer_t_init(string_buffer_t *obj)
{
    object_init(&obj->base.base);
    obj->base.base.dealloc = (object_dealloc)string_buffer_t_dealloc;
    obj->base.ncat = string_buffer_ncat;
    obj->base.cat = string_buffer_cat;
    obj->base.cat_string_buffer = string_buffer_cat_string_buffer;
    obj->base.ptr = string_buffer_ptr;
    obj->base.len = string_buffer_len;
    obj->ptr = malloc(1);
    obj->len = 0;
}

void string_buffer_t_deinit(string_buffer_t *obj)
{
    if (obj->ptr) {
        free(obj->ptr);
        obj->ptr = NULL;
    }
    obj->len = 0;
}

string_buffer *string_buffer_create()
{
    string_buffer_t *obj = (string_buffer_t *)malloc(sizeof(string_buffer_t));
    string_buffer_t_init(obj);
    return &obj->base;
}

static void string_buffer_t_dealloc(string_buffer_t *obj)
{
    string_buffer_t_deinit(obj);
    free(obj);
}

static void string_buffer_ncat(string_buffer *p, const char *str, const int max)
{
    string_buffer_t *obj = (string_buffer_t *)p;
    if (max == 0) return;
    int len = strlen(str);
    if (len > max) len = max;

    char *ptr = realloc(obj->ptr, obj->len + len + 1);
    if (ptr) {
        obj->ptr = ptr;
        memcpy(obj->ptr + obj->len, str, len);
        obj->len += len;
        obj->ptr[obj->len] = '\0';
    }
}

static void string_buffer_cat(string_buffer *obj, const char *str)
{
    string_buffer_ncat(obj, str, INT_MAX);
}

static void string_buffer_cat_string_buffer(string_buffer *obj, const string_buffer *src)
{
    string_buffer_ncat(obj, src->ptr(src), src->len(src));
}

static const char *string_buffer_ptr(const string_buffer *p)
{
    string_buffer_t *obj = (string_buffer_t *)p;
    return obj->ptr;
}

static int string_buffer_len(const string_buffer *p)
{
    string_buffer_t *obj = (string_buffer_t *)p;
    return obj->len;
}