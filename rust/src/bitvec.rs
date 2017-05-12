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

//! A contiguous growable boolean array type with heap-allocated contents. Each
//! element occupies one bit. Elements are grouped into `target_pointer_width`
//! unsigned integers.
//!
//! Vectors have `O(1)` indexing, amortized `O(1)` push (to the end) and
//! `O(1)` pop (from the end).

use std::fmt;

/// A contiguous growable boolean array type with heap-allocated contents. Each
/// element occupies one bit. Elements are grouped into `target_pointer_width`
/// unsigned integers.
///
/// Vectors have `O(1)` indexing, amortized `O(1)` push (to the end) and
/// `O(1)` pop (from the end).
pub struct BitVec {
    buf: Vec<usize>,
    len: usize,
} 

////////////////////////////////////////////////////////////////////////////////
// Inherent methods
////////////////////////////////////////////////////////////////////////////////

impl BitVec {
    /// Constructs a new, empty `BitVec`.
    ///
    /// The vector will not allocate until elements are pushed onto it.
    ///
    /// # Examples
    ///
    /// ```
    /// let mut vec = los::bitvec::BitVec::new();
    /// assert_eq!(vec.len(), 0);
    /// assert_eq!(vec.capacity(), 0);
    /// ```
    #[inline]
    pub fn new() -> BitVec {
        BitVec {
            buf: Vec::<usize>::new(),
            len: 0,
        }
    }

    /// Constructs a new, empty `BitVec` with the specified capacity.
    ///
    /// The vector will be able to hold at least `capacity` elements without
    /// reallocating. Reallocation of memory at heap occurs when there is not 
    /// enough `buf` capacity. If `capacity` is 0, the vector will not allocate.
    ///
    /// It is important to note that this function does not specify the *length*
    /// of the returned vector, but only the *capacity*.
    ///
    /// # Examples
    ///
    /// ```
    /// use los::bitvec::{BitVec, WORDSIZE};
    ///
    /// let mut vec = BitVec::with_capacity(WORDSIZE);
    /// assert_eq!(vec.len(), 0);
    /// assert_eq!(vec.capacity(), WORDSIZE);
    /// 
    /// // These are all done without reallocating...
    /// for _ in 0..WORDSIZE {
    ///     vec.push(true);
    /// }
    /// assert_eq!(vec.len(), WORDSIZE);
    /// assert_eq!(vec.capacity(), WORDSIZE);
    /// 
    /// // ...but this may make the vector reallocate
    /// vec.push(true);
    /// assert_eq!(vec.len(), WORDSIZE+1);
    /// assert_eq!(vec.capacity(), WORDSIZE*2);
    /// ```
    #[inline]
    pub fn with_capacity(capacity: usize) -> BitVec {
        BitVec {
            buf: create_buf(capacity),
            len: 0,
        }
    }

    /// Constructs a new `BitVec` with the specified length. All elements are
    /// setups to `false`.
    ///
    /// # Examples
    ///
    /// ```
    /// use los::bitvec::{BitVec, WORDSIZE};
    ///
    /// let mut vec = BitVec::with_length(WORDSIZE);
    /// assert_eq!(vec.len(), WORDSIZE);
    /// assert_eq!(vec.capacity(), WORDSIZE);
    /// ```
    #[inline]
    pub fn with_length(len: usize) -> BitVec {
        BitVec {
            buf: create_buf(len),
            len: len,
        }
    }
    
    /// Creates a vector from string. '1' is `true`, other symbols is `false`.
    /// 
    /// # Examples
    ///
    /// ```
    /// use los::bitvec::BitVec;
    ///
    /// let vec = BitVec::from("0110");
    /// assert_eq!(vec.to_string(), "0110");
    /// ```
    pub fn from(string: &str) -> BitVec {
        let mut vec = BitVec::with_capacity(string.len());
        let mut value: bool;
                
        for c in string.chars() {
            match c {
                '1' => value = true, 
                _ => value = false
            }
            vec.push(value);
        }
        
        vec
    }
    
