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

extern crate los;

use std::env;
use std::time::SystemTime;
use los::bitmat::BitMat;
use los::ligsol::LightsSolver;

fn main() {
    let n = env::args().nth(1).unwrap_or("10".to_string()).parse::<usize>().unwrap_or(10);
    let mut field = BitMat::with_size(n, n);
    
    for i in 0..n {
        field[i].setall(true);
    }

    println!("Usage: los <size>\nSolving {} x {}...\n", n, n);

    let mut solver = LightsSolver::from(&field);
    let now = SystemTime::now();

    match solver.solve() {
        Some(sol) => println!("{}\nNumber of solutions: {}\nMinimal weight: {}", 
                              sol, solver.alg().n_solutions(), solver.alg().min_weight()),
        None => println!("No solution"),
    }

    match now.elapsed() {
       Ok(elapsed) => {
           println!("Done at {} seconds", elapsed.as_secs());
       }
       Err(e) => {
           println!("Error: {:?}", e);
       }
   }    
}
