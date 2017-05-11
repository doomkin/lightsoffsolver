pub mod bitvec;
pub mod bitmat;
pub mod bitalg;
pub mod ligsol;

#[cfg(test)]
mod tests {
    #[test]
    fn it_works() {
        let mut vec = super::bitvec::BitVec::new();
        vec.push(true);
        assert_eq!(vec.get(0), true);
        vec.set(0, false);
        assert_eq!(vec.pop(), Some(false));
        assert_eq!(vec.len(), 0);

        let mut mat = super::bitmat::BitMat::with_size(10, 10);
        mat[0].set(0, true);
        assert_eq!(mat[0].get(0), true);
    }
}
