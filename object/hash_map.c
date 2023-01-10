#include "hash_map.h"
#include <stdlib.h>
#include <string.h>
#include "array.h"
#include "byte_buffer.h"

#define KEY_INDEX 0
#define OBJ_INDEX 1

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
static void hash_map_erase(struct hash_map *p, const void *key, const int key_len);
static int hash_map_size(struct hash_map *p);

static int __hash(const char *key, const int key_len, const int capacity)
{
    unsigned long hash = 5381;
    for (int i = 0; i < key_len; ++i) {
        hash = ((hash << 5) + hash) + key[i];
    }
    return hash % capacity;
}

void hash_map_bucket_init(hash_map_bucket *p)
{
    object_init(&p->base.base);
    p->base.base.dealloc = (object_dealloc) hash_map_bucket_dealloc;
    p->base.insert = hash_map_insert;
    p->base.find = hash_map_find;
    p->base.erase = hash_map_erase;
    p->base.size = hash_map_size;
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
    /*calculate new buckets size*/
    int size = p->buckets->size(p->buckets) * 2;
    /*prepare new buckets*/
    for (int i = 0; i < size; ++i) {
        array *b = array_create();
        buckets_new->push_back(buckets_new, &b->base);
        RELEASE(&b->base);
    }
    /* move objects */
    for (int i = p->buckets->size(p->buckets) - 1; i >= 0; i--) {
        array *b = (array *)p->buckets->get(p->buckets, i);
        for (int j = b->size(b) - 1; j >= 0; j--) {
            /*get key - value*/
            array *c = (array *)b->get(b, j);
            /*calculate new hash index*/
            byte_buffer *k = (byte_buffer *)c->get(c, KEY_INDEX);
            int idx = __hash(k->ptr(k), k->len(k), size);
            /*add to new buckets*/
            array *slot = (array *)buckets_new->get(buckets_new, idx);
            slot->push_back(slot, &c->base);
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
    size = p->buckets->size(p->buckets);
    /*create key*/
    byte_buffer *k = byte_buffer_create();
    k->cat(k, key, key_len);
    /*create key value*/
    array *c = array_create();
    c->push_back(c, &k->base);
    c->push_back(c, obj);
    RELEASE(&k->base);
    /*calculate slot index*/
    int idx = __hash(k->ptr(k), k->len(k), size);
    /*add to buckets*/
    array *slot = (array *)p->buckets->get(p->buckets, idx);
    slot->push_back(slot, &c->base);
    RELEASE(&c->base);
    p->total++;
}

static object *hash_map_find(struct hash_map *m, const void *key, const int key_len)
{
    hash_map_bucket *p = (hash_map_bucket *)m;
    /*calculate idx*/
    int idx = __hash((const char *)key, key_len, p->buckets->size(p->buckets));
    array *slot = (array *)p->buckets->get(p->buckets, idx);
    for (int i = slot->size(slot) - 1; i >= 0; i--) {
        array *c = (array *)slot->get(slot, i);
        byte_buffer *k = (byte_buffer *)c->get(c, KEY_INDEX);
        if (k->len(k) == key_len && memcmp(k->ptr(k), key, key_len) == 0) return c->get(c, OBJ_INDEX);
    }
    return NULL;
}

static void hash_map_erase(struct hash_map *m, const void *key, const int key_len)
{
    hash_map_bucket *p = (hash_map_bucket *)m;
    /*calculate idx*/
    int idx = __hash((const char *)key, key_len, p->buckets->size(p->buckets));
    array *slot = (array *)p->buckets->get(p->buckets, idx);
    for (int i = slot->size(slot) - 1; i >= 0; i--) {
        array *c = (array *)slot->get(slot, i);
        byte_buffer *k = (byte_buffer *)c->get(c, KEY_INDEX);
        if (k->len(k) == key_len && memcmp(k->ptr(k), key, key_len) == 0) {
            slot->remove_index(slot, i);
            p->total--;
            break;
        }
    }
}

static int hash_map_size(struct hash_map *m)
{
    hash_map_bucket *p = (hash_map_bucket *)m;
    return p->total;
}