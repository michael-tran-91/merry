#ifndef __hash_map_h
#define __hash_map_h

#ifdef __cplusplus
extern "C" {
#endif

#include "object.h"

typedef struct hash_map
{
    object base;
    void    (*insert)(struct hash_map *p, const void *key, const int key_len, object *obj);
    object *(*find)(struct hash_map *p, const void *key, const int key_len);
    void    (*erase)(struct hash_map *p, const void *key, const int key_len);
    int     (*size)(struct hash_map *p);
} hash_map;

hash_map *hash_map_create();

#ifdef __cplusplus
}
#endif

#endif