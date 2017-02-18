/*
 * boolgauss.h
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

#ifndef BOOL_GAUSS_H_
#define BOOL_GAUSS_H_

#include "boolmatrix.h"

/**
 * SECTION: boolgauss
 * @title: boolgauss
 * @short_description: Solves a logical equations by the Gauss method
 * 
 * Solves a logical equations by the Gauss method. For uncertain systems
 * searches for the minimum of ones in the solution.
 */

/**
 * bool_gauss:
 * @system:        A system of logical equations as boolean matrix
 * @n_rows:        Number of equations
 * @n_cols:        Number of variables with right part of system
 * @progress_sign: Show a progress bar
 *
 * Gausses system with @n_rows logical equations and @n_cols-1 variables.
 *
 * Returns:        The rank of system
 */
int
bool_gauss (word_t **system,
            int      n_rows,
            int      n_cols,
            bool     progress_sign);

/**
 * find_shortest_solution:
 * @system:        A system of logical equations as boolean matrix
 * @n_rows:        Number of equations
 * @n_cols:        Number of variables with right part of system
 * @rank:          A rank of system calculated by gauss method
 * 
 * Finds shortest solution in the gaussed system.
 * 
 * Returns:        A shortest solution as boolean array
 */
word_t *
find_shortest_solution (word_t **system,
                        int      n_rows,
                        int      n_cols,
                        int      rank);

#endif
