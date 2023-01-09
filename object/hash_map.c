#include "hash_map.h"
#include <stdlib.h>
#include <string.h>
#include "array.h"
#include "byte_buffer.h"

typedef struct hash_map_bucket
{
    hash_map base;
    array *buckets;
    int total;
} hash_map_bucket;

static void hash_map_bucket_dealloc(hash_map_bucket *p);
static void hash_map_bucket_init(hash_map_bucket *p);
static void hash_map_bucket_deinit(hash_map_bucket *p);
static void hash_map_insert(struct hash_map *p, const void *key, const int key_len, object *obj);
static object *hash_map_find(struct hash_map *p, const void *key, const int key_len);

void hash_map_bucket_init(hash_map_bucket *p)
{
    object_init(&p->base.base);
    p->base.base.dealloc = (object_dealloc) hash_map_bucket_dealloc;
    p->base.insert = hash_map_insert;
    p->base.find = hash_map_find;
    p->total = 0;
    p->buckets = array_create();
    for (int i = 0; i < 2; ++i) {
        array *b = array_create();
        p->buckets->push_back(p->buckets, &b->base);
        RELEASE(&b->base);
    }
}

void hash_map_bucket_deinit(hash_map_bucket *p)
{
    RELEASE(&p->buckets->base);
}

hash_map *hash_map_create()
{
    hash_map_bucket *p = (hash_map_bucket *) malloc(sizeof(hash_map_bucket));
    hash_map_bucket_init(p);
    return &p->base;
}

static void hash_map_bucket_dealloc(hash_map_bucket *p)
{
    hash_map_bucket_deinit(p);
    free(p);
}

static void hash_map_resize(struct hash_map *m)
{
    hash_map_bucket *p = (hash_map_bucket *)m;
    /* create new buckets */
    array *buckets_new = array_create();
    int size = p->buckets->size(p->buckets) * 2;
    for (int i = 0; i < size; ++i) {
        array *b = array_create();
        buckets_new->push_back(buckets_new, &b->base);
        RELEASE(&b->base);
    }
    /* move objects */
    for (int i = p->buckets->size(p->buckets) - 1; i >= 0; i--) {
        array *b = (array *)p->buckets->get(p->buckets, i);
        for (int j = b->size(b) - 1; j >= 0; j--) {

        }
    }
    /* assign new buckets */
    RELEASE(&p->buckets->base);
    p->buckets = buckets_new;
}

static void hash_map_insert(struct hash_map *m, const void *key, const int key_len, object *obj)
{
    hash_map_bucket *p = (hash_map_bucket *)m;
    int size = p->buckets->size(p->buckets);
    if (p->total + 1 >= (size / 2)) {
        hash_map_resize(m);
    }
}

static object *hash_map_find(struct hash_map *m, const void *key, const int key_len)
{
    hash_map_bucket *p = (hash_map_bucket *)m;
}