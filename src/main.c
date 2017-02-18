/*
 * main.c
 *
 * The program solves the puzzle Lights Off. It is a puzzle game, where the
 * objective is to turn off all of the tiles on the board. Each click toggles
 * the state of the clicked tile and its non-diagonal neighbors.
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

#include <gdk-pixbuf/gdk-pixbuf.h>
#include <time.h>
#include "lightsoffsolver.h"

/*
 * Prints usage into console. 
 */ 
static void
print_usage (char *program_name)
{
  printf ("The program solves the puzzle Lights Off.\n"
          "This program is free software. See http://www.gnu.org/copyleft/gpl.html the license.\n"
          "\n"
          "Usage: %s [<Switches>] [< input.txt]\n"
          "010\n"
          "111\n"
          "010\n"
          "\n"
          "<Switches>\n"
          "  -r4 : number of rows in the field of ones\n"
          "  -c5 : number of columns in the field of ones\n"
          "  -p  : create image of solution to file \"lightsoff_4x5.png\"\n"
          "  -a  : apply solution to field of ones\n"
          "  -i  : print info: field size, number of solutions, weight of solution, time\n"
          "  -h  : print this help\n",
          program_name);
}

/*
 * Creates a new puzzle field filled by ones.
 */
word_t **
create_field (int n_rows,
              int n_cols)
{
  word_t **field = bool_matrix_new (n_rows, n_cols);
  int      i, j;

  for (i = 0; i < n_rows; i++)
    {
      for (j = 0; j < n_cols; j++)
        bool_array_set (field[i], j, true);
    }

  return field;
}

/*
 * Converts a boolean matrix to image as #GdkPixbuf.
 */
static GdkPixbuf *
bool_matrix_image (word_t **matrix,
                   int      n_rows,
                   int      n_cols)
{
  int x, y, rowstride, n_channels;
  guchar *pixels, *p;
  GdkPixbuf *pixbuf;

  pixbuf     = gdk_pixbuf_new (GDK_COLORSPACE_RGB, TRUE, 8, n_cols, n_rows);
  pixels     = gdk_pixbuf_get_pixels (pixbuf);
  rowstride  = gdk_pixbuf_get_rowstride (pixbuf);
  n_channels = gdk_pixbuf_get_n_channels (pixbuf);

  for (y = 0; y < n_rows; y++)
    {
      for (x = 0; x < n_cols; x++)
        {
          p = pixels + rowstride * y + n_channels * x;
          if (bool_array_get (matrix[y], x))
            {
              p[0] = 50;
              p[1] = 99;
              p[2] = 183;
              p[3] = 255;
            }
          else
            {
              p[0] = 226;
              p[1] = 224;
              p[2] = 233;
              p[3] = 255;
            }
        }
    }

  return pixbuf;
}

/*
 * The main program.
 */
int
main (int    argc,
      char **argv)
{
  bool       print_info   = false;
  bool       apply_mode   = false;
  bool       create_image = false;
  int        n_rows       = 0;
  int        n_cols       = 0;
  int        n_solutions  = 0;
  int        weight       = 0;
  word_t   **field        = NULL;
  word_t   **solution     = NULL;
  GError    *error        = NULL;
  GdkPixbuf *image        = NULL;
  char      *filename     = malloc (64);
  int        optind;
  clock_t    start, end;

  /* Reading command line arguments */
  for (optind = 1; optind < argc && argv[optind][0] == '-'; optind++)
    {
      switch (argv[optind][1])
        {
        case 'r':
          n_rows = atoi (&(argv[optind][2]));
          break;
        case 'c':
          n_cols = atoi (&(argv[optind][2]));
          break;
        case 'p':
          create_image = true;
          break;
        case 'a':
          apply_mode = true;
          break;
        case 'i':
          print_info = true;
          break;
        case 'h':
          print_usage (argv[0]);
          exit (EXIT_SUCCESS);
          break;
        default:
          print_usage (argv[0]);
          exit (EXIT_FAILURE);
        }
    }

  /* Setup square field if one of size is present */
  if (n_rows == 0 && n_cols > 0)
    n_rows = n_cols;

  if (n_cols == 0 && n_rows > 0)
    n_cols = n_rows;

  /* Construct the field */
  if (n_rows > 0 && n_cols > 0)
    {
      field = create_field (n_rows, n_cols);
    }
  /* Read the board state from the console to the Bool matrix */
  else
    {
      field = bool_matrix_read (stdin, &n_rows, &n_cols);
      if (field == NULL)
        {
          print_usage (argv[0]);
          exit (EXIT_FAILURE);
        }
    }

  /* Solve the puzzle */
  if (!apply_mode)
    {
      start = clock(); 
      solution = lightsoff_solve (field, n_rows, n_cols,
                                  &n_solutions, &weight,
                                  print_info);
      end = clock();

      /* Print solution to the console */
      if (solution != NULL)
        bool_matrix_print (solution, n_rows, n_cols);
      else
        printf ("0\n\n");

      if (print_info)
        {
          printf ("Size      : %i x %i\n", n_rows, n_cols);
          printf ("Solutions : %i\n",      n_solutions);
          printf ("Weight    : %i\n",      weight);
          printf ("Time      : %ld\n",     end - start);
        }
    }
  /* Apply the solution to the puzzle */
  else
    {
      solution = field;
      field = create_field (n_rows, n_cols);
      lightsoff_apply (field, solution, n_rows, n_cols);
      bool_matrix_print (field, n_rows, n_cols);
    }

  /* Save solution to image file */
  if (create_image)
    {
      /* Convert bool matrix to image */
      image = bool_matrix_image (solution, n_rows, n_cols);

      /* Save image to file */
      sprintf (filename, "lightsoff_%ix%i.png", n_rows, n_cols);
      error = NULL;
      gdk_pixbuf_savev (image, filename, "png", NULL, NULL, &error);
      if (error != NULL)
        {
          fprintf (stderr, "Unable to save file: %s\n", error->message);
          g_object_unref (error);
        }

      /* Release memory */
      g_object_unref (image);
    }

  /* Release memory */
  bool_matrix_free (field, n_rows);
  bool_matrix_free (solution, n_rows);
  free (filename);

  return 0;
}