    /// Returns the `&buf` collection.
    pub fn buf(&self) -> &Vec<usize> {
        &self.buf
    }

    /// Returns the `&mut buf` collection.
    pub fn buf_mut(&mut self) -> &mut Vec<usize> {
        &mut self.buf
    }

    /// Returns the number of elements the vector can hold without
    /// reallocating. its round up to target bitness.
    ///
    /// # Examples
    ///
    /// ```
    /// use los::bitvec::{BitVec, WORDSIZE};
    ///
    /// let vec = BitVec::with_capacity(10);
    /// assert_eq!(vec.capacity(), WORDSIZE);
    /// ```
    #[inline]
    pub fn capacity(&self) -> usize {
        self.buf.len() << BIT_SHIFT
    }

    /// Returns the number of elements in the vector.
    ///
    /// # Examples
    ///
    /// ```
    /// use los::bitvec::BitVec;
    ///
    /// let mut vec = BitVec::new();
    /// for _ in 0..10 {
    ///     vec.push(true);
    /// }
    /// assert_eq!(vec.len(), 10);
    /// ```
    #[inline]
    pub fn len(&self) -> usize {
        self.len
    }

    /// Gets element by its `index` < `self.len()`. Check `index` yourself.
    ///
    /// # Examples
    ///
    /// ```
    /// use los::bitvec::BitVec;
    ///
    /// let mut vec = BitVec::new();
    /// vec.push(true);
    /// assert_eq!(vec.get(0), true);
    /// ```
    #[inline]
    pub fn get(&self, index: usize) -> bool {
        (self.buf[buf_index(index)] >> bit_index(index)) & 1 == 1
    }

    /// Sets element by its `index` < `self.len()`. Check `index` yourself.
    ///
    /// # Examples
    ///
    /// ```
    /// use los::bitvec::BitVec;
    ///
    /// let mut vec = BitVec::with_capacity(1);
    /// vec.set(0, true);
    /// assert_eq!(vec.get(0), true);
    /// ```
    #[inline]
    pub fn set(&mut self, index: usize, value: bool) {
        if value {
            self.buf[buf_index(index)] |= bit_mask(index);
        }
        else {
            self.buf[buf_index(index)] &= !bit_mask(index);
        }
    }

    /// Sets all elements to false.
    ///
    /// # Examples
    ///
    /// ```
    /// use los::bitvec::BitVec;
    ///
    /// let mut vec = BitVec::new();
    /// vec.resize(100);
    /// vec.setall(true);
    /// assert_eq!(vec.count_ones(), 100);
    /// vec.setall(false);
    /// assert_eq!(vec.count_ones(), 0);
    /// ```
    #[inline]
    pub fn setall(&mut self, value: bool) {
        let len = self.buf.len();
        let pkg_value = match value {
            false => 0usize,
            true  => usize::max_value()
        };

        for i in 0..len {
            self.buf[i] = pkg_value;
        }
    }

    /// Xors element by its `index` < `self.len()`. Check `index` yourself.
    ///
    /// # Examples
    ///
    /// ```
    /// use los::bitvec::BitVec;
    ///
    /// let mut vec = BitVec::new();
    /// vec.push(true);
    /// vec.xor(0, true);
    /// assert_eq!(vec.pop(), Some(false));
    /// ```
    #[inline]
    pub fn xor(&mut self, index: usize, value: bool) {
        if value {
            self.buf[buf_index(index)] ^= bit_mask(index);
        }
    }

    /// Appends an element to the back of a collection. Reallocation of memory 
    /// at heap occurs when there is not enough `buf` capacity.
    ///
    /// # Examples
    ///
    /// ```
    /// use los::bitvec::BitVec;
    ///
    /// let mut vec = BitVec::new();
    /// vec.push(true);
    /// assert_eq!(vec.len(), 1);
    /// ```
    pub fn push(&mut self, value: bool) {
        let len = self.len;
        
        if self.buf.len() == 0 || len == self.capacity() {
            self.buf.push(0);
        }
        
        self.len += 1;
        self.set(len, value);
    }

