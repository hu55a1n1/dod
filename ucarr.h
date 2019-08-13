#ifndef uCUTILS_UCARR_H
#define uCUTILS_UCARR_H

#define ucarr_t(t, l) struct { \
  t data[l]; \
  size_t len; \
}
#define ucarr_t_init(l, ...) {.data = {__VA_ARGS__}, .len = l}
#define ucarr_t_default() ucarr_t_init(0, 0)
#define ucarr_t_decl(arr, t, l) ucarr_t(t, l) arr = ucarr_t_default()
#define ucarr_t_decl_ptr(arr, t, l) \
  ucarr_t(t, l) *arr = memcpy(malloc(sizeof(*arr)), \
  &(ucarr_t(t, l))ucarr_t_default(), sizeof(*arr))
#define ucarr_t_decl_array(arr, t, ...) \
  ucarr_t(t, sizeof((t[]){__VA_ARGS__}) / sizeof(t)) arr = ucarr_t_init(sizeof((t[]){__VA_ARGS__}) / sizeof(t), __VA_ARGS__)
#define ucarr_t_decl_ptr_array(arr, t, ...) \
  ucarr_t(t, sizeof((t[]){__VA_ARGS__}) / sizeof(t)) *arr = memcpy(malloc(sizeof(*arr)), \
  &(ucarr_t(t, sizeof((t[]){__VA_ARGS__}) / sizeof(t)))ucarr_t_init(sizeof((t[]){__VA_ARGS__}) / sizeof(t), __VA_ARGS__), sizeof(*arr))

// Element access
#define ucarr_at(arr, pos) ((pos >= (arr).len) ? NULL: ((arr).data + pos))
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

#endif //uCUTILS_UCARR_H
