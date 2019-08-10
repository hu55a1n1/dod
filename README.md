# ucutils
Micro C utilities - Small header-only utilities written in C99.

* [ucbs.h](#ucbsh---c-stdbitset-for-c) - C++ std::bitset for C
* [ucbytes.h](#ucbytesh---byte-manipulation-utils) - Byte manipulation utils
* [ucvec.h](#ucvech---c-stdvector-for-c) - C++ std::vector for C

## ucbs.h - C++ std::bitset for C
The API is (almost) complaint with STL's `std::bitset` with a few exceptions (see notes section below).
The implementation tries to stick to the STL implementation unless otherwise specified.


### Usage
For more examples see `test_ucbs.c`.
```c
ucbs_t_decl(bs1, 16);           // Create bitset of length 16
ucbs_init(bs1, 43690U);         // Initialize by value - 43690 == b1010101010101010
ucbs_flipall(bs);               // Flip all bits
assert(ucbs_any(bs) == true);   // Check if any bit is set
assert(ucbs_none(bs) == false); // Check if no bit is set
ucbs_setall(bs);                // Set all bits
assert(ucbs_all(bs) == true);   // Check if all bits are set
```


### Constructors
`std::bitset` provides 3 types of constructors -
* default constructor - The object is initialized with zeros.
For this, the library provides 2 methods, one for stack and the other for heap initialization.
Note: The library relies on the declaration to zero initialize the storage, hence the unorthodox method of creation.
If you do not prefer the `*decl*` macros, then by all means use the manual declaration method.
```c
// Create a bitset of length 16 on the stack
ucbs_t_decl(bs, 16);
// Equivalent manual decl
// ucbs_t(16) bs = {.bytes = {0}, .len = 16 };

// Create a bitset of length 16 on the heap
ucbs_t_decl_ptr(bsptr, 16);
// Equivalent manual decl
// ucbs_t(16) *bsptr = malloc(sizeof(*bsptr));
// bsptr->bytes = {0};
// bsptr->len = 16
/* ... */
free(bsptr);
```

* initialization from integer value - Initializes the object with the bit values of val.
This is the `ucbs_init()` function.
```c
ucbs_t_decl(bs, 16);
ucbs_init(bs, 43690U); // 43690 == b1010101010101010
```

* initialization from string. This is the `ucbs_init_str()` function.
```c
ucbs_t_decl(bs2, 16);
ucbs_init_str(bs2, "1010101010101010");
```


### Bit access
| Function   | Use                      | Equivalent function in ucbs.h | Notes                                                                                                                                                |
|------------|--------------------------|-------------------------------|------------------------------------------------------------------------------------------------------------------------------------------------------|
| operator[] | Access bit               | Not implemented               | This functions uses C++ features (references and operator overloading) that are non-existent in C. Use ucbs_test()/ucbs_set()/ucbs_reset() instead.  |
| count      | Count bits set           | `ucbs_count(bs)`              |                                                                                                                                                      |
| size       | Return size              | `ucbs_size(bs)`               |                                                                                                                                                      |
| test       | Return bit value         | `ucbs_test(bs, pos)`          |                                                                                                                                                      |
| any        | Test if any bit is set   | `ucbs_any(bs)`                |                                                                                                                                                      |
| none       | Test if no bit is set    | `ucbs_none(bs)`               | Opposite of `ucbs_any(bs)`.                                                                                                                          |
| all        | Test if all bits are set | `ucbs_all(bs)`                |                                                                                                                                                      |


### Bit operations
| Function | Use        | Equivalent function in ucbs.h                  | Notes                                                                                                                                                                                                            |
|----------|------------|------------------------------------------------|------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| set      | Set bits   | `ucbs_set(bs, pos)`/`ucbs_setall(bs)`          | `bitset& set() noexcept;` is equivalent to `ucbs_setall(bs)`. Note also that `ucbs_set(bs, pos)` has no `val` default arg unlike its c++ counterpart. These functions return void unlike their C++ counterparts. |
| reset    | Reset bits | `ucbs_reset(bs, pos)`/`ucbs_resetall(bs, pos)` | `bitset& reset() noexcept;` is equivalent to `ucbs_resetall(bs)`. These functions return void unlike their C++ counterparts.                                                                                     |
| flip     | Flip bits  | `ucbs_flip(bs, pos)`/`ucbs_flipall(bs)`        | `bitset& flip() noexcept;` is equivalent to `ucbs_flipall(bs)`. These functions return void unlike their C++ counterparts.                                                                                       |


### Bitset operations
| Function  | Use                              | Equivalent function in ucbs.h | Notes                                                                                                                  |
|-----------|----------------------------------|-------------------------------|------------------------------------------------------------------------------------------------------------------------|
| to_string | Convert to string                | `ucbs_to_string(bs)`          | Returns a `char *` that must be freed after usage.                                                                     |
| to_ulong  | Convert to unsigned long integer | `ucbs_to_ulong(bs)`           | Internally creates a temporary `unsigned long` variable and reads to it using `memcpy` to avoid alignment issues.      |
| to_ullong | Convert to unsigned long long    | `ucbs_to_ullong(bs, pos)`     | Internally creates a temporary `unsigned long long` variable and reads to it using `memcpy` to avoid alignment issues. |


**Note:** Direct use of bitwise operators is unsupported.


### Internals
This implementation is inspired by `llvm-libc++`'s `std::bitset`.
The backend storage for bits comprises of an array of `size_t`s. This can be changed by modifying the `ucbs_storage_t` typedef.
Most functions are macros that delegate to other functions, this allows for generic functions that can handle bitsets of all sizes.
It also provides a convenient API where you don't have to always pass the length of the bitset.



## ucbytes.h - Byte manipulation utils
This is a byte array that can grow in a logarithmic manner.
It is implemented as the backend for the vector library and is not intended to be used in isolation.
If you choose to use it as a standalone library make sure you understand *alignment* and its implications.



## ucvec.h - C++ std::vector for C
The API is (almost) complaint with STL's `std::vector` with a few exceptions (see notes section below).
The implementation tries to stick to the STL implementation unless otherwise specified.


### Usage
For more examples see `test_ucvec.c`.
```c
ucvec_t *v = ucvec_new(int,10);                                                     // Create vector of ints and reserve space for 10 ints
for (int i = 0; i < 5; ++i)                                                         // Push back 5 ints
  ucvec_push_back(v, &i);                                                           // {0, 1, 2, 3, 4}
assert(*ucvec_at(v, 2) == 2);                                                       // Check element at pos 2 is 2
assert(!memcmp(ucvec_data(v), (int[]) {0, 1, 2, 3, 4}, sizeof(int) * 5));           // {0, 1, 2, 3, 4}
ucvec_pop_back(v);                                                                  // Pop back
assert(!memcmp(ucvec_data(v), (int[]) {0, 1, 2, 3}, sizeof(int) * 4));              // {0, 1, 2, 3}
ucvec_erase(v, 0);                                                                  // Erase at pos 0
assert(!memcmp(ucvec_data(v), (int[]) {1, 2, 3}, sizeof(int) * 3));                 // {1, 2, 3}
int vals[] = {501, 502, 503};                                                       // Insert range using array
ucvec_insert_range(v, ucvec_front(v), vals, vals + 3);                              // at front
assert(!memcmp(ucvec_data(v), (int[]) {501, 502, 503, 1, 2, 3}, sizeof(int) * 3));  // {501, 502, 503, 1, 2, 3}
ucvec_free(v);                                                                      // free after use
```
