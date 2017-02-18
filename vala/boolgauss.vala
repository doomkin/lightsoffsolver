/*
 * boolgauss.vala
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

/**
 * Solve a system of n_rows logic system with n_cols-1 variables.
 * Returns the shortest solution and the rank of system of system.
 * Pass the copy matrix of system, if you need it after solving.
 */
int solve_bool_equations (BoolMatrix system, out BoolArray solution)
{
  int n = system.n_rows;
  int i, j = 0, k = 0, rank = 0, n_words = system[j].array.length;
  BoolArray swap = null;

  for (i = 0; i < n; i++)
    {
      for (j = i; j < n; j++)
        {
          if (system[j][i])
            {
              swap = system[j];
              system[j] = system[i];
              system[i] = swap;
              break;
            }
        }

      if (!system[i][i])
        continue;

      rank = i + 1;

      for (j = 0; j < n; j++)
        {
          if (j != i && system[j][i])
            {
              for (k = 0; k < n_words; k++)
                system[j].array[k] ^= system[i].array[k];
            }
        }
    }

  solution = null;

  int n_vars = system.n_cols - 1;

  for (i = rank; i < n; i++)
    {
      if (system[i][n_vars])
        return rank;
    }

  solution = new BoolArray.size (n_vars);
  if (rank == n_vars)
    {
      for (i = 0; i < n; i++)
        solution[i] = system[i][n_vars];
    }
  else
    {
      var n_remn = n_vars - rank;
      var n_solutions = 1u << n_remn;
      var min_weight = n;
      var remn = new BoolArray.size (n_remn);
      var sum = new BoolArray.size (n);

      for (i = 0; i < n_solutions; i++)
        {
          remn.array[0] = (uint64)i;
          sum.clear ();

          for (j = 0; j < rank; j++)
            {
              for (k = 0; k < n_remn; k++)
                {
                  if (remn[k])
                    sum.xor (j, system[j][rank + k]);
                }
              sum.xor (j, system[j][n_vars]);
            }

          var weight = remn.count() + sum.count();
          if (weight < min_weight)
            {
              min_weight = weight;
              solution.clear ();

              for (j = 0; j < rank; j++)
                solution[j] = sum[j];

              for (j = 0; j < n_remn; j++)
                solution[rank + j] = remn[j];
            }
        }
    }

  return rank;
}
