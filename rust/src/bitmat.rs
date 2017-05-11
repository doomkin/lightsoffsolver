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

//! A contiguous growable boolean matrix type with heap-allocated contents.
//! The rows are represented by a vector of references to `BitVec`.

use std::ptr;
use std::fmt;
use std::ops::{Index, IndexMut};
use ::bitvec::BitVec;

/// A contiguous growable boolean matrix type with heap-allocated contents.
/// The rows are represented by a vector of references to `BitVec`.
pub struct BitMat {
    rows: Vec<BitVec>
}

////////////////////////////////////////////////////////////////////////////////
// Inherent methods
////////////////////////////////////////////////////////////////////////////////

impl BitMat {
    /// Constructs a new empty `BitMat`.
    ///
    /// # Examples
    ///
    /// ```
    /// use los::bitmat::BitMat;
    ///
    /// let mat = BitMat::new();
    /// assert_eq!(mat.n_rows(), 0);
    /// assert_eq!(mat.n_cols(), 0);
    /// ```
    #[inline]
    pub fn new() -> BitMat {
        BitMat {
            rows: vec![]
        }
    }

    /// Constructs a new `BitMat` with the specified `n_rows` number of rows.
    /// Each row are presents by a `BitVec` with specified `n_cols`
    /// length. If `n_cols` == `0` then creates an empty `BitVec`.
    ///
    /// # Examples
    ///
    /// ```
    /// use los::bitmat::BitMat;
    ///
    /// // Create 10x10 boolean matrix
    /// let mut mat = BitMat::with_size(10, 10);
    /// assert_eq!(mat.n_rows(), 10);
    /// assert_eq!(mat.n_cols(), 10);
    /// ```
    #[inline]
    pub fn with_size(n_rows: usize, n_cols: usize) -> BitMat {
        let mut rows: Vec<BitVec> = vec![];
        rows.resize(n_rows, BitMat::new_row(n_cols));
        BitMat {
            rows: rows
        }
    }
    
    /// Returns number of rows.
    #[inline]
    pub fn n_rows(&self) -> usize {
        self.rows.len()
    }

    /// Returns number of columns.
    #[inline]
    pub fn n_cols(&self) -> usize {
        match self.n_rows() {
            0 => 0,
            _ => self.rows[0].len()
        }
    }

    /// Returns an immutable collection of rows.
    ///
    /// # Examples
    ///
    /// ```
    /// use los::bitvec::BitVec;
    /// use los::bitmat::BitMat;
    ///
    /// // Create an empty boolean matrix
    /// let mat = BitMat::new();
    /// assert_eq!(mat.rows().len(), 0);
    /// ```
    #[inline]
    pub fn rows(&self) -> &Vec<BitVec> {
        &self.rows
    }
    
    /// Returns an mutable collection of rows.
    ///
    /// # Examples
    ///
    /// ```
    /// use los::bitvec::BitVec;
    /// use los::bitmat::BitMat;
    ///
    /// // Create 10x0 boolean matrix
    /// let mut mat = BitMat::new();
    /// mat.rows_mut().resize(10, BitMat::new_row(0));
    /// assert_eq!(mat.n_rows(), 10);
    /// assert_eq!(mat.n_cols(), 0);
    /// ```
    #[inline]
    pub fn rows_mut(&mut self) -> &mut Vec<BitVec> {
        &mut self.rows
    }
    
    /// Returns a row as a `BitVec` with `n_cols` elements.
    ///
    /// # Examples
    ///
    /// ```
    /// use los::bitvec::BitVec;
    /// use los::bitmat::BitMat;
    ///
    /// // Create 1x10 boolean matrix
    /// let mut mat = BitMat::new();
    /// mat.rows_mut().push(BitMat::new_row(10));
    /// assert_eq!(mat.n_rows(), 1);
    /// assert_eq!(mat.n_cols(), 10);
    /// ```
    #[inline]
    pub fn new_row(n_cols: usize) -> BitVec {
        BitVec::with_length(n_cols)
    }

    /// Swaps in-place row by index `row_i` with row by index `row_j`.
    ///
    /// # Examples
    ///
    /// ```
    /// use los::bitmat::BitMat;
    ///
    /// let n = 3;
    /// let mut mat = BitMat::with_size(n, n);
    ///
    /// for i in 0..n {
    ///     mat.set(i, n-i-1, true);
    /// }
    ///
    /// assert_eq!(mat.to_string(), "001\n010\n100\n");
    /// mat.swap(0, n-1);    
    /// assert_eq!(mat.to_string(), "100\n010\n001\n");
    #[inline]
    pub fn swap(&mut self, row_i: usize, row_j: usize) {
        unsafe {
            let p_row_i = &mut self.rows[row_i] as *mut BitVec;
            let p_row_j = &mut self.rows[row_j] as *mut BitVec;
            ptr::swap(p_row_i, p_row_j);
        }
    }

    /// Xors in-place row by index `row_i` with row by index `row_j`.
    #[inline]
    pub fn xor(&mut self, row_i: usize, row_j: usize) {
        unsafe {
            let p_row_j = self.rows[row_j].buf() as *const Vec<usize>;
            let len = self.rows[row_j].buf().len();

            for i in 0..len {
                self.rows[row_i].buf_mut()[i] ^= (&*p_row_j)[i];
            }
        }
    }
    
    /// Sets `value` by index `row` and index `col`
    #[inline]
    pub fn set(&mut self, row: usize, col: usize, value: bool) {
        self.rows_mut()[row].set(col, value);
    }

    /// Gets value by index `row` and index `col`
    #[inline]
    pub fn get(&self, row: usize, col: usize) -> bool {
        self.rows()[row].get(col)
    }
}

////////////////////////////////////////////////////////////////////////////////
// Common trait implementations for BitMat
////////////////////////////////////////////////////////////////////////////////

/// The `Index` trait is used to specify the functionality of indexing operations
/// like `container[index]` when used in an immutable context.
///
/// # Examples
///
/// ```
/// use los::bitmat::BitMat;
///
/// let mat = BitMat::with_size(10, 0);
/// assert_eq!(mat[0].len(), 0);
/// ```
impl Index<usize> for BitMat {
    type Output = BitVec;

    #[inline]
    fn index(&self, index: usize) -> &BitVec {
        &(*self.rows)[index]
    }
}

/// The `IndexMut` trait is used to specify the functionality of indexing
/// operations like `container[index]` when used in a mutable context.
///
/// # Examples
///
/// ```
/// use los::bitmat::BitMat;
///
/// let mut mat = BitMat::with_size(1, 0);
/// mat[0].resize(10);
/// assert_eq!(mat[0].get(9), false);
/// ```
impl IndexMut<usize> for BitMat {
    #[inline]
    fn index_mut(&mut self, index: usize) -> &mut BitVec {
        &mut (*self.rows)[index]
    }
}

/// The fmt::Display trait is used to specify the to_string() functionality.
///
/// # Examples
///
/// ```
/// use los::bitmat::BitMat;
///
/// let mut m = BitMat::with_size(2, 2);
/// m[0].set(0, true);
/// m[1].set(1, true);
/// assert_eq!(m.to_string(), "10\n01\n");
/// ```
impl fmt::Display for BitMat {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        let mut strmat = String::new();
        
        for r in self.rows() {
            strmat += &*(r.to_string());
            strmat += "\n";
        }
        
        write!(f, "{}", strmat)
    }
}

