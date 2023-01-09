#ifndef __array_h
#define __array_h

#ifdef __cplusplus
extern "C" {
#endif

#include "object.h"

typedef struct array
{
    object base;
    void    (*push_back)(struct array *p, object *obj);
    object* (*get)(struct array *p, int index);
    void    (*remove_index)(struct array *p, int index);
    int     (*size)(struct array *p);
} array;

array *array_create();

#ifdef __cplusplus
}
#endif

#endif