/*
 * lightsoffsolver.vala
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

static int n_rows;
static int n_cols;
static int n_equations;

BoolMatrix? solve_lightsoff (BoolMatrix field)
{
  n_rows = field.n_rows;
  n_cols = field.n_cols;
  n_equations = n_rows * n_cols;
  BoolMatrix equations = create_equations (field);
  BoolArray equations_solution = null;
  solve_bool_equations (equations, out equations_solution);
  BoolMatrix solution = null;

  if (equations_solution != null)
    {
      solution = new BoolMatrix.size (n_rows, n_cols);
      for (int row = 0; row < n_rows; row++)
        {
          for (int col = 0; col < n_cols; col++)
            solution[row][col] = equations_solution[idx(row, col)];
        }
    }

  return solution;
}

static BoolMatrix create_equations (BoolMatrix field)
{
  var equations = new BoolMatrix.size (n_equations, n_equations + 1);

  for (var row = 0; row < field.n_rows; row++)
    {
      for (var col = 0; col < field.n_cols; col++)
        {
          equations[idx (row, col)][idx (row, col)] = true;

          if (idx (row, col + 1) >= 0)
            equations[idx (row, col)][idx (row, col + 1)] = true;

          if (idx (row, col - 1) >= 0)
            equations[idx (row, col)][idx (row, col - 1)] = true;

          if (idx (row + 1, col) >= 0)
            equations[idx (row + 1, col)][idx (row, col)] = true;

          if (idx (row - 1, col) >= 0)
            equations[idx (row - 1, col)][idx (row, col)] = true;

          equations[idx (row, col)][n_equations] = field[row][col];
        }
    }

  return equations;
}

static int idx (int row, int col)
{
  return (0 <= row && row < n_rows && 0 <= col && col < n_cols) ?
          row * n_cols + col :
          -1;
}
