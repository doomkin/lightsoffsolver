/*
 * boolmatrix.vala
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

/*
 * Class BoolMatrix implements a matrix of booleans. Each value occupies one bit.
 */
class BoolMatrix : GLib.Object
{
  public BoolArray[] matrix = null;
  public int n_rows { construct set; get; }
  public int n_cols { construct set; get; }

  public BoolMatrix () {}

  public BoolMatrix.size (int n_rows, int n_cols)
  {
    Object (n_rows: n_rows, n_cols: n_cols);
    for (int i = 0; i < n_rows; i++)
      matrix[i] = new BoolArray.size (n_cols);
  }

  public BoolMatrix.copy (BoolMatrix source)
  {
    Object (n_rows: source.n_rows, n_cols: source.n_cols);
    for (int i = 0; i < n_rows; i++)
      matrix[i] = new BoolArray.copy (source[i]);
  }

  construct
  {
    if (n_rows > 0)
      matrix = new BoolArray[n_rows];
  }

  public new BoolArray get (int row)
    requires (row < n_rows)
  {
    return matrix[row];
  }

  public new void set (int row, BoolArray value)
    requires (row < n_rows)
  {
    matrix[row] = value;
  }

  public new string to_string ()
  {
    string result = "";
    for (int i = 0; i < n_rows; i++)
      result += matrix[i].to_string () + "\n";
    return result;
  }
}
