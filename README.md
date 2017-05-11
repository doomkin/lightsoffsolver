## Abstract
The program solves the puzzle Lights Off. It is a puzzle game, where the objective is to turn off all of the tiles on the board. Each click toggles the state of the clicked tile and its non diagonal neighbors.

## License
This program is free software. See http://www.gnu.org/copyleft/gpl.html the license.  

## Build
```bash
git clone https://github.com/doomkin/lightsoffsolver.git  
cd lightsoffsolver  
make  
```
## Usage
```bash
./lightsoffsolver [<Switches>] [< input.txt]
<Switches>  
  -r4 : number of rows in the field of ones, square if no columns  
  -c5 : number of columns in the field of ones, square if no rows  
  -p  : create image of solution to file "lightsoff_4x5.png"  
  -a  : apply solution to field of ones  
  -i  : print info: field size, number of solutions, weight of solution, time  
  -h  : print help  
```
## Examples
1. `010`  
`111`  
`010`  
->  
`000`  
`000`  
`000`
2. `1111`  
`1111`  
`1111`  
`1111`  
 ->  
`0010`  
`1000`  
`0001`  
`0100`
3. `1111111111`  
`1111111111`  
`1111111111`  
`1111111111`  
`1111111111`  
`1111111111`  
`1111111111`  
`1111111111`  
`1111111111`  
`1111111111`  
->  
`1010000101`  
`0100110010`  
`1010110101`  
`0001001000`  
`0110110110`  
`0110110110`  
`0001001000`  
`1010110101`  
`0100110010`  
`1010000101`
