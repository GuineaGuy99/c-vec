/*
	MIT License

	Copyright (c) 2023 GuineaGuy99

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.
*/

#pragma once

#include <assert.h>
#include <errno.h>
#include <stddef.h>
#include <stdlib.h>

/*
	Version 1.0.0
	GitHub https://github.com/GuineaGuy99/c-vec

	##########################
	# Quick Library Overview #
	##########################

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
*/

/*===========*/
/* constants */
/*===========*/

#define VEC_VERSION_MAJOR 1
#define VEC_VERSION_MINOR 0
#define VEC_VERSION_PATCH 0

/*=======*/
/* types */
/*=======*/

typedef struct
{
	size_t len;
	size_t cap;
} vec__header_t;

/*==========================*/
/* user configurable macros */
/*==========================*/

#ifndef VEC_MALLOC
#define VEC_MALLOC(size) malloc(size)
#endif

#ifndef VEC_REALLOC
#define VEC_REALLOC(ptr, size) realloc(ptr, size)
#endif

#ifndef VEC_FREE
#define VEC_FREE(ptr) free(ptr)
#endif

#ifndef VEC_PANIC
#define VEC_PANIC(reason) do { fprintf(stderr, "%s\n", (reason)); exit(EXIT_FAILURE); } while (0)
#endif

#ifndef VEC_ASSERT
#define VEC_ASSERT(condition, reason) assert((reason) && (condition))
#endif

/*========*/
/* macros */
/*========*/

#define vec_t(type) type*
#define vec_init(vec) *(vec) = NULL
#define vec_deinit(vec) VEC_FREE(vec__has_header(vec) ? vec__get_header(vec) : NULL)
#define vec_len(vec) (vec__has_header(vec) ? vec__get_header(vec)->len : 0)
#define vec_cap(vec) (vec__has_header(vec) ? vec__get_header(vec)->cap : 0)
#define vec_reserve(vec, capacity) vec__reserve_or_fail(vec, capacity, { VEC_PANIC("vec_reserve out of memory"); });
#define vec_try_reserve(vec, capacity) vec__reserve_or_fail(vec, capacity, { errno = ENOMEM; });
#define vec__has_header(vec) (*(vec) != NULL)
#define vec__get_header(vec) (((vec__header_t*)*(vec)) - 1) // warning: UB if !vec__has_header
#define vec__set_header(vec, header) (*(vec) = (void*)((header) + 1))

#define vec_deinit_with(vec, deinit_fn)                              \
	do                                                               \
	{                                                                \
		if (vec__has_header(vec))                                    \
		{                                                            \
			vec__header_t *_vec_header = vec__get_header(vec);       \
			for (size_t _vec_i=0; _vec_i<_vec_header->len; _vec_i++) \
			{                                                        \
				(deinit_fn)(*(vec) + _vec_i);                        \
			}                                                        \
			VEC_FREE(_vec_header);                                   \
		}                                                            \
	} while (0)

#define vec_push(vec, value)                                              \
	do                                                                    \
	{                                                                     \
		vec__grow_or_fail(vec, { VEC_PANIC("vec_push out of memory"); }); \
		/* grow guarantees header */                                      \
		(*(vec))[vec__get_header(vec)->len - 1] = (value);                \
	} while (0)

#define vec_try_push(vec, value)                               \
	do                                                         \
	{                                                          \
		vec__grow_or_fail(vec, { errno = ENOMEM; });           \
		if (errno != ENOMEM)                                   \
		{                                                      \
			/* grow guarantees header */                       \
			(*(vec))[vec__get_header(vec)->len - 1] = (value); \
		}                                                      \
	} while (0)

#define vec_emplace(vec)                                                     \
	({                                                                       \
		vec__grow_or_fail(vec, { VEC_PANIC("vec_emplace out of memory"); }); \
		/* grow guarantees header */                                         \
		*(vec) + vec__get_header(vec)->len - 1;                              \
	})

#define vec_try_emplace(vec)                                                 \
	({                                                                       \
		vec__grow_or_fail(vec, { errno = ENOMEM; });                         \
		/* grow guarantees header */                                         \
		(errno != ENOMEM : (*(vec) + vec__get_header(vec)->len - 1) : NULL); \
	})

