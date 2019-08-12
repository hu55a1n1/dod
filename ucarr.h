#ifndef uCUTILS_UCARR_H
#define uCUTILS_UCARR_H

#define ucarr_t(t, l) struct { \
  t data[l]; \
  size_t len; \
}
#define ucarr_t_default() {.data = {0}, .len = 0}
#define ucarr_t_decl(arr, t, l) ucarr_t(t, l) arr = ucarr_t_default()
#define ucarr_t_decl_ptr(arr, t, l) \
  ucarr_t(t, l) *arr = memcpy(malloc(sizeof(*arr)), \
  &(ucarr_t(t, l))ucarr_t_default(), sizeof(*arr))

// Element access
#define ucarr_at(arr, pos) ucarr_at_(arr, (arr).len, pos)
#define ucarr_front(arr) (arr)[0]
#define ucarr_back(arr) (arr)[(arr).len - 1]
#define ucarr_data(arr) (arr).data

// Capacity
#define ucarr_empty(arr) ((arr).len == 0)
#define ucarr_size(arr) (arr).len
#define ucarr_max_size(arr) (sizeof((arr).data)/sizeof(*(arr).data))

// Operations
#define ucarr_fill(arr, val) do { \
  size_t msz = ucarr_max_size(arr); \
  for (size_t _i_ = 0; _i_ <= msz; ++_i_) \
    (arr)[_i_] = val; \
} while (0)
#define ucarr_swap(arr1, arr2) do { \
  size_t _sz_ = sizeof(arr1); \
  char _arr_[_sz_]; \
  memcpy((_arr_), &(arr1), _sz_); \
  memcpy(&(arr1), &(arr2), sizeof(arr2)); \
  memcpy(&(arr2), (_arr_), _sz_); \
} while (0)

// Internal funcs
static inline void *ucarr_at_(unsigned char *arr, size_t l, size_t pos) {
  if (pos >= l)
    return NULL;
  return (arr + (pos * sizeof(*arr)));
}

#endif //uCUTILS_UCARR_H
