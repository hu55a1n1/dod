# dod
Tools to facilitate data-oriented design in C99.

If you are new to data-oriented design, [this](https://github.com/dbartolini/data-oriented-design) is a good place to start.

This library provides the following utilities -
* [dodbytes.h](#dodbytesh---byte-manipulation-utils) - Byte manipulation utils
* [dodvec.h](#dodvech---a-feature-complete-generic-vector-implementation) - A feature-complete generic vector implementation
* [dodbs.h](#dodbsh---a-feature-complete-bitset-implementation) - A feature-complete bitset implementation
* [dodarr.h](#dodarrh---generic-array-with-value-semantics) - Generic array with value semantics and optional bounds checking



## dodbytes.h - Byte manipulation utils
This is a byte array that can grow in a logarithmic manner.
It is implemented as the backend for the vector library and is not intended to be used in isolation.
If you choose to use it as a standalone library make sure you understand *alignment* and its implications.



## dodvec.h - A feature-complete generic vector implementation
The API is (almost) complaint with C++ STL's `std::vector` with a few exceptions (see notes section below).
The implementation tries to stick to the STL implementation unless otherwise specified.


### Usage
For more examples see `test_dodvec.c`.
```c
dodvec_t *v = dodvec_new(int, 10);                          // Create vector of ints and reserve space for 10 ints
for (int i = 0; i < 5; ++i)                                 // Push back 5 ints
  dodvec_push_back(v, &i);                                  // >> {0, 1, 2, 3, 4}
assert(*dodvec_at(v, 2) == 2);                              // Check element at pos 2 is 2
dodvec_pop_back(v);                                         // Pop back >> {0, 1, 2, 3}
dodvec_erase(v, 0);                                         // Erase at pos 0 >> {1, 2, 3}
int vals[] = {501, 502, 503};                               // Insert range using array
dodvec_insert_range(v, dodvec_front(v), vals, vals + 3);    // at front >> {501, 502, 503, 1, 2, 3}
dodvec_free(v);                                             // free after use
```



## dodbs.h - A feature-complete bitset implementation
The API is (almost) complaint with C++ STL's `std::bitset` with a few exceptions (see notes section below).
The implementation tries to stick to the STL implementation unless otherwise specified.


### Usage
For more examples see `test_dodbs.c`.
```c
dodbs_t_decl(bs1, 16);              // Create bitset of length 16
dodbs_init(bs1, 43690U);            // Initialize by value - 43690 == b1010101010101010
dodbs_flipall(bs);                  // Flip all bits
assert(dodbs_any(bs) == true);      // Check if any bit is set
assert(dodbs_none(bs) == false);    // Check if no bit is set
dodbs_setall(bs);                   // Set all bits
assert(dodbs_all(bs) == true);      // Check if all bits are set
```


### Constructors
`std::bitset` provides 3 types of constructors -
* default constructor - The object is initialized with zeros.
For this, the library provides 2 methods, one for stack and the other for heap initialization.
Note: The library relies on the declaration to zero initialize the storage, hence the unorthodox method of creation.
If you do not prefer the `*decl*` macros, then by all means use the manual declaration method.
```c
// Create a bitset of length 16 on the stack
dodbs_t_decl(bs, 16);
// Equivalent manual decl
// dodbs_t(16) bs = {.bytes = {0}, .len = 16 };

// Create a bitset of length 16 on the heap
dodbs_t_decl_ptr(bsptr, 16);
// Equivalent manual decl
// dodbs_t(16) *bsptr = malloc(sizeof(*bsptr));
// bsptr->bytes = {0};
// bsptr->len = 16
/* ... */
free(bsptr);
```

* initialization from integer value - Initializes the object with the bit values of val.
This is the `dodbs_init()` function.
```c
dodbs_t_decl(bs, 16);
dodbs_init(bs, 43690U); // 43690 == b1010101010101010
```

* initialization from string. This is the `dodbs_init_str()` function.
```c
dodbs_t_decl(bs2, 16);
dodbs_init_str(bs2, "1010101010101010");
```


### Bit access
| Function   | Use                      | Equivalent function in dodbs.h | Notes                                                                                                                                                |
|------------|--------------------------|-------------------------------|------------------------------------------------------------------------------------------------------------------------------------------------------|
| operator[] | Access bit               | Not implemented               | This functions uses C++ features (references and operator overloading) that are non-existent in C. Use dodbs_test()/dodbs_set()/dodbs_reset() instead.  |
| count      | Count bits set           | `dodbs_count(bs)`              |                                                                                                                                                      |
| size       | Return size              | `dodbs_size(bs)`               |                                                                                                                                                      |
| test       | Return bit value         | `dodbs_test(bs, pos)`          |                                                                                                                                                      |
| any        | Test if any bit is set   | `dodbs_any(bs)`                |                                                                                                                                                      |
| none       | Test if no bit is set    | `dodbs_none(bs)`               | Opposite of `dodbs_any(bs)`.                                                                                                                          |
| all        | Test if all bits are set | `dodbs_all(bs)`                |                                                                                                                                                      |


### Bit operations
| Function | Use        | Equivalent function in dodbs.h                  | Notes                                                                                                                                                                                                            |
|----------|------------|------------------------------------------------|------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| set      | Set bits   | `dodbs_set(bs, pos)`/`dodbs_setall(bs)`          | `bitset& set() noexcept;` is equivalent to `dodbs_setall(bs)`. Note also that `dodbs_set(bs, pos)` has no `val` default arg unlike its c++ counterpart. These functions return void unlike their C++ counterparts. |
| reset    | Reset bits | `dodbs_reset(bs, pos)`/`dodbs_resetall(bs, pos)` | `bitset& reset() noexcept;` is equivalent to `dodbs_resetall(bs)`. These functions return void unlike their C++ counterparts.                                                                                     |
| flip     | Flip bits  | `dodbs_flip(bs, pos)`/`dodbs_flipall(bs)`        | `bitset& flip() noexcept;` is equivalent to `dodbs_flipall(bs)`. These functions return void unlike their C++ counterparts.                                                                                       |


### Bitset operations
| Function  | Use                              | Equivalent function in dodbs.h | Notes                                                                                                                  |
|-----------|----------------------------------|-------------------------------|------------------------------------------------------------------------------------------------------------------------|
| to_string | Convert to string                | `dodbs_to_string(bs)`          | Returns a `char *` that must be freed after usage.                                                                     |
| to_ulong  | Convert to unsigned long integer | `dodbs_to_ulong(bs)`           | Internally creates a temporary `unsigned long` variable and reads to it using `memcpy` to avoid alignment issues.      |
| to_ullong | Convert to unsigned long long    | `dodbs_to_ullong(bs, pos)`     | Internally creates a temporary `unsigned long long` variable and reads to it using `memcpy` to avoid alignment issues. |


**Note:** Direct use of bitwise operators is unsupported.


### Internals
This implementation is inspired by `llvm-libc++`'s `std::bitset`.
The backend storage for bits comprises of an array of `size_t`s. This can be changed by modifying the `dodbs_storage_t` typedef.
Most functions are macros that delegate to other functions, this allows for generic functions that can handle bitsets of all sizes.
It also provides a convenient API where you don't have to always pass the length of the bitset.



## dodarr.h - Generic array with value semantics