#define vec_pop(vec)                                   \
	do                                                 \
	{                                                  \
		VEC_ASSERT(vec_len(vec) > 0, "vec_pop empty"); \
		/* assert guarantees header */                 \
		vec__get_header(vec)->len -= 1;                \
	} while (0)

#define vec_pop_with(vec, deinit_fn)                       \
	do                                                     \
	{                                                      \
		VEC_ASSERT(vec_len(vec) > 0, "vec_pop_with empty") \
		/* assert guarantees header */                     \
		vec__header_t *_vec_header = vec__get_header(vec); \
		_vec_header->len -= 1;                             \
		(deinit_fn)(*(vec) + _vec_header->len);            \
	} while (0)

#define vec_clear(vec)                                         \
	do                                                         \
	{                                                          \
		if (vec__has_header(vec))                              \
		{                                                      \
			vec__header_t *_vec_header = vec__get_header(vec); \
			_vec_header->len = 0;                              \
		}                                                      \
	} while (0)

#define vec_clear_with(vec, deinit_fn)                               \
	do                                                               \
	{                                                                \
		if (vec__has_header(vec))                                    \
		{                                                            \
			vec__header_t *_vec_header = vec__get_header(vec);       \
			for (size_t _vec_i=0; _vec_i<_vec_header->len; _vec_i++) \
			{                                                        \
				(deinit_fn)(*(vec) + _vec_i);                        \
			}                                                        \
			_vec_header->len = 0;                                    \
		}                                                            \
	} while (0)

#define vec_remove_swap_at(vec, index)                                             \
	do                                                                             \
	{                                                                              \
		size_t _vec_index = (index);                                               \
		VEC_ASSERT(_vec_index < vec_len(vec), "vec_remove_swap_at out of bounds"); \
		/* assert guarantees header */                                             \
		vec__header_t *_vec_header = vec__get_header(vec);                         \
		_vec_header->len -= 1;                                                     \
		(*(vec))[_vec_index] = (*(vec))[_vec_header->len];                         \
	} while (0)

#define vec_remove_swap_at_with(vec, index, deinit_fn)                                  \
	do                                                                                  \
	{                                                                                   \
		size_t _vec_index = (index);                                                    \
		VEC_ASSERT(_vec_index < vec_len(vec), "vec_remove_swap_at_with out of bounds"); \
		/* assert guarantees header */                                                  \
		vec__header_t *_vec_header = vec__get_header(vec);                              \
		_vec_header->len -= 1;                                                          \
		(deinit_fn)(*(vec) + _vec_index);                                               \
		(*(vec))[_vec_index] = (*(vec))[_vec_header->len];                              \
	} while (0)

#define vec_remove_shift_at(vec, index)                                             \
	do                                                                              \
	{                                                                               \
		size_t _vec_index = (index);                                                \
		VEC_ASSERT(_vec_index < vec_len(vec), "vec_remove_shift_at out of bounds"); \
		/* assert guarantees header */                                              \
		vec__header_t *_vec_header = vec__get_header(vec);                          \
		_vec_header->len -= 1;                                                      \
		for (size_t _vec_i=_vec_index; _vec_i<_vec_header->len; _vec_i++)           \
		{                                                                           \
			(*(vec))[_vec_i] = (*(vec))[_vec_i+1];                                  \
		}                                                                           \
	} while (0)

#define vec_remove_shift_at_with(vec, index, deinit_fn)                                 \
	do                                                                                  \
	{                                                                                   \
		size_t _vec_index = (index);                                                    \
		VEC_ASSERT(_vec_index < vec_len(vec), "vec_remove_swap_at_with out of bounds"); \
		/* assert guarantees header */                                                  \
		vec__header_t *_vec_header = vec__get_header(vec);                              \
		_vec_header->len -= 1;                                                          \
		(deinit_fn)(*(vec) + _vec_index);                                               \
		for (size_t _vec_i=_vec_index; _vec_i<_vec_header->len; _vec_i++)               \
		{                                                                               \
			(*(vec))[_vec_i] = (*(vec))[_vec_i+1];                                      \
		}                                                                               \
	} while (0)

