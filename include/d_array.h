#ifndef D_ARRAY_HEADER_H
#define D_ARRAY_HEADER_H

#include <stdlib.h>

#define DARRAY_IMPL(word) DARRAY_COMB1(DARRAY_PREFIX, word)
#define DARRAY_COMB1(prefix, word) DARRAY_COMB2(prefix, word)
#define DARRAY_COMB2(prefix, word) prefix##word

#endif

#ifndef DARRAY_T
#error "DARRAY_T is not defined"
#endif

#ifndef DARRAY_NAME
#define DARRAY_NAME DARRAY_COMB1(DARRAY_COMB1(darray, _), DARRAY_T)
#endif

#ifndef DARRAY_PREFIX
#define DARRAY_PREFIX DARRAY_COMB1(DARRAY_NAME, _)
#endif

#ifndef DARRAY_LINKAGE
#define DARRAY_LINKAGE static inline
#endif

typedef struct DARRAY_NAME DARRAY_NAME;

struct DARRAY_NAME {
  DARRAY_T *items;
  size_t count;
  size_t capacity;
};

#define DARRAY_push DARRAY_IMPL(push)
#define DARRAY_pop DARRAY_IMPL(pop)

#ifdef DARRAY_DECLS_ONLY

DARRAY_LINKAGE
void DARRAY_push(DARRAY_NAME *array, DARRAY_T item);
DARRAY_LINKAGE
DARRAY_T *DARRAY_pop(DARRAY_NAME *array);

#else

DARRAY_LINKAGE
void DARRAY_push(DARRAY_NAME *array, DARRAY_T item) {

  if (array->count >= array->capacity) {
    size_t old_cap = array->capacity;
    size_t new_cap = old_cap ? old_cap * 2 : 4;
    size_t new_size = new_cap * sizeof(DARRAY_T);
    array->items = realloc(array->items, new_size);
    if (!array->items) {
      return;
    }
    array->capacity = new_cap;
  }

  array->items[array->count++] = item;
}

DARRAY_LINKAGE
DARRAY_T *DARRAY_pop(DARRAY_NAME *array) {
  if (array->count == 0) {
    return NULL;
  }
  return array->items[--array->count];
}

#endif

#undef DARRAY_T
#undef DARRAY_PREFIX
#undef DARRAY_NAME
#undef DARRAY_LINKAGE
#undef DARRAY_push
#ifdef DARRAY_DECLS_ONLY
#undef DARRAY_DECLS_ONLY
#endif