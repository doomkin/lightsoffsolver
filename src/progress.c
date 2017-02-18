/*
 * progress.c
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

#include <stdio.h>
#include <string.h>
#include "progress.h"

static int prev_percent = -1;

/*
 * Shows progress bar into console. Does not supports re-entry.
 */
void
show_progress (const char *message,
               int         percent)
{
  int n_spaces, i, len;

  if (prev_percent == percent)
    {
      return;
    }

  prev_percent = percent;
  printf ("%s", message);
  n_spaces = PROGRESS_MESSAGE_WIDTH - strlen (message);
  for (i = 0; i < n_spaces; i++)
    {
      printf (" ");
    }
  printf ("[");
  len = percent * PROGRESS_BAR_LENGTH / 100;
  for (i = 0; i < len; i++)
    {
      printf ("#");
    }
  len = PROGRESS_BAR_LENGTH - len;
  for (i = 0; i < len; i++)
    {
      printf (" ");
    }
  printf ("] %i%c\r", percent, '%');

  if (percent == 100)
    {
      prev_percent = -1;
      printf ("\n");
    }
}