#define vec__grow_or_fail(vec, ...)                                                                                   \
	do                                                                                                                \
	{                                                                                                                 \
		if (!vec__has_header(vec))                                                                                    \
		{                                                                                                             \
			vec__header_t *_vec_header;                                                                               \
			if ((_vec_header = VEC_MALLOC(sizeof(vec__header_t) + sizeof(**(vec)))))                                  \
			{                                                                                                         \
				vec__set_header(vec, _vec_header);                                                                    \
				_vec_header->cap = 1;                                                                                 \
				_vec_header->len = 1;                                                                                 \
			}                                                                                                         \
			else                                                                                                      \
			{                                                                                                         \
				__VA_ARGS__                                                                                           \
			}                                                                                                         \
		}                                                                                                             \
		else                                                                                                          \
		{                                                                                                             \
			vec__header_t *_vec_header = vec__get_header(vec);                                                        \
			if (_vec_header->len < _vec_header->cap)                                                                  \
			{                                                                                                         \
				_vec_header->len += 1;                                                                                \
			}                                                                                                         \
			else                                                                                                      \
			{                                                                                                         \
				size_t _vec_new_cap = _vec_header->cap * 2;                                                           \
				if ((_vec_header = VEC_REALLOC(_vec_header, sizeof(vec__header_t) + sizeof(**(vec)) * _vec_new_cap))) \
				{                                                                                                     \
					vec__set_header(vec, _vec_header);                                                                \
					_vec_header->cap = _vec_new_cap;                                                                  \
					_vec_header->len += 1;                                                                            \
				}                                                                                                     \
				else                                                                                                  \
				{                                                                                                     \
					__VA_ARGS__                                                                                       \
				}                                                                                                     \
			}                                                                                                         \
		}                                                                                                             \
	} while (0)

#define vec__reserve_or_fail(vec, capacity, ...)                                                                       \
	do                                                                                                                 \
	{                                                                                                                  \
		size_t _vec_capacity = (capacity);                                                                             \
		if (_vec_capacity == 0) break;                                                                                 \
		if (!vec__has_header(vec))                                                                                     \
		{                                                                                                              \
			vec__header_t *_vec_header;                                                                                \
			if ((_vec_header = VEC_MALLOC(sizeof(vec__header_t) + sizeof(**(vec)) * _vec_capacity)))                   \
			{                                                                                                          \
				vec__set_header(vec, _vec_header);                                                                     \
				_vec_header->cap = _vec_capacity;                                                                      \
				_vec_header->len = 0;                                                                                  \
			}                                                                                                          \
			else                                                                                                       \
			{                                                                                                          \
				__VA_ARGS__                                                                                            \
			}                                                                                                          \
		}                                                                                                              \
		else                                                                                                           \
		{                                                                                                              \
			vec__header_t *_vec_header = vec__get_header(vec);                                                         \
			if (_vec_capacity > _vec_header->cap)                                                                      \
			{                                                                                                          \
				if ((_vec_header = VEC_REALLOC(_vec_header, sizeof(vec__header_t) + sizeof(**(vec)) * _vec_capacity))) \
				{                                                                                                      \
					vec__set_header(vec, _vec_header);                                                                 \
					_vec_header->cap = _vec_capacity;                                                                  \
				}                                                                                                      \
				else                                                                                                   \
				{                                                                                                      \
					__VA_ARGS__                                                                                        \
				}                                                                                                      \
			}                                                                                                          \
		}                                                                                                              \
	} while (0)

#define vec_for_each(vec, value_ptr)               \
	if (vec__has_header(vec))                      \
		for (size_t _vec_i=0; _vec_i==0; ++_vec_i) \
			for(__typeof__(**(vec)) *(value_ptr)=*(vec); _vec_i<vec__get_header(vec)->len; (value_ptr)=*(vec)+(++_vec_i))

#define vec_for_each_index(vec, value_ptr, index) \
	if (vec__has_header(vec))                     \
		for (size_t index=0; index==0; ++index)   \
			for(__typeof__(**(vec)) *(value_ptr)=*(vec); index<vec__get_header(vec)->len; (value_ptr)=*(vec)+(++index))
