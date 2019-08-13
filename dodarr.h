#ifndef DOD_DODARR_H
#define DOD_DODARR_H

#define dodarr_t(t, l) struct { \
  t data[l]; \
  size_t len; \
}
#define dodarr_t_init(l, ...) {.data = {__VA_ARGS__}, .len = l}
#define dodarr_t_default() dodarr_t_init(0, 0)
#define dodarr_t_decl(arr, t, l) dodarr_t(t, l) arr = dodarr_t_default()
#define dodarr_t_decl_ptr(arr, t, l) \
  dodarr_t(t, l) *arr = memcpy(malloc(sizeof(*arr)), \
  &(dodarr_t(t, l))dodarr_t_default(), sizeof(*arr))
#define dodarr_t_decl_array(arr, t, ...) \
  dodarr_t(t, sizeof((t[]){__VA_ARGS__}) / sizeof(t)) arr = dodarr_t_init(sizeof((t[]){__VA_ARGS__}) / sizeof(t), __VA_ARGS__)
#define dodarr_t_decl_ptr_array(arr, t, ...) \
  dodarr_t(t, sizeof((t[]){__VA_ARGS__}) / sizeof(t)) *arr = memcpy(malloc(sizeof(*arr)), \
  &(dodarr_t(t, sizeof((t[]){__VA_ARGS__}) / sizeof(t)))dodarr_t_init(sizeof((t[]){__VA_ARGS__}) / sizeof(t), __VA_ARGS__), sizeof(*arr))

// Element access
#define dodarr_at(arr, pos) ((pos >= (arr).len) ? NULL: ((arr).data + pos))
#define dodarr_front(arr) (arr)[0]
#define dodarr_back(arr) (arr)[(arr).len - 1]
#define dodarr_data(arr) (arr).data

// Capacity
#define dodarr_empty(arr) ((arr).len == 0)
#define dodarr_size(arr) (arr).len
#define dodarr_max_size(arr) (sizeof((arr).data)/sizeof(*(arr).data))

// Operations
#define dodarr_fill(arr, val) do { \
  size_t msz = dodarr_max_size(arr); \
  for (size_t _i_ = 0; _i_ <= msz; ++_i_) \
    (arr)[_i_] = val; \
} while (0)
#define dodarr_swap(arr1, arr2) do { \
  size_t _sz_ = sizeof(arr1); \
  char _arr_[_sz_]; \
  memcpy((_arr_), &(arr1), _sz_); \
  memcpy(&(arr1), &(arr2), sizeof(arr2)); \
  memcpy(&(arr2), (_arr_), _sz_); \
} while (0)

// Non-member functions
#define dodarr_to_array(arr, a, l) do { \
  for (size_t _i_ = 0; _i_ < sizeof(arr) && _i_ < l; ++_i_) \
    (arr).data[_i_] = a[_i_]; \
} while (0)

#endif //DOD_DODARR_H
