// Copyright (C) 2017 - Pavel Nikitin
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.

//! Solves "Lights off" pazzle.

use ::bitmat::BitMat;
use ::bitalg::BitGauss;

/// Solves "Lights Off" pazzle.
pub struct LightsSolver {
    alg: BitGauss,
    n_rows: usize,
    n_cols: usize,
    n_solutions: usize,
    min_weight: usize,
}

impl LightsSolver {
    /// Creates "Lights Off" solver with specified field.
    pub fn with(field: &BitMat) -> LightsSolver {
        let n_rows = field.n_rows() as isize;
        let n_cols = field.n_cols() as isize;
        let n = n_rows * n_cols;
        let mut himself: isize;
        let mut neighbor: isize;
        let mut sys = BitMat::with_size(n as usize, (n + 1) as usize);

        for row in 0..n_rows {
            for col in 0..n_cols {
                himself = sys_index(row, col, n_rows, n_cols);
                sys.set(himself as usize, himself as usize, true);

                neighbor = sys_index(row, col + 1, n_rows, n_cols);
                if neighbor >= 0 {
                    sys.set(himself as usize, neighbor as usize, true);
                }

                neighbor = sys_index(row, col - 1, n_rows, n_cols);
                if neighbor >= 0 {
                    sys.set(himself as usize, neighbor as usize, true);
                }

                neighbor = sys_index(row + 1, col, n_rows, n_cols);
                if neighbor >= 0 {
                    sys.set(neighbor as usize, himself as usize, true);
                }

                neighbor = sys_index(row - 1, col, n_rows, n_cols);
                if neighbor >= 0 {
                    sys.set(neighbor as usize, himself as usize, true);
                }

                sys.set(himself as usize, n as usize, field.get(row as usize, col as usize));
            }
        }

        let alg = BitGauss::with(sys);
        
        LightsSolver {
            alg: alg,
            n_rows: n_rows as usize,
            n_cols: n_cols as usize,
            n_solutions: 0,
            min_weight: 0,
        }
    }
        
    /// Returns an immutable `n_solutions`.
    #[inline]
    pub fn n_solutions(&self) -> usize {
        self.n_solutions
    }

    /// Returns an immutable `min_weight`.
    #[inline]
    pub fn min_weight(&self) -> usize {
        self.min_weight
    }

    /// Finds shortest solution in the system.
    pub fn solve(&mut self) -> Option<BitMat> {
        match self.alg.solve() {
            Some(syssol) => {
                self.n_solutions = 1usize << (self.n_rows * self.n_cols - self.alg.rank());
                self.min_weight = syssol.count_ones() as usize;

                let mut sol = BitMat::with_size(self.n_rows, self.n_cols);
                for row in 0..self.n_rows {
                    for col in 0..self.n_cols {
                        if syssol.get(self.n_cols * row + col) {
                            sol.set(row, col, true);
                        }
                    }
                }

                Some(sol)
            },
            None => None
        }
    }
}

/// Calculates of the index in the system matrix by row and column in the field.
/// Returns -1 if out of field range.
#[inline]
fn sys_index(row: isize, col: isize, n_rows: isize, n_cols: isize) -> isize {
    if 0 <= row && row < n_rows as isize && 0 <= col && col < n_cols as isize {
        n_cols * row + col
    } else {
        -1
    }
}