    /// Removes the last element from a vector and returns it, or `None` if it
    /// is empty.
    ///
    /// # Examples
    ///
    /// ```
    /// use los::bitvec::BitVec;
    ///
    /// let mut vec = BitVec::new();
    /// vec.push(true);
    /// let value = vec.pop();
    /// assert_eq!(value, Some(true));
    /// assert_eq!(vec.len(), 0);
    /// ```
    #[inline]
    pub fn pop(&mut self) -> Option<bool> {
        let len = self.len;
        let result: Option<bool>;
        
        if len == 0 {
            result = None;
        } else {
            result = Some(self.get(len-1));
            self.len -= 1;
        }
        
        result
    }
    
    /// Resizes the `BitVec` in-place so that `len()` is equal to `new_len`.
    ///
    /// If `new_len` is greater than `len()`, the `BitVec` is extended by the
    /// difference, with additional zeroes. If `new_len` is less than 
    /// `len()`, the `BitVec` is simply truncated.
    ///
    /// # Examples
    ///
    /// ```
    /// use los::bitvec::BitVec;
    ///
    /// let mut vec = BitVec::with_capacity(100);
    /// vec.resize(150);
    /// assert_eq!(vec.len(), 150);
    /// vec.resize(50);
    /// assert_eq!(vec.len(), 50);
    /// ```
    #[inline]
    pub fn resize(&mut self, new_len: usize) {
        let len = self.len();

        if new_len > len {
            self.extend(new_len - len);
        } else {
            self.truncate(new_len);
        }
    }

    /// Extends the vector by `n` additional zeroes.
    ///
    /// # Examples
    ///
    /// ```
    /// use los::bitvec::BitVec;
    ///
    /// let mut vec = BitVec::new();
    /// vec.extend(1);
    /// vec.set(0, true);
    /// assert_eq!(vec.get(0), true);
    /// assert_eq!(vec.len(), 1);
    /// ```
    pub fn extend(&mut self, n: usize) {
        let new_len = self.len + n;

        if new_len > self.capacity() {
            self.buf.resize(buf_capacity(new_len), 0);
        }
        self.len = new_len;
    }
    
    /// Shortens the vector, keeping the first `len` elements and dropping
    /// the rest. No truncation occurs when `len` is greater than the vector's 
    /// current length.
    ///
    /// # Examples
    ///
    /// ```
    /// use los::bitvec::{BitVec, WORDSIZE};
    ///
    /// let mut vec = BitVec::new();
    /// for _ in 0..WORDSIZE+1 {
    ///     vec.push(true);
    /// }
    /// assert_eq!(vec.len(), WORDSIZE+1);
    /// assert_eq!(vec.capacity(), WORDSIZE*2);
    /// vec.truncate(WORDSIZE);
    /// assert_eq!(vec.len(), WORDSIZE);
    /// assert_eq!(vec.capacity(), WORDSIZE);
    /// ```
    #[inline]
    pub fn truncate(&mut self, len: usize) {
        if len < self.len {
            self.len = len;
            self.buf.truncate(buf_capacity(len))
        }
    }

    /// Returns the number of ones in the binary representation of self.
    ///
    /// # Examples
    ///
    /// ```
    /// use los::bitvec::BitVec;
    ///
    /// let mut vec = BitVec::new();
    /// for _ in 0..128 {
    ///     vec.push(true);
    /// }
    /// assert_eq!(vec.count_ones(), 128);
    /// 
    /// vec.resize(100);
    /// assert_eq!(vec.count_ones(), 100);
    /// 
    /// vec.set(0, false);
    /// vec.set(99, false);
    /// assert_eq!(vec.count_ones(), 98);
    /// ```
    pub fn count_ones(&self) -> u32 {
        let mut count: u32 = 0;   
        let len = self.len();
        
        // Check vector length
        if len == 0 {
            return count
        }
        
        // Count ones in the buffer except tie
        let buf_len = buf_index(len);
        for i in 0..buf_len {
            count += self.buf[i].count_ones();
        }
        
        // Count ones in the tie
        let tie_len = bit_index(len);
        if tie_len != 0 {
            let tie = self.buf[buf_len] & ((1usize << tie_len) - 1);
            count += tie.count_ones();
        }
        
        count
    }

