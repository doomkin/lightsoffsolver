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

//! Implements Gauss algorithm for `n_rows` logical equations and `n_cols-1`
//! variables.    

use ::bitmat::BitMat;
use ::bitvec::BitVec;

/// Implements Gauss algorithm for `n_rows` logical equations and `n_cols-1`
/// variables.    
pub struct BitGauss {
    sys: BitMat,
    rank: usize,
}

impl BitGauss {
    /// Creates boolean gauss algorithm with specified size of system of logical
    /// equations. System contains `row` equations with `col-1` variables.
    #[inline]
    pub fn with(sys: BitMat) -> BitGauss {
        BitGauss {
            sys: sys,
            rank: 0,
        }
    }
        
    /// Returns an immutable `sys`.
    #[inline]
    pub fn sys(&self) -> &BitMat {
        &self.sys
    }
    
    /// Returns a mutable `sys`.
    #[inline]
    pub fn sys_mut(&mut self) -> &mut BitMat {
        &mut self.sys
    }
    
    /// Returns a `rank` of system.
    #[inline]
    pub fn rank(&self) -> usize {
        self.rank
    }
    
    /// Gausses system with `n_rows` logical equations and `n_cols-1` variables.
    ///
    /// # Examples
    ///
    /// ```
    /// use los::bitalg::BitGauss;
    ///
    /// let n = 3;
    /// let mut alg = BitGauss::with_size(n, n+1);
    ///
    /// for i in 0..n {
    ///     alg.sys_mut().set(i, n-i-1, true);
    ///     alg.sys_mut().set(i, n, true);
    /// }
    ///
    /// assert_eq!(alg.sys().to_string(), "0011\n0101\n1001\n");
    /// alg.gauss();    
    /// assert_eq!(alg.sys().to_string(), "1001\n0101\n0011\n");
    /// assert_eq!(alg.rank(), n);
    /// ```
    pub fn gauss(&mut self) {
        let n_rows = self.sys.n_rows();
      
        // Convert the left square matrix to a identity matrix
        for i in 0..n_rows {
            // Find and set one on the main diagonal
            for j in i..n_rows {
                if self.sys.get(j, i) {
                    self.sys.swap(j, i);
                }
            }

            // Skip column, if it does not contains `true`
            if !self.sys.get(i, i) {
                continue;
            }

            // Increase rank: the column contains `true` on the main diagonal
            self.rank = i+1;

            // Set values to `false` at column except the main diagonal
            for j in 0..n_rows {
                if self.sys.get(j, i) && j != i {
                    self.sys.xor(j, i);
                }
            }
        }
    }

    // Finds shortest solution in the system.
    pub fn solve(&mut self) -> Option<BitVec> {
        // Gauss system
        self.gauss();
        
        // Check the system for inconsistency
        let n_rows = self.sys.n_rows();
        let n_cols = self.sys.n_cols();
        let n_vars = n_cols-1;
        let rank = self.rank;
        
        for i in rank..n_rows {
            if self.sys.get(i, n_vars) {
                return None
            }
        }

        // Create the solution vector
        let mut solution = BitVec::with_length(n_vars);

        // The system has one solution
        if rank == n_vars {
            for i in 0..n_rows {
                solution.set(i, self.sys.get(i, n_vars));
            }
        }
        // The system has 2^(n_vars-rank) solutions
        else {
            let n_rest = n_vars - rank;
            let n_solutions = 1usize << n_rest;
            let mut min_weight = n_cols as u32;
            let mut weight;
            let mut rest = BitVec::with_length(n_rest);
            let mut accumulator = BitVec::with_length(n_rows);

            // Find a solution with a minimum number of ones
            for i in 0..n_solutions {
                // Create subset by index i
                rest.buf_mut()[0] = i;
                
                // Reset accumulator
                accumulator.setall(false);

                // Accumulate the solution with index i
                for j in 0..rank {
                    for k in 0..n_rest {
                        if rest.get(k) {
                            accumulator.xor(j, self.sys.get(j, rank + k));
                        }
                    }
                    accumulator.xor(j, self.sys.get(j, n_vars));
                }

                // Weigh out the solution with index i
                weight = rest.count_ones() + accumulator.count_ones();

                // Build the solution with less weight
                if weight < min_weight {
                    min_weight = weight;

                    // Get first part of elemets of solution from accumulator
                    for j in 0..rank {
                        solution.set(j, accumulator.get(j));
                    }

                    // Get second part of elemets of solution from rest
                    for j in 0..n_rest {
                        solution.set(rank + j, rest.get(j));
                    }
                }
            }
        }

        Some(solution)
    }
}
