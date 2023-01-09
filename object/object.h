#ifndef __object_h
#define __object_h

#ifdef __cplusplus
extern "C" {
#endif

typedef void(*object_dealloc)(void *);

typedef struct object
{
    volatile int cnt;
    object_dealloc dealloc;
    void(*retain)(struct object *obj);
    void(*release)(struct object *obj);
} object;

void object_init(object *obj);

#define RETAIN(o) (o)->retain(o);
#define RELEASE(o) (o)->release(o);

#ifdef __cplusplus
}
#endif

#endif