    /// Converts the bit vector to string.
    ///
    /// # Examples
    ///
    /// ```
    /// use los::bitvec::BitVec;
    ///
    /// let a = BitVec::from("0011");
    /// assert_eq!(a.stringify(), "0011");
    /// ```
    pub fn stringify(&self) -> String {
        let len = self.len;
        let mut strvec = String::new();
        
        for i in 0..len {
            if self.get(i) {
                strvec += "1";
            } else {
                strvec += "0";
            }
        }

        strvec
    }
}

////////////////////////////////////////////////////////////////////////////////
// Common trait implementations for BitVec
////////////////////////////////////////////////////////////////////////////////

/// The Clone trait is used to specify the clone() functionality.
///
/// # Examples
///
/// ```
/// use los::bitvec::BitVec;
///
/// let a = BitVec::from("0011");
/// let b = a.clone();
/// let mut c = BitVec::from("1100");
/// c.clone_from(&b);
/// assert_eq!(a.to_string(), "0011");
/// assert_eq!(b.to_string(), "0011");
/// assert_eq!(c.to_string(), "0011");
/// ```
impl Clone for BitVec {
    fn clone(&self) -> BitVec {
        BitVec {
            buf: self.buf.clone(),
            len: self.len,
        }
    }

    fn clone_from(&mut self, other: &BitVec) {
        self.buf.clone_from(&other.buf);
        self.len = other.len;
    }
}

/// The fmt::Display trait is used to specify the to_string() functionality.
///
/// # Examples
///
/// ```
/// use los::bitvec::BitVec;
///
/// let a = BitVec::from("0011");
/// assert_eq!(a.to_string(), "0011");
/// ```
impl fmt::Display for BitVec {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        write!(f, "{}", self.stringify())
    }
}

/// The fmt::Debug trait is used to specify the println!("{:?}", vec) functionality.
impl fmt::Debug for BitVec {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        write!(f, "{}", self.stringify())
    }
}

////////////////////////////////////////////////////////////////////////////////
// Constants and static methods
////////////////////////////////////////////////////////////////////////////////

#[cfg(target_pointer_width = "32")]
pub const WORDSIZE: usize = 32;
#[cfg(target_pointer_width = "32")]
const BIT_SHIFT: u8 = 5;

#[cfg(target_pointer_width = "64")]
pub const WORDSIZE: usize = 64;
#[cfg(target_pointer_width = "64")]
const BIT_SHIFT: u8 = 6;

const BIT_MASK: usize = WORDSIZE-1;

// Get index in the buffer by vector index.
#[inline]
fn buf_index(index: usize) -> usize {
    index >> BIT_SHIFT
}

// Get index in the usize chunk by vector index.
#[inline]
fn bit_index(index: usize) -> usize {
    index & BIT_MASK
}

// Get bit mask for bit operations by vector index.
#[inline]
fn bit_mask(index: usize) -> usize {
    1usize << bit_index(index)
}

// Get number of usize chunks required to store `capacity` bits.
#[inline]
fn buf_capacity (capacity: usize) -> usize {
    let mut cap = buf_index(capacity);
    if bit_index(capacity) != 0 {
        cap += 1;
    }
    cap
}

// Create buffer with specified length.
#[inline]
fn create_buf(len: usize) -> Vec<usize> {
    let cap = buf_capacity(len);
    let mut buf = Vec::<usize>::with_capacity(cap);
    buf.resize(cap, 0);
    buf
}

