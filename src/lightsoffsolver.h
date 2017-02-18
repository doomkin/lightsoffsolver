/*
 * lightsoffsolver.h
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

#ifndef LIGHTSOFF_SOLVER_H_
#define LIGHTSOFF_SOLVER_H_

#include "boolgauss.h"

/**
 * SECTION: lightsoffsolver
 * @title: lightsoffsolver
 * @short_description: The program solves the puzzle Lights Off.
 *
 * The program solves the puzzle Lights Off. It is a puzzle game, where the
 * objective is to turn off all of the tiles on the board. Each click toggles
 * the state of the clicked tile and its non-diagonal neighbors.
 */

/**
 * lightsoff_solve:
 * @field:              The puzzle field as the boolean matrix
 * @n_rows:             Number of rows in the field
 * @n_cols:             Number of columns in the field
 * @n_solutions: (out): Number of all solutions
 * @min_weight:  (out): The weight of solution as number of ones
 * @progress_sign:      Shows progress bar
 *
 * Solves a puzzle Lights Off.
 * 
 * Returns: The solution as the boolean matrix
 **/
word_t **
lightsoff_solve (word_t **field,
                 int      n_rows,
                 int      n_cols,
                 int     *n_solutions,
                 int     *min_weight,
                 bool     progress_sign);

/**
 * lightsoff_apply:
 * @field:    The puzzle Lights Off field to apply solution as the boolean matrix
 * @solution: The solution to apply as the boolean matrix
 * @n_rows:   Number of rows in the field
 * @n_cols:   Number of columns in the field
 * 
 * Applies the solution to the puzzle Lights Off field
 **/
void
lightsoff_apply (word_t **field,
                 word_t **solution,
                 int      n_rows,
                 int      n_cols);

#endif
