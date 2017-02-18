/*
 * boolgauss.c
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

#include "boolgauss.h"
#include "progress.h"

/*
 * Gausses system with @n_rows logical equations and @n_cols-1 variables.
 */
int
bool_gauss (word_t **system,
            int      n_rows,
            int      n_cols,
            bool     progress_sign)
{
  int     rank    = 0;
  int     n_words = bool_array_n_words (n_cols);
  int     i, j, k;
  word_t *swap;

  /* Convert the left square matrix to a identity matrix */
  for (i = 0; i < n_rows; i++)
    {
      /* Find and set one on the main diagonal */
      for (j = i; j < n_rows; j++)
        {
          if (bool_array_get (system[j], i))
            {
              swap = system[j];
              system[j] = system[i];
              system[i] = swap;
              break;
            }
        }

      /* Skip column, if it does not contain one */
      if (! bool_array_get (system[i], i))
        continue;

      rank = i + 1;

      /* Zero column except the main diagonal */
      for (j = 0; j < n_rows; j++)
        {
          if (bool_array_get (system[j], i) && j != i)
            {
              for (k = 0; k < n_words; k++)
                system[j][k] ^= system[i][k];
            }
        }

      /* Refresh progress bar */
      if (progress_sign)
        show_progress ("Gaussing system", (i + 1) * 100 / n_rows);
    }

  return rank;
}

/*
 * Finds shortest solution in the gaussed system.
 */
word_t *
find_shortest_solution (word_t **system,
                        int      n_rows,
                        int      n_cols,
                        int      rank)
{
  int     n_vars  = n_cols - 1;
  int     n_words = bool_array_n_words (n_rows);
  int     n_remn, n_solutions, min_weight, weight, i, j, k;
  word_t *solution, *remn, *sum;

  /* Check the system for inconsistency */
  for (i = rank; i < n_rows; i++)
    {
      if (bool_array_get (system[i], n_vars))
        return NULL;
    }

  solution = bool_array_new (n_vars);

  /* The system has one solution */
  if (rank == n_vars)
    {
      for (i = 0; i < n_rows; i++)
        bool_array_set (solution, i, bool_array_get (system[i], n_vars));
    }
  /* The system has 2^(n_vars-rank) solutions */
  else
    {
      n_remn = n_vars - rank;
      n_solutions = 1 << n_remn;
      min_weight = n_cols;
      remn = bool_array_new (n_remn);
      sum = bool_array_new (n_rows);

      /* Find a solution with a minimum number of ones */
      for (i = 0; i < n_solutions; i++)
        {
          remn[0] = i;
          bool_array_clear (sum, n_words);

          /* Sum a solution with index i */
          for (j = 0; j < rank; j++)
            {
              for (k = 0; k < n_remn; k++)
                {
                  if (bool_array_get (remn, k))
                    bool_array_xor (sum, j, bool_array_get (system[j], rank + k));
                }
              bool_array_xor (sum, j, bool_array_get (system[j], n_vars));
            }

          /* Weigh out a solution with index i */
          weight = bool_array_count (remn, 1) + bool_array_count (sum, n_words);

          /* Construct a solution with less weight */
          if (weight < min_weight)
            {
              min_weight = weight;

              /* First elemets of solution get from sum */
              for (j = 0; j < rank; j++)
                bool_array_set (solution, j, bool_array_get (sum, j));

              /* Rest elemets of solution get from remnant */
              for (j = 0; j < n_remn; j++)
                bool_array_set (solution, rank + j, bool_array_get (remn, j));
            }
        }

      free (remn);
      free (sum);
    }

  return solution;
}
