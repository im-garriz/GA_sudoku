#pragma once // Multiple inclusion guardian

#include <set> // std::set

/*

SudokuCell struct:

Data structure that represents each cell of a sudoku. It has the following members:

    - cellValue: the value that the cell contains
    - fixed: true if the cell is fixed (cannot be modified), false if not
    - row, col and subgrid: the row, col and subgrid to which the cell belongs to
    - cellNumber: the number (1-81) of the cell

    - posibleValues: set of integers that has the values between 1-9 to would be legal to be changed

    - posibleRowValues, posibleColValues and posibleSubgridValues: the same as posibleValues, but
      taking into account rows, cols and subgrids separately

*/

struct SudokuCell
{
    // Value that the cell has
    int cellValue;

    // Whether it is a fixed (given) cell or not
    bool fixed;

    // row, col and subgrid of the cell
    int row;
    int col;
    int subgrid;

    // Cell number (1-81)
    int cellNumber;
    
    // Set of values that do satisfy initial restrictions of the sudoku problem for the current cell
    std::set<int> posibleValues;

    // Set of values that do satisfy initial restrictions of the sudoku problem for the current cell
    // separating the problem on rows, cols and subgrids
    std::set<int> posibleRowValues;
    std::set<int> posibleColValues;
    std::set<int> posibleSubgridValues;
    
};