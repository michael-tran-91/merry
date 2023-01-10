#include "array.h"
#include <stdlib.h>
#include <string.h>

typedef struct array_t
{
    array base;
    object **ptr;
    int len;
} array_t;

static void array_t_init(array_t *p);
static void array_t_deinit(array_t *p);
static void array_t_dealloc(array_t *p);
static void array_push_back(array *p, object *obj);
static object *array_get(array *p, int index);
static void array_remove_index(array *p, int index);
static int array_size(array *p);

static void array_t_init(array_t *p)
{
    object_init(&p->base.base);
    p->base.base.dealloc = (object_dealloc)array_t_dealloc;
    p->base.push_back = array_push_back;
    p->base.get = array_get;
    p->base.remove_index = array_remove_index;
    p->base.size = array_size;
    p->ptr = NULL;
    p->len = 0;
}

static void array_t_deinit(array_t *p)
{
    for (int i = 0; i < p->len; ++i) {
        RELEASE(p->ptr[i]);
    }
    if (p->ptr) {
        free(p->ptr);
        p->ptr = NULL;
    }
    p->len = 0;
}

array *array_create()
{
    array_t *p = (array_t *)malloc(sizeof(array_t));
    array_t_init(p);
    return &p->base;
}

static void array_t_dealloc(array_t *p)
{
    array_t_deinit(p);
    free(p);
}

static void array_push_back(array *a, object *obj)
{
    array_t *p = (array_t *)a;
    object **ptr = realloc(p->ptr, sizeof(object *) * (p->len + 1));
    if (ptr) {
        p->ptr = ptr;
        p->ptr[p->len] = obj;
        p->len++;
        RETAIN(obj);
    }
}

static object *array_get(array *a, int index)
{
    array_t *p = (array_t *)a;
    if (index < 0 || index >= p->len) return NULL;
    return p->ptr[index];
}

static void array_remove_index(array *a, int index)
{
    array_t *p = (array_t *)a;
    if (index < 0 || index >= p->len) return;

    object *o = p->ptr[index];
    p->ptr[index] = p->ptr[p->len - 1];
    p->len--;
    RELEASE(o);
}

static int array_size(array *a)
{
    array_t *p = (array_t *)a;
    return p->len;
}