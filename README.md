# C99 Generic Vec

A lightweight and easy-to-use vector library for C99, which uses macros to provide a type-safe generic container for storing and manipulating dynamic contiguous blocks of elements.

Key features of this library include:

- 🪶 **Lightweight** as the codebase is only ~300 significant lines of code.
- 📄 **Header-only** so that it's easy to integrate into your existing codebase.
- 🔒 **Type-safe** so that you get type-related compiler errors and warnings.
- 🔢 **Behaves like an array** allowing for:
    - Indexing with the `[]` operator.
    - Passing into functions expecting a C array or pointer.

## Quick Links

- [General Usage](#general-usage)
- [User Configurable Macros](#user-configurable-macros)
- [Quick Library Overview](#quick-library-overview)
- [Documentation](#documentation)

# General Usage <a id="general-usage"/>

You create a vec by declaring a variable of `vec_t(T)` and then initializing it. From that point on all vector operations are aware of the type and you will get the same type safety for values that C would provide in any other context.

```c
// create a vec of ints
vec_t(int) numbers;
vec_init(&numbers);

// simply push values
vec_push(&numbers, 10);
vec_push(&numbers, 20);
vec_push(&numbers, 30);

// deinitialize to free memory
vec_deinit(&numbers);
```
To demonstrate the type safety here is what happens when you push some values of various types.

```c
vec_push(&numbers, (my_struct_t){ /* ... */ });
// error: incompatible types when assigning to type ‘int’ from type ‘my_struct_t’

vec_push(&numbers, "Oops");
// warning: assignment to ‘int’ from ‘char *’ makes integer from pointer without a cast [-Wint-conversion]
```
for reference, this is the same behaviour as an ordinary variable.

```c
int x = (my_struct_t){ /* ... */ };
// error: incompatible types when assigning to type ‘int’ from type ‘my_struct_t’

x = "Oops";
// warning: assignment to ‘int’ from ‘char *’ makes integer from pointer without a cast [-Wint-conversion]
```

You can iterate over the vec by using [vec_len](#vec-len) and an ordinary for loop,

```c
for (size_t i = 0; i < vec_len(&numbers); i++)
{
    printf("%d\n", numbers[i]); // note that indexing works
}
```

Or by using the convenient [vec_for_each](#vec-for-each) macro which gives you a pointer to each element.

```c
vec_for_each(&numbers, num)
{
    printf("%d\n", *num);
}
```

Indexing into the vec works because the variable `numbers` is actually a pointer to the first element of the vec's buffer, just like a regular C array. This also means you can pass the vector into any function that expects a C array without any casting.

```c

void int_array_sort(int array[], size_t array_len);

// ...

int_array_sort(numbers, vec_len(&numbers));
```

# User Configurable Macros <a id="user-configurable-macros"/>

Some macros can be provided by the user to add custom behaviour to the library. These include:

- **VEC_MALLOC(size)** allows the user to provide their own malloc function.
- **VEC_REALLOC(ptr, size)** allows the user to provide their own realloc function.
- **VEC_FREE(ptr)** allows the user to provide their own free function.
- **VEC_PANIC(reason)** allows the user to control the behaviour when an unrecoverable error is encountered. This should always result in program termination and should never be defined to nothing.
- **VEC_ASSERT(condition, reason)** allows the user to provide their own assert function, or to define it as nothing for release builds.

# Quick Library Overview <a id="quick-library-overview"/>

```c
// init / free
void vec_init(vec_t(T) *vec);
void vec_deinit(vec_t(T) *vec);
void vec_deinit_with(vec_t(T) *vec, void (*deinit_fn)(T* value));

// getters
size_t vec_len(const vec_t(T) *vec);
size_t vec_cap(const vec_t(T) *vec);

// add / remove operations
void vec_push(vec_t(T) *vec, T value);
void vec_try_push(vec_t(T) *vec, T value);
T *vec_emplace(vec_t(T) *vec);
T *vec_try_emplace(vec_t(T) *vec);
void vec_reserve(vec_t(T) *vec, size_t capacity);
void vec_try_reserve(vec_t(T) *vec, size_t capacity);
void vec_pop(vec_t(T) *vec);
void vec_pop_with(vec_t(T) *vec, void (*deinit_fn)(T* value));
void vec_clear(vec_t(T) *vec);
void vec_clear_with(vec_t(T) *vec, void (*deinit_fn)(T* value));
void vec_remove_swap_at(vec_t(T) *vec, size_t index);
void vec_remove_swap_at_with(vec_t(T) *vec, size_t index, void (*deinit_fn)(T* value));
void vec_remove_shift_at(vec_t(T) *vec, size_t index);
void vec_remove_shift_at_with(vec_t(T) *vec, size_t index, void (*deinit_fn)(T* value));

// iteration macros
vec_for_each(vec_t(T) *vec, value_ptr);
vec_for_each_index(vec_t(T) *vec, value_ptr, index);

// internals
bool vec__has_header(vec_t(T) *vec);
vec__header_t *vec__get_header(vec_t(T) *vec);
void vec__set_header(vec_t(T) *vec, vec__header_t *header);
void vec__grow_or_fail(vec_t(T) *vec, fail_block);
void vec__reserve_or_fail(vec_t(T) *vec, size_t capacity, fail_block);
```

# Documentation <a id="documentation"/>

- init / deinit
    - [vec_init](#vec-init)
    - [vec_deinit](#vec-deinit)
    - [vec_deinit_with](#vec-deinit-with)
- getters
    - [vec_len](#vec-len)
    - [vec_cap](#vec-cap)
- add / remove operations
    - [vec_push](#vec-push)
    - [vec_try_push](#vec-try-push)
    - [vec_emplace](#vec-emplace)
    - [vec_try_emplace](#vec-try-emplace)
    - [vec_reserve](#vec-reserve)
    - [vec_try_reserve](#vec-try-reserve)
    - [vec_pop](#vec-pop)
    - [vec_pop_with](#vec-pop-with)
    - [vec_clear](#vec-clear)
    - [vec_clear_with](#vec-clear-with)
    - [vec_remove_swap_at](#vec-remove-swap-at)
    - [vec_remove_swap_at_with](#vec-remove-swap-at-with)
    - [vec_remove_shift_at](#vec-remove-shift-at)
    - [vec_remove_shift_at_with](#vec-remove-shift-at-with)
- iteration macros
    - [vec_for_each](#vec-for-each)
    - [vec_for_each_index](#vec-for-each-index)
- internals
    - [vec__has_header](#vec--has-header)
    - [vec__get_header](#vec--get-header)
    - [vec__set_header](#vec--set-header)
    - [vec__grow_or_fail](#vec--grow-or-fail)
    - [vec__reserve_or_fail](#vec--reserve-or-fail)

# vec_init <a id="vec-init"/>

Initialize a vec to an empty state. This does not perform any allocations. After initializing a vec it must be deinitialized with [vec_deinit](#vec-deinit) or [vec_deinit_with](#vec-deinit-with).

### Parameters

```c
void vec_init(vec_t(T) *vec);
```
- `vec_t(T) *vec`: The uninitialized vec to initialize.

### Example Usage

```c
#include "vec.h"

int main(int argc, char **argv)
{
    // declare and initialize vec
    vec_t(int) numbers;
    vec_init(&numbers);
    
    // initialized vec must be deinitialized
    vec_deinit(&numbers);
    
    return 0;
}
```

### Notes

- Calling anything other than [vec_init](#vec-init) on a vec before initializing it is **undefined behaviour**.
- Calling [vec_init](#vec-init) on an already initialized vec is **undefined behaviour**.

<div dir="rtl">[<a href="#documentation">Back to top</a>]</div>

# vec_deinit <a id="vec-deinit"/>

Deinitialize a vec previously initialized by [vec_init](#vec-init). This does not deinitialize the elements stored within the vec, to deinitialize both the vec and the elements see [vec_deinit_with](#vec-deinit-with).

### Parameters

```c
void vec_deinit(vec_t(T) *vec);
```
- `vec_t(T) *vec`: The initialized vec to deinitialize.

### Example Usage

```c
#include "vec.h"

int main(int argc, char **argv)
{
    // declare and initialize vec
    vec_t(int) numbers;
    vec_init(&numbers);
    
    // initialized vec must be deinitialized
    vec_deinit(&numbers);
    
    return 0;
}
```

### Notes

- You should call either [vec_deinit](#vec-deinit) or [vec_deinit_with](#vec-deinit-with). Not both.
- Calling [vec_deinit](#vec-deinit) on an already deinitialized vec is **undefined behaviour**.
- Calling anything other than [vec_init](#vec-init) an a vec after deinitializing is **undefined behaviour**.

<div dir="rtl">[<a href="#documentation">Back to top</a>]</div>

# vec_deinit_with <a id="vec-deinit-with"/>

Deinitialize a vec and the elements within it by providing a deinitialize function to apply to each element. This is equivalent to manually iterating and deinitializing each element and then calling [vec_deinit](#vec-deinit).

```c
vec_deinit_with(&people, &person_deinit);

// is equivilant to

vec_for_each(&people, person)
{
    person_deinit(person);
}
vec_deinit(&people);
```

### Parameters

```c
void vec_deinit_with(vec_t(T) *vec, void (*deinit_fn)(T* value));
```
- `vec_t(T) *vec`: The initialized vec to deinitialize.
- `void (*deinit_fn)(T* value)`: The deinitialize function to apply to each element in the vec.

### Example Usage

```c
#include <stdint.h>
#include "vec.h"

typedef struct { /* ... */ } person_t;
void person_init(person_t *person, const char *name, uint8_t age);
void person_deinit(person_t *person);

int main(int argc, char **argv)
{
    // declare and initialize vec
    vec_t(person_t) people;
    vec_init(&people);

    // add and initialize a person
    person_t *person = vec_emplace(&people);
    person_init(person, "Alice", 25);
    
    // deinitialize vec and elements
    vec_deinit_with(&people, &person_deinit);

    return 0;
}
```

### Notes

- You should call either [vec_deinit](#vec-deinit) or [vec_deinit_with](#vec-deinit-with). Not both.
- Calling [vec_deinit_with](#vec-deinit-with) on an already deinitialized vec is **undefined behaviour**.
- Calling anything other than [vec_init](#vec-init) an a vec after deinitializing is **undefined behaviour**.

<div dir="rtl">[<a href="#documentation">Back to top</a>]</div>

# vec_len <a id="vec-len"/>

Returns the number of elements in the vec.

### Parameters

```c
size_t vec_len(const vec_t(T) *vec);
```
- `const vec_t(T) *vec`: The vec to obtain the length from.
- `return`: The number of elements in the vec.

### Example Usage

```c
#include <stdint.h>
#include <stdio.h>
#include "vec.h"

int main(int argc, char **argv)
{
    // declare and initialize vec
    vec_t(int) numbers;
    vec_init(&numbers);

    // add some numbers
    vec_push(&numbers, 5);
    vec_push(&numbers, 10);
    vec_push(&numbers, 15);

    // print len
    printf("vec_len: %zu\n", vec_len(&numbers));
    // vec_len: 3

    // deinitialize vec
    vec_deinit(&numbers);

    return 0;
}
```

<div dir="rtl">[<a href="#documentation">Back to top</a>]</div>

# vec_cap <a id="vec-cap"/>

Returns the number of elements that the vec has capacity to store before it needs to reallocate.

### Parameters

```c
size_t vec_cap(const vec_t(T) *vec);
```
- `const vec_t(T) *vec`: The vec to obtain the capacity from.
- `return`: The number of elements the vec has capacity for.

### Example Usage

```c
#include <stdio.h>
#include "vec.h"

int main(int argc, char **argv)
{
    // declare and initialize vec
    vec_t(int) numbers;
    vec_init(&numbers);

    // add some numbers
    vec_push(&numbers, 5);
    vec_push(&numbers, 10);
    vec_push(&numbers, 15);

    // print capacity
    printf("vec_cap: %zu\n", vec_cap(&numbers));
    // vec_cap: 4

    // deinitialize vec
    vec_deinit(&numbers);

    return 0;
}
```

<div dir="rtl">[<a href="#documentation">Back to top</a>]</div>

# vec_push <a id="vec-push"/>

Copies and appends a new element to the end of the vec.

Pushing is convenient for smaller types such as primitives however for larger types copying into the vec can be slow. Instead consider using [vec_emplace](#vec-emplace) to allocate and initialize the element "in place" to avoid copying the value.

If pushing a new element causes `vec_len(&vec) > vec_cap(&vec)` the vec will reallocate. If reallocating fails a panic will occur, if you want to handle the failure case use [vec_try_push](#vec-try-push).

### Parameters

```c
void vec_push(vec_t(T) *vec, T value);
```
- `vec_t(T) *vec`: The vec to push into.
- `T value`: The value to append to the end of the vec.

### Example Usage

```c
#include <stdio.h>
#include "vec.h"

int main(int argc, char **argv)
{
    // declare and initialize vec
    vec_t(int) numbers;
    vec_init(&numbers);

    // add some numbers
    vec_push(&numbers, 5);
    vec_push(&numbers, 10);
    vec_push(&numbers, 15);

    // print the numbers
    vec_for_each_index(&numbers, num, i)
    {
        printf("numbers[%zu] = %d\n", i, *num);
    }
    // numbers[0] = 5
    // numbers[1] = 10
    // numbers[2] = 15

    // deinitialize vec and elements
    vec_deinit(&numbers);

    return 0;
}
```

### Notes

- When vec reallocates it may cause the memory location of it's elements to change, this means that any pre-existing pointers to elements will be invalidated and dereferencing those pointers is **undefined behaviour**.

<div dir="rtl">[<a href="#documentation">Back to top</a>]</div>

# vec_try_push <a id="vec-try-push"/>

Copies and appends a new element to the end of the vec.

Pushing is convenient for smaller types such as primatives however for larger types copying into the vec can be slow. Instead consider using [vec_try_emplace](#vec-try-emplace) to allocate and initialize the element "in place" to avoid copying the value.

If pushing a new element causes `vec_len(&vec) > vec_cap(&vec)` the vec will reallocate. If reallocating fails the vec will be in the same state it was before pushing and `errno` will be set to `ENOMEM`. If you do not care about the failure case use [vec_push](#vec-push).

### Parameters

```c
void vec_try_push(vec_t(T) *vec, T value);
```
- `vec_t(T) *vec`: The vec to push into.
- `T value`: The value to append to the end of the vec.

### Example Usage

```c
#include <stdio.h>
#include "vec.h"

int main(int argc, char **argv)
{
    // declare and initialize vec
    vec_t(int) numbers;
    vec_init(&numbers);

    // try add some numbers
    vec_try_push(&numbers, 5);
    vec_try_push(&numbers, 10);
    vec_try_push(&numbers, 15);

    // stop if an error occoured
    if (errno == ENOMEM)
    {
        printf("out of memory\n");
        vec_deinit(&numbers);
        return 1;
    }

    // print the numbers
    vec_for_each_index(&numbers, num, i)
    {
        printf("numbers[%zu] = %d\n", i, *num);
    }
    // numbers[0] = 5
    // numbers[1] = 10
    // numbers[2] = 15

    // deinitialize vec and elements
    vec_deinit(&numbers);

    return 0;
}
```

### Notes

- When vec reallocates it may cause the memory location of it's elements to change, this means that any pre-existing pointers to elements will be invalidated and dereferencing those pointers is **undefined behaviour**.

<div dir="rtl">[<a href="#documentation">Back to top</a>]</div>

# vec_emplace <a id="vec-emplace"/>

Allocates a new uninitialized element at the end of the vec and returns a pointer to it for initialization.

This should be preferred over [vec_push](#vec-push) for larger types where instead of allocating and initializing a value outside the vec and then copying it into the vec, you can instead allocate and initialize the value "in place" avoiding the need to copy.

If emplacing a new element causes `vec_len(&vec) > vec_cap(&vec)` the vec will reallocate. If reallocating fails a panic will occur, if you want to handle the failure case use [vec_try_emplace](#vec-try-emplace).

### Parameters

```c
T *vec_emplace(vec_t(T) *vec);
```
- `vec_t(T) *vec`: The vec to emplace into.
- `return`: A non-null pointer to the uninitialized new element.

### Example Usage

```c
#include <stdint.h>
#include <stdio.h>
#include "vec.h"

typedef struct { /* ... */ } person_t;
void person_init(person_t *person, const char *name, uint8_t age);
void person_deinit(person_t *person);

int main(int argc, char **argv)
{
    // declare and initialize vec
    vec_t(person_t) people;
    vec_init(&people);

    // add a person using push
    person_t alice;                   // allocate initial person on stack
    person_init(&alice, "Alice", 25); // initialize person
    vec_push(&people, alice);         // copy person into vec

    // add a person using emplace
    person_t *bob = vec_emplace(&people); // allocate person directly within vec
    person_init(bob, "Bob", 30);          // initialize person

    // deinitialize vec and elements
    vec_deinit_with(&people, &person_deinit);

    return 0;
}
```

### Notes

- When vec reallocates it may cause the memory location of it's elements to change, this means that any pre-existing pointers to elements will be invalidated and dereferencing those pointers is **undefined behaviour**.
- [vec_emplace](#vec-emplace) is implemented using the [GNU statement expression extension](https://gcc.gnu.org/onlinedocs/gcc/Statement-Exprs.html). This is supported by both GCC and Clang but is not supported by MSVC. If you need to support MSVC use [vec_push](#vec-push) instead.

<div dir="rtl">[<a href="#documentation">Back to top</a>]</div>

# vec_try_emplace <a id="vec-try-emplace"/>

Allocates a new uninitialized element at the end of the vec and returns a pointer to it for initialization.

This should be preferred over [vec_try_push](#vec-try-push) for larger types where instead of allocating and initializing a value outside the vec and then copying it into the vec, you can instead allocate and initialize the value "in place" avoiding the need to copy.

If emplacing a new element causes `vec_len(&vec) > vec_cap(&vec)` the vec will reallocate. If reallocating fails the vec will be in the same state it was before emplacing, `errno` will be set to `ENOMEM`, and `NULL` will be returned. If you do not care about the failure case use [vec_emplace](#vec-emplace).

### Parameters

```c
T *vec_emplace(vec_t(T) *vec);
```
- `vec_t(T) *vec`: The vec to emplace into.
- `return`: A pointer to the uninitialized new element or `NULL`.

### Example Usage

```c
#include <stdint.h>
#include <stdio.h>
#include "vec.h"

typedef struct { /* ... */ } person_t;
void person_init(person_t *person, const char *name, uint8_t age);
void person_deinit(person_t *person);

int main(int argc, char **argv)
{
    // declare and initialize vec
    vec_t(person_t) people;
    vec_init(&people);

    // add a person using try emplace
    person_t *alice= vec_try_emplace(&people); // allocate person directly within vec
    if (alice != NULL)
    {
        person_init(alice, "Alice", 25); // initialize person
    }

    // deinitialize vec and elements
    vec_deinit_with(&people, &person_deinit);

    return 0;
}
```

### Notes

- When vec reallocates it may cause the memory location of it's elements to change, this means that any pre-existing pointers to elements will be invalidated and dereferencing those pointers is **undefined behaviour**.
- [vec_try_emplace](#vec-try-emplace) is implemented using the [GNU statement expression extension](https://gcc.gnu.org/onlinedocs/gcc/Statement-Exprs.html). This is supported by both GCC and Clang but is not supported by MSVC. If you need to support MSVC use [vec_push](#vec-push) instead.

<div dir="rtl">[<a href="#documentation">Back to top</a>]</div>

# vec_reserve <a id="vec-reserve"/>

Pre-allocate space so that vec has enough capacity to store at least `capacity` elements.

The vec will reallocate if it does not have enough capacity. If it already has a larger capacity then requested it will do nothing. If reallocating fails a panic will occur, if you want to handle the failure case use [vec_try_reserve](#vec-try-reserve).

Reserve should be used if you have a rough idea how many elements your vec will store as it can reduce reallocations and improve performance.

### Parameters

```c
void vec_reserve(vec_t(T) *vec, size_t capacity);
```
- `vec_t(T) *vec`: The uninitialized vec to initialize.
- `size_t capacity`: The number of elements to pre-allocate space for.

### Example Usage

```c
#include "vec.h"

int main(int argc, char **argv)
{
    // declare and initialize the vec
    vec_t(int) numbers;
    vec_init(&numbers);

    // pre-allocate space for 100 ints
    vec_reserve(&numbers, 100);

    // initialized vec must be deinitialized
    vec_deinit(&numbers);
    
    return 0;
}
```

### Notes

- When vec reallocates it may cause the memory location of it's elements to change, this means that any pre-existing pointers to elements will be invalidated and dereferencing those pointers is **undefined behaviour**.

<div dir="rtl">[<a href="#documentation">Back to top</a>]</div>

# vec_try_reserve <a id="vec-try-reserve"/>

Pre-allocate space so that vec has enough capacity to store at least `capacity` elements.

The vec will reallocate if it does not have enough capacity. If it already has a larger capacity then requested it will do nothing. If reallocating fails the vec will be in the same state it was before reserving and `errno` will be set to `ENOMEM`. If you do not care about the failure case use [vec_reserve](#vec-reserve).

Reserve should be used if you have a rough idea how many elements your vec will store as it can reduce reallocations and improve performance.

### Parameters

```c
void vec_try_reserve(vec_t(T) *vec, size_t capacity);
```
- `vec_t(T) *vec`: The uninitialized vec to initialize.
- `size_t capacity`: The number of elements to pre-allocate space for.

### Example Usage

```c
#include "vec.h"

int main(int argc, char **argv)
{
    // declare and initialize the vec
    vec_t(int) numbers;
    vec_init(&numbers);

    // pre-allocate space for 100 ints
    vec_try_reserve(&numbers, 100);
    if (errno == ENOMEM)
    {
        // reserving failed
        return 1;
    }

    // initialized vec must be deinitialized
    vec_deinit(&numbers);
    
    return 0;
}
```

### Notes

- When vec reallocates it may cause the memory location of it's elements to change, this means that any pre-existing pointers to elements will be invalidated and dereferencing those pointers is **undefined behaviour**.

<div dir="rtl">[<a href="#documentation">Back to top</a>]</div>

# vec_pop <a id="vec-pop"/>

Removes the last element from the end of the vec.

If the vec is empty an assertion will trigger. If assertions are disabled popping an empty vec is **undefined behaviour**. You can check if there are elements remaining in the vec with [vec_len](#vec-len).

If the element being removed needs to be deinitialized consider using [vec_pop_with](#vec-pop-with).

### Parameters

```c
void vec_pop(vec_t(T) *vec);
```
- `vec_t(T) *vec`: The non-empty vec.

### Example Usage

```c
#include <stdint.h>
#include <stdio.h>
#include "vec.h"

void print_numbers(float *numbers, size_t numbers_len);

int main(int argc, char **argv)
{
    // declare and initialize vec
    vec_t(float) numbers;
    vec_init(&numbers);

    // add numbers
    vec_push(&numbers, 1.0f);
    vec_push(&numbers, 2.0f);
    vec_push(&numbers, 3.0f);

    // print initial numbers
    print_numbers(numbers, vec_len(&numbers));
    // 1.0f, 2.0f, 3.0f

    // pop vec
    vec_pop(&numbers);

    // print numbers
    print_numbers(numbers, vec_len(&numbers));
    // 1.0f, 2.0f

    // deinitialize vec
    vec_deinit(&numbers);

    return 0;
}
```

<div dir="rtl">[<a href="#documentation">Back to top</a>]</div>

# vec_pop_with <a id="vec-pop-with"/>

Deinitializes and removes the last element from the end of the vec.

If the vec is empty the an assertion will trigger. If assertions are disabled popping an empty vec is **undefined behaviour**. You can check if there are elements remaining in the vec with [vec_len](#vec-len).

If the element does not need to be deinitialized use [vec_pop](#vec-pop)

```c
vec_pop_with(&people, &person_deinit);

// is equivilant to

person_deinit(&people[vec_len(&people) - 1]);
vec_pop(&people);
```

### Parameters

```c
void vec_pop_with(vec_t(T) *vec, void (*deinit_fn)(T* value));
```
- `vec_t(T) *vec`: The non-empty vec.
- `void (*deinit_fn)(T* value)`: The deinitialize function to apply to the element being removed.

### Example Usage

```c
#include <stdint.h>
#include <stdio.h>
#include "vec.h"

typedef struct { /* ... */ } person_t;
void person_init(person_t *person, const char *name, uint8_t age);
void person_deinit(person_t *person);
void print_people(const person_t *people, size_t people_len);

int main(int argc, char **argv)
{
    // declare and initialize vec
    vec_t(person_t) people;
    vec_init(&people);
    vec_reserve(&people, 2);

    // add people
    person_t *alice = vec_emplace(&people);
    person_t *bob = vec_emplace(&people);
    person_init(alice, "Alice", 25);
    person_init(bob, "Bob", 30);

    // print initial people
    print_people(people, vec_len(&people));
    // Alice 25, Bob 30

    // pop last element
    vec_pop_with(&people, &person_deinit);

    // print people
    print_people(people, vec_len(&people));
    // Alice 25

    // deinitialize vec and elements
    vec_deinit_with(&people, &person_deinit);

    return 0;
}
```

<div dir="rtl">[<a href="#documentation">Back to top</a>]</div>

# vec_clear <a id="vec-clear"/>

Removes all elements from the vec leaving it empty. This does not affect the capacity of the vec.

If you need to deinitialize the elements consider using [vec_clear_with](#vec-clear-with).

### Parameters

```c
void vec_clear(vec_t(T) *vec);
```
- `vec_t(T) *vec`: The vec to clear.

### Example Usage

```c
#include <stdio.h>
#include "vec.h"

int main(int argc, char **argv)
{
    // declare and initialize vec
    vec_t(int) numbers;
    vec_init(&numbers);

    // add some numbers
    vec_push(&numbers, 5);
    vec_push(&numbers, 10);
    vec_push(&numbers, 15);

    // print len
    printf("vec_len: %zu\n", vec_len(&numbers));
    // vec_len: 3

    // clear the vec
    vec_clear(&numbers);

    // print len
    printf("vec_len: %zu\n", vec_len(&numbers));
    // vec_len: 0

    // deinitialize vec
    vec_deinit(&numbers);

    return 0;
}
```

<div dir="rtl">[<a href="#documentation">Back to top</a>]</div>

# vec_clear_with <a id="vec-clear-with"/>

Deinitializes and removes all elements from the vec leaving it empty. This does not affect the capacity of the vec.

If you do not need to deinitialize the elements use [vec_clear](#vec-clear).

```c
vec_clear_with(&people, &person_deinit);

// is equivilant to

vec_for_each(&people, person)
{
    person_deinit(person);
}
vec_clear(&people);
```

### Parameters

```c
void vec_clear_with(vec_t(T) *vec, void (*deinit_fn)(T* value));
```
- `vec_t(T) *vec`: The vec to clear.
- `void (*deinit_fn)(T* value)`: The deinitialize function to apply to each element in the vec.

### Example Usage

```c
#include <stdint.h>
#include <stdio.h>
#include "vec.h"

typedef struct { /* ... */ } person_t;
void person_init(person_t *person, const char *name, uint8_t age);
void person_deinit(person_t *person);

int main(int argc, char **argv)
{
    // declare and initialize vec
    vec_t(person_t) people;
    vec_init(&people);
    vec_reserve(&people, 2);

    // add people
    person_t *alice = vec_emplace(&people);
    person_t *bob = vec_emplace(&people);
    person_init(alice, "Alice", 25);
    person_init(bob, "Bob", 30);

    // print initial len
    printf("vec_len: %zu\n", vec_len(&people));
    // vec_len: 2

    // clear the vec
    vec_clear_with(&people, &person_deinit);

    // print len
    printf("vec_len: %zu\n", vec_len(&people));
    // vec_len: 0

    // deinitialize vec, note that we don't need to use vec_deinit_with because
    // we know the vec is empty.
    vec_deinit(&people);

    return 0;
}
```

<div dir="rtl">[<a href="#documentation">Back to top</a>]</div>

# vec_remove_swap_at <a id="vec-remove-swap-at"/>

Remove an element at `index` from the vec. The removed element is replaced by the last element of the vector.

This is an *O(1)* operation but does not preserve the ordering of elements in the vec. If the order of elements matters use [vec_remove_shift_at](#vec-remove-shift-at) instead.

If you need to deinitialize the element being removed consider using [vec_remove_swap_at_with](#vec-remove-swap-at-with).

If the provided index is out of bounds an assertion will trigger. If assertions are disabled it is **undefined behaviour**.

### Parameters

```c
void vec_remove_swap_at(vec_t(T) *vec, size_t index);
```
- `vec_t(T) *vec`: The vec to remove the element from.
- `index`: The index of the element to remove.

### Example Usage

```c
#include <stdint.h>
#include <stdio.h>
#include "vec.h"

void print_numbers(int *numbers, size_t numbers_len);

int main(int argc, char **argv)
{
    // declare and initialize vec
    vec_t(int) numbers;
    vec_init(&numbers);

    // add some numbers
    vec_push(&numbers, 1);
    vec_push(&numbers, 2);
    vec_push(&numbers, 3);
    vec_push(&numbers, 4);
    vec_push(&numbers, 5);

    // print initial numbers
    print_numbers(numbers, vec_len(&numbers));
    // 1, 2, 3, 4, 5

    // remove element at numbers[1] (the number 2)
    vec_remove_swap_at(&numbers, 1);

    // print numbers
    print_numbers(numbers, vec_len(&numbers));
    // 1, 5, 3, 4

    // deinitialize vec
    vec_deinit(&numbers);

    return 0;
}
```

<div dir="rtl">[<a href="#documentation">Back to top</a>]</div>

# vec_remove_swap_at_with <a id="vec-remove-swap-at-with"/>

Deinitialize and remove an element at `index` from the vec. The removed element is replaced by the last element of the vector.

This is an *O(1)* operation but does not preserve the ordering of elements in the vec. If the order of elements matters use [vec_remove_shift_at_with](#vec-remove-shift-at-with) instead.

If you do not need to deinitialize the element being removed use [vec_remove_swap_at](#vec-remove-swap-at).

If the provided index is out of bounds an assertion will trigger. If assertions are disabled it is **undefined behaviour**.

```c
vec_remove_swap_at_with(&people, 2, &person_deinit);

// is equivilant to

person_deinit(&people[2]);
vec_remove_swap_at(&people, 2);
```

### Parameters

```c
void vec_remove_swap_at_with(vec_t(T) *vec, size_t index, void (*deinit_fn)(T* value));
```
- `vec_t(T) *vec`: The vec to remove the element from.
- `index`: The index of the element to remove.
- `void (*deinit_fn)(T* value)`: The deinitialize function to apply to the element being removed.

### Example Usage

```c
#include <stdint.h>
#include <stdio.h>
#include "vec.h"

typedef struct { /* ... */ } person_t;
void person_init(person_t *person, const char *name, uint8_t age);
void person_deinit(person_t *person);
void print_people(const person_t *people, size_t people_len);

int main(int argc, char **argv)
{
    // declare and initialize vec
    vec_t(person_t) people;
    vec_init(&people);
    vec_reserve(&people, 5);

    // add some people
    person_t *alice = vec_emplace(&people);
    person_t *bob = vec_emplace(&people);
    person_t *charlie = vec_emplace(&people);
    person_t *dave = vec_emplace(&people);
    person_t *eve = vec_emplace(&people);
    person_init(alice, "Alice", 30);
    person_init(bob, "Bob", 31);
    person_init(charlie, "Charlie", 32);
    person_init(dave, "Dave", 33);
    person_init(eve, "Eve", 34);

    // print initial people
    print_people(people, vec_len(&people));
    // Alice 30, Bob 31, Charlie 32, Dave 33, Eve 34

    // remove element at people[1] (Bob)
    vec_remove_swap_at_with(&people, 1, &person_deinit);

    // print people
    print_people(people, vec_len(&people));
    // Alice 30, Eve 34, Charlie 32, Dave 33

    // deinitialize vec and elements
    vec_deinit_with(&people, &person_deinit);

    return 0;
}
```

<div dir="rtl">[<a href="#documentation">Back to top</a>]</div>

# vec_remove_shift_at <a id="vec-remove-shift-at"/>

Remove an element at `index` from the vec, shifting all elements after the removed element to the left.

This has a worst-case performance of *O(n)* due to the shifting of elements. If the ordering of elements in the vec is not important use [vec_remove_swap_at](#vec-remove-swap-at) instead as it is faster.

If you need to deinitialize the element being removed consider using [vec_remove_shift_at_with](#vec-remove-shift-at-with).

If the provided index is out of bounds an assertion will trigger. If assertions are disabled it is **undefined behaviour**.

### Parameters

```c
void vec_remove_shift_at(vec_t(T) *vec, size_t index);
```
- `vec_t(T) *vec`: The vec to remove the element from.
- `index`: The index of the element to remove.

### Example Usage

```c
#include <stdint.h>
#include <stdio.h>
#include "vec.h"

void print_numbers(int *numbers, size_t numbers_len);

int main(int argc, char **argv)
{
    // declare and initialize vec
    vec_t(int) numbers;
    vec_init(&numbers);

    // add some numbers
    vec_push(&numbers, 1);
    vec_push(&numbers, 2);
    vec_push(&numbers, 3);
    vec_push(&numbers, 4);
    vec_push(&numbers, 5);

    // print initial numbers
    print_numbers(numbers, vec_len(&numbers));
    // 1, 2, 3, 4, 5

    // remove element at numbers[1] (the number 2)
    vec_remove_shift_at(&numbers, 1);

    // print numbers
    print_numbers(numbers, vec_len(&numbers));
    // 1, 3, 4, 5

    // deinitialize vec
    vec_deinit(&numbers);

    return 0;
}
```

<div dir="rtl">[<a href="#documentation">Back to top</a>]</div>

# vec_remove_shift_at_with <a id="vec-remove-shift-at-with"/>

Deinitialize and remove an element at `index` from the vec, shifting all elements after the removed element to the left.

This has a worst-case performance of *O(n)* due to the shifting of elements. If the ordering of elements in the vec is not important use [vec_remove_swap_at_with](#vec-remove-swap-at-with) instead as it is faster.

If you do not need to deinitialize the element being removed use [vec_remove_shift_at](#vec-remove-shift-at).

If the provided index is out of bounds an assertion will trigger. If assertions are disabled it is **undefined behaviour**.

```c
vec_remove_shift_at_with(&people, 2, &person_deinit);

// is equivilant to

person_deinit(&people[2]);
vec_remove_shift_at(&people, 2);
```

### Parameters

```c
void vec_remove_shift_at_with(vec_t(T) *vec, size_t index, void (*deinit_fn)(T* value));
```
- `vec_t(T) *vec`: The vec to remove the element from.
- `index`: The index of the element to remove.
- `void (*deinit_fn)(T* value)`: The deinitialize function to apply to the element being removed.

### Example Usage

```c
#include <stdint.h>
#include <stdio.h>
#include "vec.h"

typedef struct { /* ... */ } person_t;
void person_init(person_t *person, const char *name, uint8_t age);
void person_deinit(person_t *person);
void print_people(const person_t *people, size_t people_len);

int main(int argc, char **argv)
{
    // declare and initialize vec
    vec_t(person_t) people;
    vec_init(&people);
    vec_reserve(&people, 5);

    // add some people
    person_t *alice = vec_emplace(&people);
    person_t *bob = vec_emplace(&people);
    person_t *charlie = vec_emplace(&people);
    person_t *dave = vec_emplace(&people);
    person_t *eve = vec_emplace(&people);
    person_init(alice, "Alice", 30);
    person_init(bob, "Bob", 31);
    person_init(charlie, "Charlie", 32);
    person_init(dave, "Dave", 33);
    person_init(eve, "Eve", 34);

    // print initial people
    print_people(people, vec_len(&people));
    // Alice 30, Bob 31, Charlie 32, Dave 33, Eve 34

    // remove element at people[1] (Bob)
    vec_remove_shift_at_with(&people, 1, &person_deinit);

    // print people
    print_people(people, vec_len(&people));
    // Alice 30, Charlie 32, Dave 33, Eve 34

    // deinitialize vec and elements
    vec_deinit_with(&people, &person_deinit);

    return 0;
}
```

<div dir="rtl">[<a href="#documentation">Back to top</a>]</div>

# vec_for_each <a id="vec-for-each"/>

Convenience macro to iterate over the values of a vec. The macro takes two arguments: a pointer to the vec and the name of the value variable to be used for each iteration.

During each iteration, the current value can be accessed using the variable name you specified, which will be a pointer to the current value. For example, in the provided code snippet:

```c
vec_t(float) my_vec;
// ...
vec_for_each(&my_vec, num)
{
    printf("%f\n", *num);
}
```

The macro is used to loop over the values of `my_vec`, assigning the address of the current value to the variable `num` each iteration.

If you need the current index while iterating, use [vec_for_each_index](#vec-for-each-index) instead.

### Parameters

```c
vec_for_each(vec_t(T) *vec, value_ptr);
```
- `vec_t(T) *vec`: The vec to iterate.
- `value_ptr`: The name of the variable that contains a pointer to the current iterations value.

### Example Usage

```c
#include <stdint.h>
#include <stdio.h>
#include "vec.h"

typedef struct { /* ... */ } person_t;
void person_init(person_t *person, const char *name, uint8_t age);
void person_deinit(person_t *person);
void person_print(const person_t *person);

int main(int argc, char **argv)
{
    // declare and initialize vec
    vec_t(person_t) people;
    vec_init(&people);
    vec_reserve(&people, 2);

    // add some people
    person_t *alice = vec_emplace(&people);
    person_t *bob = vec_emplace(&people);
    person_init(alice, "Alice", 25);
    person_init(bob, "Bob", 30);

    // print people using a normal for loop
    for (size_t i=0; i<vec_len(&people); i++)
    {
        person_print(&people[i]);
    }

    // print people using vec_for_each
    vec_for_each(&people, p);
    {
        person_print(p);
    }

    // deinitialize vec and elements
    vec_deinit_with(&people, &person_deinit);

    return 0;
}
```

### Notes

- [vec_for_each](#vec-for-each) is implemented using the [GNU typeof extension](https://gcc.gnu.org/onlinedocs/gcc/Typeof.html). This is supported by both GCC and Clang but is not supported by MSVC. If you need to support MSVC use an ordinary for loop instead.

<div dir="rtl">[<a href="#documentation">Back to top</a>]</div>

# vec_for_each_index <a id="vec-for-each-index"/>

Convenience macro to iterate over the values and indices of a vec. The macro takes three arguments: a pointer to the vec, the name of the value variable to be used for each iteration, and the name if the index variable to be used for each iteration.

During each iteration, the current value and index can be accessed using the variable names you specified, the value variable will be a pointer to the current value, the index will be a `size_t`. For example, in the provided code snippet:

```c
vec_t(float) my_vec;
// ...
vec_for_each_index(&my_vec, num, i)
{
    printf("my_vec[%zu] = %f\n", i, *num);
}
```

The macro is used to loop over the values of `my_vec`, assigning the address of the current value to the variable `num` and the current index to `i` each iteration.

If you do not need the current index while iterating, use [vec_for_each](#vec-for-each) instead.

### Parameters

```c
vec_for_each_index(vec_t(T) *vec, value_ptr, index);
```
- `vec_t(T) *vec`: The vec to iterate.
- `value_ptr`: The name of the variable that contains a pointer to the current iterations value.
- `index`: The name of the variable that contains the index of current iterations value.

### Example Usage

```c
#include <stdint.h>
#include <stdio.h>
#include "vec.h"

typedef struct { /* ... */ } person_t;
void person_init(person_t *person, const char *name, uint8_t age);
void person_deinit(person_t *person);
void person_print(const person_t *person);

int main(int argc, char **argv)
{
    // declare and initialize vec
    vec_t(person_t) people;
    vec_init(&people);
    vec_reserve(&people, 2);

    // add some people
    person_t *alice = vec_emplace(&people);
    person_t *bob = vec_emplace(&people);
    person_init(alice, "Alice", 25);
    person_init(bob, "Bob", 30);

    // print people using a normal for loop
    for (size_t i=0; i<vec_len(&people); i++)
    {
        printf("person #%zu = ", i+1);
        person_print(&people[i]);
    }

    // print people using vec_for_each_index
    vec_for_each_index(&people, p, i);
    {
        printf("person #%zu = ", i+1);
        person_print(p);
    }

    // deinitialize vec and elements
    vec_deinit_with(&people, &person_deinit);

    return 0;
}
```

### Notes

- [vec_for_each_index](#vec-for-each-index) is implemented using the [GNU typeof extension](https://gcc.gnu.org/onlinedocs/gcc/Typeof.html). This is supported by both GCC and Clang but is not supported by MSVC. If you need to support MSVC use an ordinary for loop instead.

<div dir="rtl">[<a href="#documentation">Back to top</a>]</div>


# vec__has_header <a id="vec--has-header"/>

⚠️ **This exists for internal use and may be removed without warning. Use at your own risk.** ⚠️ 

Checks if a vec has a header that can be retrieved with [vec__get_header](#vec--get-header).

### Parameters

```c
bool vec__has_header(vec_t(T) *vec);
```
- `vec_t(T) *vec`: The vec to check.
- `return`: true if vec has a header, false if it does not.

### Example Usage

```c
#include <stdio.h>
#include "vec.h"

int main(int argc, char **argv)
{
    // declare and initialize vec
    vec_t(int) numbers;
    vec_init(&numbers);

    // add some numbers
    vec_push(&numbers, 5);
    vec_push(&numbers, 10);
    vec_push(&numbers, 15);

    if (vec__has_header(&numbers))
    {
        vec__header_t *header = vec__get_header(&numbers);

        // print len
        printf("vec_len: %zu\n", header->len);
        // vec_len: 3
    }

    // deinitialize vec
    vec_deinit(&numbers);

    return 0;
}
```

<div dir="rtl">[<a href="#documentation">Back to top</a>]</div>

# vec__get_header <a id="vec--get-header"/>

⚠️ **This exists for internal use and may be removed without warning. Use at your own risk.** ⚠️ 

Gets the header of the vec. Calling `vec__get_header(&vec)` when `!vec__has_header(&vec)` is **undefined behaviour**.

### Parameters

```c
vec__header_t *vec__get_header(vec_t(T) *vec);
```
- `vec_t(T) *vec`: The vec to obtain the header of.
- `return`: The header of the vec.

### Example Usage

```c
#include <stdio.h>
#include "vec.h"

int main(int argc, char **argv)
{
    // declare and initialize vec
    vec_t(int) numbers;
    vec_init(&numbers);

    // add some numbers
    vec_push(&numbers, 5);
    vec_push(&numbers, 10);
    vec_push(&numbers, 15);

    if (vec__has_header(&numbers))
    {
        vec__header_t *header = vec__get_header(&numbers);

        // print len
        printf("vec_len: %zu\n", header->len);
        // vec_len: 3
    }

    // deinitialize vec
    vec_deinit(&numbers);

    return 0;
}
```

# vec__set_header <a id="vec--set-header"/>

⚠️ **This exists for internal use and may be removed without warning. Use at your own risk.** ⚠️ 

Sets the header of the vec.

### Parameters

```c
void vec__set_header(vec_t(T) *vec, vec__header_t *header);
```
- `vec_t(T) *vec`: The vec to set the header of.
- `vec__header_t *header`: The header to assign to the vec.

### Example Usage

```c
#include <stdio.h>
#include "vec.h"

int main(int argc, char **argv)
{
    // declare and initialize vec
    vec_t(int) numbers;
    vec_init(&numbers);

    // reserve a capacity of 3 elements manually
    vec__header_t *header = VEC_MALLOC(sizeof(vec__header_t) + sizeof(int) * 3);
    if (header != NULL)
    {
        vec__set_header(&numbers, header);
        header->len = 0;
        header->cap = 3;
    }

    // deinitialize vec
    vec_deinit(&numbers);

    return 0;
}
```

<div dir="rtl">[<a href="#documentation">Back to top</a>]</div>

# vec__grow_or_fail <a id="vec--grow-or-fail"/>

⚠️ **This exists for internal use and may be removed without warning. Use at your own risk.** ⚠️ 

Allocates an uninitialized new element at the end of the vec. If the new element causes `vec_len(&vec) > vec_cap(&vec)` the vec will reallocate. If a new element cannot be allocated `fail_block` is executed.

### Parameters

```c
void vec__grow_or_fail(vec_t(T) *vec, fail_block);
```
- `vec_t(T) *vec`: The vec to grow.
- `fail_block`: block of code to execute when allocation fails.

### Example Usage

```c
#include <stdio.h>
#include "vec.h"

int main(int argc, char **argv)
{
    // declare and initialize vec
    vec_t(int) numbers;
    vec_init(&numbers);

    // grow vec to fit a new element
    vec__grow_or_fail(&numbers, { printf("Oh no!"); return 1; });
    numbers[0] = 10;

    // deinitialize vec
    vec_deinit(&numbers);

    return 0;
}
```

### Notes

- When vec reallocates it may cause the memory location of it's elements to change, this means that any pre-existing pointers to elements will be invalidated and dereferencing those pointers is **undefined behaviour**.

<div dir="rtl">[<a href="#documentation">Back to top</a>]</div>

# vec__reserve_or_fail <a id="vec--reserve-or-fail"/>

⚠️ **This exists for internal use and may be removed without warning. Use at your own risk.** ⚠️ 

Pre-allocate space so that vec has enough capacity to store at least `capacity` elements. The vec will reallocate if it does not have enough capacity. If it already has a larger capacity then requested it will do nothing. If reallocating fails `fail_block` is executed.

### Parameters

```c
void vec__reserve_or_fail(vec_t(T) *vec, size_t capacity, fail_block);
```
- `vec_t(T) *vec`: The vec to grow.
- `size_t capacity`: The number of elements to allocate space for.
- `fail_block`: block of code to execute when allocation fails.

### Example Usage

```c
#include <stdio.h>
#include "vec.h"

int main(int argc, char **argv)
{
    // declare and initialize vec
    vec_t(int) numbers;
    vec_init(&numbers);

    // reserve space for 100 ints.
    vec__reserve_or_fail(&numbers, 100, { printf("Oh no!"); return 1; });

    // deinitialize vec
    vec_deinit(&numbers);

    return 0;
}
```

### Notes

- When vec reallocates it may cause the memory location of it's elements to change, this means that any pre-existing pointers to elements will be invalidated and dereferencing those pointers is **undefined behaviour**.

<div dir="rtl">[<a href="#documentation">Back to top</a>]</div>
