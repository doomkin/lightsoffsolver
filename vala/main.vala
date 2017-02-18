/*
 * main.vala
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

int main (string[] args)
{
  bool print_time;
  var field = new BoolMatrix ();
  var n_cols = 0;
  string line = null;

  if (args.length == 1)
    print_time = false;
  else if (args.length == 2 && args[1] == "-i")
    print_time = true;
  else
    {
      print_usage ();
      return 1;
    }

  do
    {
      line = read_line ();
      if (line != null && line != "\n")
        {
          if (n_cols == 0)
            n_cols = line.length - 1;
          else if (n_cols != line.length - 1)
            {
              stderr.printf ("Row size %d != %d in first row.\n",
                line.length, n_cols);
              return 1;
            }

          var field_row = new BoolArray.size (n_cols);

          for (var i = 0; i < n_cols; i++)
            {
              var symbol = line.substring (i, 1);

              if (symbol == "1")
                field_row[i] = true;
              else if (symbol == "0")
                field_row[i] = false;
              else
                {
                  stderr.printf ("Each character must be a 0 or 1\n");
                  return 1;
                }
            }
          field.matrix += field_row;
        }
    }
  while (line != null && line != "\n");

  field.n_rows = field.matrix.length;
  field.n_cols = field.matrix[0].length;
  var start = new DateTime.now_local ();
  BoolMatrix? solution = solve_lightsoff (field);
  var end = new DateTime.now_local ();

  if (solution != null)
    stdout.printf (@"$solution");
  else
    stdout.printf ("0\n");

  if (print_time)
    {
      TimeSpan timestamp = end.difference (start);
      stdout.printf (@"$timestamp\n");
    }
  return 0;
}

static string? read_line ()
{
  var line = new StringBuilder ();
  var buffer = new char[256];

  while (stdin.eof () || !line.str.has_suffix ("\n"))
    {
      string chunk = stdin.gets (buffer);
      if (chunk != null)
        line.append (chunk);
      else
        return null;
    }

  return line.str;
}

static void print_usage ()
{
  stdout.printf ("""The program solves the puzzle Lights Off.
This program is free software. See http://www.gnu.org/copyleft/gpl.html the license.

Usage: lightsoff-solver [-i]
010
111
010

""");
}
