#include "sudokuProblemIndividual.hpp"

// Function that returns the row (1-9) of a certain position (1-81) in the sudoku
int getRowFromPosition(int position)
{
    switch (position)
    {
    case 1: case 2: case 3: case 4: case 5: case 6: case 7: case 8: case 9:
        return 1;
        break;

    case 10: case 11: case 12: case 13: case 14: case 15: case 16: case 17: case 18:
        return 2;
        break;

    case 19: case 20: case 21: case 22: case 23: case 24: case 25: case 26: case 27:
        return 3;
        break;

    case 28: case 29: case 30: case 31: case 32: case 33: case 34: case 35: case 36:
        return 4;
        break;

    case 37: case 38: case 39: case 40: case 41: case 42: case 43: case 44: case 45:
        return 5;
        break;

    case 46: case 47: case 48: case 49: case 50: case 51: case 52: case 53: case 54:
        return 6;
        break;

    case 55: case 56: case 57: case 58: case 59: case 60: case 61: case 62: case 63:
        return 7;
        break;

    case 64: case 65: case 66: case 67: case 68: case 69: case 70: case 71: case 72:
        return 8;
        break;

    case 73: case 74: case 75: case 76: case 77: case 78: case 79: case 80: case 81:
        return 9;
        break;
    
    default:
        return -1;
        break;
    }
}

// Function that returns the column (1-9) of a certain position (1-81) in the sudoku
int getColFromPosition(int position)
{
    switch (position)
    {
    case 1: case 10: case 19: case 28: case 37: case 46: case 55: case 64: case 73:
        return 1;
        break;

    case 2: case 11: case 20: case 29: case 38: case 47: case 56: case 65: case 74:
        return 2;
        break;

    case 3: case 12: case 21: case 30: case 39: case 48: case 57: case 66: case 75:
        return 3;
        break;

    case 4: case 13: case 22: case 31: case 40: case 49: case 58: case 67: case 76:
        return 4;
        break;

    case 5: case 14: case 23: case 32: case 41: case 50: case 59: case 68: case 77:
        return 5;
        break;

    case 6: case 15: case 24: case 33: case 42: case 51: case 60: case 69: case 78:
        return 6;
        break;

    case 7: case 16: case 25: case 34: case 43: case 52: case 61: case 70: case 79:
        return 7;
        break;

    case 8: case 17: case 26: case 35: case 44: case 53: case 62: case 71: case 80:
        return 8;
        break;

    case 9: case 18: case 27: case 36: case 45: case 54: case 63: case 72: case 81:
        return 9;
        break;
    
    default:
        return -1;
        break;
    }
}

// Function that returns the subgrid (1-9) of a certain position (1-81) in the sudoku
int getSubgridFromPosition(int position)
{
    switch (position)
    {
    case 1: case 2: case 3: case 10: case 11: case 12: case 19: case 20: case 21:
        return 1;
        break;

    case 4: case 5: case 6: case 13: case 14: case 15: case 22: case 23: case 24:
        return 2;
        break;

    case 7: case 8: case 9: case 16: case 17: case 18: case 25: case 26: case 27:
        return 3;
        break;

    case 28: case 29: case 30: case 37: case 38: case 39: case 46: case 47: case 48:
        return 4;
        break;

    case 31: case 32: case 33: case 40: case 41: case 42: case 49: case 50: case 51:
        return 5;
        break;

    case 34: case 35: case 36: case 43: case 44: case 45: case 52: case 53: case 54:
        return 6;
        break;

    case 55: case 56: case 57: case 64: case 65: case 66: case 73: case 74: case 75:
        return 7;
        break;

    case 58: case 59: case 60: case 67: case 68: case 69: case 76: case 77: case 78:
        return 8;
        break;

    case 61: case 62: case 63: case 70: case 71: case 72: case 79: case 80: case 81:
        return 9;
        break;
    
    default:
        return -1;
        break;
    }
}