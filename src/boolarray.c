/*
 * boolarray.c
 *
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

#include "boolarray.h"

/*
 * Creates a new boolean array and initializes it by zeros.
 */
word_t *
bool_array_new (int n_bools)
{
  return calloc (bool_array_n_words (n_bools), sizeof (word_t));
}

/*
 * Creates a new boolean array from string.
 */
word_t *
bool_array_new_string (const char *string,
                       int        *n_bools)
{
  word_t *array = NULL;
  int     i;
  
  *n_bools = strlen (string);
  if (string[*n_bools - 1] == '\n')
    (*n_bools)--; 
  
  if (*n_bools > 0)
    {
      array = malloc (*n_bools * sizeof (word_t));
      if (array != NULL)
        {
          for (i = 0; i < *n_bools; i++)
            bool_array_set (array, i, string[i] == '1');
        }
    }

  return array;
}

/*
 * Gets boolean by its index in the boolean array.
 */
bool
bool_array_get (word_t *array,
                int     index)
{
  return (array[ARRAY_INDEX(index)] >> BIT_INDEX(index)) & 1;
}

/*
 * Sets boolean by its index in the boolean array.
 */
void
bool_array_set (word_t *array,
                int     index,
                bool    value)
{
  if (value)
    array[ARRAY_INDEX(index)] |= BIT_MASK(index);
  else
    array[ARRAY_INDEX(index)] &= ~BIT_MASK(index);
}

/*
 * Xors boolean by its index in the boolean array.
 */
void
bool_array_xor (word_t *array,
                int     index,
                bool    value)
{
  if (value)
    array[ARRAY_INDEX(index)] ^= BIT_MASK(index);
}

/*
 * Gets number of processor words required to store @n bools.
 */
int
bool_array_n_words (int n)
{
  return ARRAY_INDEX(n) + (BIT_INDEX(n) == 0 ? 0 : 1);
}

/*
 * Counts number of ones in the boolean array by number of processor words.
 */
int
bool_array_count (word_t *array,
                  int     n_words)
{
  int    count = 0;
  int    i;
  word_t word;

  for (i = 0; i < n_words; i++)
    {
      word = array[i];
      while (word)
        {
          word &= (word - 1);
          count++;
        }
    }

  return count;
}

/*
 * Zeros the boolean array by number of processor words.
 */
void
bool_array_clear (word_t *array,
                  int     n_words)
{
  int i;
  for (i = 0; i < n_words; i++)
    array[i] = 0;
}

/*
 * Converts the boolean array to string.
 */
char *
bool_array_string (word_t *array,
                   int     n_bools)
{
  int   i;
  char *str = malloc (n_bools + 1);

  if (str != NULL)
    {
      for (i = 0; i < n_bools; i++)
        str[i] = bool_array_get (array, i) ? '1' : '0';

      str[n_bools] = '\0';
    }

  return str;
}
