/*
 * boolmatrix.c
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

#include "boolmatrix.h"

/*
 * Creates boolean matrix and zeros it.
 */
word_t **
bool_matrix_new (int n_rows,
                 int n_cols)
{
  int      n_words, i;
  word_t **matrix = malloc (n_rows * sizeof *matrix);

  if (matrix != NULL)
    {
      n_words = bool_array_n_words (n_cols);
      for (i = 0; i < n_rows; i++)
        {
          matrix[i] = calloc (n_words, sizeof *matrix[i]);
          if (matrix[i] == NULL)
            {
              bool_matrix_free (matrix, i - 1);
              matrix = NULL;
              break;
            }
        }
    }

  return matrix;
}

/*
 * Adds empty rows in the boolean matrix. If number of lines is negative,
 * then delete specified number of rows and release memory.
 */
bool
bool_matrix_add_rows (word_t ***matrix,
                      int       n_rows,
                      int       n_add)
{
  bool     success = true;
  int      i;
  word_t **re_matrix;

  /* Add n_add rows */
  if (n_add > 0)
    {
      if (*matrix == NULL)
        re_matrix = malloc (n_add * sizeof (**matrix));
      else
        re_matrix = realloc (*matrix, (n_rows + n_add) * sizeof (**matrix));

      if (re_matrix != NULL)
        {
          *matrix = re_matrix;
          for (i = n_rows; i < n_rows + n_add; i++)
            (*matrix)[i] = NULL;
        }
      else
        success = false;
    }
  /* Delete last -n_add rows */
  else
    {
      for (i = n_rows - 1; i >= n_rows + n_add; i--)
        {
          if ((*matrix)[i] != NULL)
            free ((*matrix)[i]);
        }
    }

  return success;
}

/*
 * Releases a boolean matrix.
 */
void
bool_matrix_free (word_t **matrix,
                  int      n_rows)
{
  int i;

  for (i = 0; i < n_rows; i++)
    {
      if (matrix[i] != NULL)
        free (matrix[i]);
    }

  if (matrix != NULL)
    free (matrix);
}

/*
 * Reads a boolean matrix from a stream.
 */
static char *
read_line (FILE *stream)
{
  size_t size   = 64;
  size_t tie    = size;
  char  *line   = malloc (size);
  char  *pline  = line;
  char  *new_line;
  int    symbol;

  if (line == NULL)
    return NULL;

  for (;;)
    {
      symbol = fgetc (stream);
      if (symbol == EOF)
        break;

      if (--tie == 0)
        {
          tie = size;
          new_line = realloc (pline, size *= 2);

          if (new_line == NULL)
            {
              free (pline);
              return NULL;
            }

          line = new_line + (line - pline);
          pline = new_line;
        }

      if ((*line++ = symbol) == '\n')
        break;
    }

  *line = '\0';

  return pline;
}

/*
 * Converts a boolean matrix to string.
 */
word_t **
bool_matrix_read (FILE *stream,
                  int  *n_rows,
                  int  *n_cols)
{
  bool     end_line = false;
  bool     success  = true;
  char    *line     = NULL;
  word_t **matrix   = NULL;
  word_t  *matrix_row;
  *n_rows = 0;
  *n_cols = 0;

  do
    {
      line = read_line (stream);
      end_line = line == NULL || *line == '\0' || *line == '\n';

      if (!end_line)
        {
          success = bool_matrix_add_rows (&matrix, *n_rows, 1);
          if (success)
            {
              matrix_row = bool_array_new_string (line, n_cols);
              success = matrix_row != NULL;
              if (success)
                matrix[(*n_rows)++] = matrix_row;
            }
        }

      free (line);
    }
  while (!end_line && success);

  if (!success)
    {
      bool_matrix_free (matrix, *n_rows);
      matrix = NULL;
    }

  return matrix;
}

/*
 * Converts a boolean matrix to string.
 */
char *
bool_matrix_string (word_t **matrix,
                    int      n_rows,
                    int      n_cols)
{
  int   row_len = n_cols + 1;
  char *str     = malloc (n_rows * row_len + 1);
  char *prow    = str;
  char *row_str;
  int   i;

  for (i = 0; i < n_rows; i++)
    {
      row_str = bool_array_string (matrix[i], n_cols);
      memcpy (prow, row_str, n_cols);
      *(prow + n_cols) = '\n';
      free (row_str);
      prow += row_len;
    }
  *prow = '\0';

  return str;
}

/**
 * Prints a boolean matrix into console.
 */
void
bool_matrix_print (word_t **matrix,
                   int      n_rows,
                   int      n_cols)
{
  char *matrix_str = bool_matrix_string (matrix, n_rows, n_cols);
  printf ("%s\n", matrix_str);
  free (matrix_str);
}
