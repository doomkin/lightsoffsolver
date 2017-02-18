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

#ifndef BOOL_MATRIX_H_
#define BOOL_MATRIX_H_

#include <stdio.h>
#include "boolarray.h"

/**
 * SECTION: boolmatrix
 * @title: boolmatrix
 * @short_description: A boolean matrix
 * 
 * A boolean matrix as dynamic array of boolean arrays.
 */

/**
 * bool_matrix_new:
 * @n_rows: Number of equations
 * @n_cols: Number of variables with right part of system
 * 
 * Creates boolean matrix and zeros it.
 * 
 * Returns: A new boolean matrix
 */
word_t **
bool_matrix_new (int n_rows,
                 int n_cols);

/**
 * bool_matrix_add_rows:
 * @matrix: (inout): A boolean matrix
 * @n_rows:          Number of rows
 * @n_add:           Number of rows to add
 * 
 * Adds empty rows in the boolean matrix. If number of lines is negative,
 * then delete specified number of rows and release memory.
 * 
 * Returns: A success flag
 */
bool
bool_matrix_add_rows (word_t ***matrix,
                      int       n_rows,
                      int       n_add);

/**
 * bool_matrix_add_rows:
 * @matrix: A boolean matrix
 * @n_rows: Number of rows
 *
 * Releases a boolean matrix.
 */
void
bool_matrix_free (word_t **matrix,
                  int      n_rows);

/**
 * bool_matrix_read:
 * @stream:        A stream to read as #FILE
 * @n_rows: (out): Number of rows
 * @n_cols: (out): Number of columns
 * 
 * Reads a boolean matrix from a stream.
 * 
 * Returns: A boolean matrix
 */
word_t **
bool_matrix_read (FILE *stream,
                  int  *n_rows,
                  int  *n_cols);

/**
 * bool_matrix_string:
 * @matrix: A boolean matrix
 * @n_rows: Number of rows
 * @n_cols: Number of columns
 * 
 * Converts a boolean matrix to string.
 * 
 * Returns: A string
 */
char *
bool_matrix_string (word_t **matrix,
                    int      n_rows,
                    int      n_cols);

/**
 * bool_matrix_print:
 * @matrix: A boolean matrix
 * @n_rows: Number of rows
 * @n_cols: Number of columns
 *
 * Prints a boolean matrix into console.
 */
void
bool_matrix_print (word_t **matrix,
                   int      n_rows,
                   int      n_cols);

#endif
