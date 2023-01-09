#include "object.h"
#include <stdlib.h>

static void retain(object *obj);
static void release(object *obj);

void object_init(object *obj)
{
    obj->cnt = 1;
    obj->dealloc = NULL;
    obj->retain = retain;
    obj->release = release;
}

static void retain(object *obj)
{
    __sync_fetch_and_add(&obj->cnt, 1);
}

static void release(object *obj)
{
    int c = __sync_fetch_and_sub(&obj->cnt, 1);
    if (c == 1) {
        if (obj->dealloc) obj->dealloc((void *)obj);
    }
}