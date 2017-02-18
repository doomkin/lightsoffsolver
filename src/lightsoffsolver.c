/*
 * lightsoffsolver.c
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

#include "lightsoffsolver.h"

/*
 * Calculates of the index in the system matrix by row and column in the field.
 * Returns -1 if out of field range.
 */
static int
matrix_index (int row,
              int col,
              int n_rows,
              int n_cols)
{
  return (0 <= row && row < n_rows && 0 <= col && col < n_cols) ?
         n_cols * row + col : -1;
}

/*
 * Creates a system of logical equations by puzzle field.
 */
static word_t **
create_system (word_t **field,
               int      n_rows,
               int      n_cols)
{
  int      n      = n_rows * n_cols;
  word_t **system = bool_matrix_new (n, n + 1);
  int      himself, neighbor, row, col;

  for (row = 0; row < n_rows; row++)
    {
      for (col = 0; col < n_cols; col++)
        {
          himself = matrix_index (row, col, n_rows, n_cols);
          bool_array_set (system[himself], himself, true);

          neighbor = matrix_index (row, col + 1, n_rows, n_cols);
          if (neighbor >= 0)
            bool_array_set (system[himself], neighbor, true);

          neighbor = matrix_index (row, col - 1, n_rows, n_cols);
          if (neighbor >= 0)
            bool_array_set (system[himself], neighbor, true);

          neighbor = matrix_index (row + 1, col, n_rows, n_cols);
          if (neighbor >= 0)
            bool_array_set (system[neighbor], himself, true);

          neighbor = matrix_index (row - 1, col, n_rows, n_cols);
          if (neighbor >= 0)
            bool_array_set (system[neighbor], himself, true);

          bool_array_set (system[himself], n, bool_array_get (field[row], col));
        }
    }

  return system;
}

/*
 * Solves a puzzle Lights Off.
 */
word_t **
lightsoff_solve (word_t **field,
                 int      n_rows,
                 int      n_cols,
                 int     *n_solutions,
                 int     *min_weight,
                 bool     progress_sign)
{
  int      n        = n_rows * n_cols;
  word_t **system   = create_system (field, n_rows, n_cols);
  int      rank     = bool_gauss (system, n, n + 1, progress_sign);
  word_t  *solution = find_shortest_solution (system, n, n + 1, rank);
  word_t **result   = NULL;
  int      row, col;

  *n_solutions = (solution == NULL) ? 0 : 1 << (n - rank);
  *min_weight = bool_array_count (solution, bool_array_n_words (n));

  if (solution != NULL)
    {
      result = bool_matrix_new (n_rows, n_cols);
      for (row = 0; row < n_rows; row++)
        {
          for (col = 0; col < n_cols; col++)
            {
              if (bool_array_get (solution, n_cols * row + col))
                bool_array_set (result[row], col, true);
            }
        }
      free (solution);
    }

  bool_matrix_free (system, n);

  return result;
}

/*
 * Applies the solution to the puzzle Lights Off field.
 */
void
lightsoff_apply (word_t **field,
                 word_t **solution,
                 int      n_rows,
                 int      n_cols)
{
  int row, col;
  for (row = 0; row < n_rows; row++)
    {
      for (col = 0; col < n_cols; col++)
        {
          if (bool_array_get (solution[row], col))
            {
              bool_array_xor (field[row], col, true);

              if (col < n_cols - 1)
                bool_array_xor (field[row], col + 1, true);

              if (col > 0)
                bool_array_xor (field[row], col - 1, true);

              if (row < n_rows - 1)
                bool_array_xor (field[row + 1], col, true);

              if (row > 0)
                bool_array_xor (field[row - 1], col, true);
            }
        }
    }
}
