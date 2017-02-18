/*
 * boolarray.h
 * Copyright (C) 2017 - Pavel Nikitin
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef BOOL_ARRAY_H_
#define BOOL_ARRAY_H_

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/**
 * SECTION: boolarray
 * @title: boolarray
 * @short_description: A boolean array
 * 
 * A boolean array as dynamic array of unsigned long integers.
 */

typedef size_t word_t;

#if __WORDSIZE == 64
#define ARRAY_INDEX(index) (index >> 6)
#define BIT_INDEX(index)   (index &  63)
#define BIT_MASK(index)    (1UL   << BIT_INDEX(index))
#else
#define ARRAY_INDEX(index) (index >> 5)
#define BIT_INDEX(index)   (index &  31)
#define BIT_MASK(index)    (1U    << BIT_INDEX(index))
#endif

/**
 * bool_array_new:
 * @n_bools: Number of booleans in the array
 *
 * Creates a new boolean array and initializes it by zeros.
 *
 * Returns: Boolean array
 */
word_t *
bool_array_new (int n_bools);

/**
 * bool_array_new_string:
 * @string:         Source string
 * @n_bools: (out): Number of booleans in the array
 * 
 * Creates a new boolean array from string.
 * 
 * Returns: Boolean array
 */
word_t *
bool_array_new_string (const char *string,
                       int        *n_bools);

/**
 * bool_array_get:
 * @array: Boolean array
 * @index: Index of boolean in the boolean array
 * 
 * Gets boolean by its index in the boolean array.
 * 
 * Returns: Boolean value
 */
bool
bool_array_get (word_t *array,
                int     index);

/**
 * bool_array_set:
 * @array: A boolean array
 * @index: Index of boolean in a boolean array
 * @value: Boolean value to set
 * 
 * Sets boolean by its index in the boolean array.
 */
void
bool_array_set (word_t *array,
                int     index,
                bool    value);

/**
 * bool_array_xor:
 * @array: A boolean array
 * @index: Index of boolean in the boolean array
 * @value: Boolean value to xor
 * 
 * Xors boolean by its index in the boolean array.
 */
void
bool_array_xor (word_t *array,
                int     index,
                bool    value);

/**
 * bool_array_n_words:
 * @n_bools: Number of booleans in the array
 * 
 * Gets number of processor words required to store @n bools.
 * 
 * Returns: Number of processor words
 */
int
bool_array_n_words (int n_bools);

/**
 * bool_array_count:
 * @array:   Boolean array
 * @n_words: Number of processor words in the boolean array
 * 
 * Counts number of ones in the boolean array by number of processor words.
 * 
 * Returns:  Number of ones in the boolean array
 */
int
bool_array_count (word_t *array,
                  int     n_words);

/**
 * bool_array_clear:
 * @array:   Boolean array
 * @n_words: Number of processor words in the boolean array
 * 
 * Zeros the boolean array by number of processor words.
 */
void
bool_array_clear (word_t *array,
                  int     n_words);

/**
 * bool_array_string:
 * @array: Boolean array.
 * @n_bools: Number of booleans in the array
 * 
 * Converts a boolean array to string.
 * 
 * Returns: A string
 */
char *
bool_array_string (word_t *array,
                   int     n_bools);

#endif
