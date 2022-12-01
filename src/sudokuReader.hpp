#pragma once // Multiple inclusion guardian

#include <iostream> // std::cout, std::endl
#include <fstream> // std::ifstream
#include <sstream> // std::stringstream
#include <iterator> //std::vector<>::iterator
#include <string> // std::string
#include <vector> // std::vector

#include "utils.hpp" // split_string

/*

SudokuReader class:

Class that reads the sudoku to be solved from a file. The structure of the file must be as follows:

    - One row of the file equals to a row of the sudoku
    - All empty cells must be represented with a 0
    - values are separated with a comma

    - Example:

    0,0,0,0,3,0,0,0,4 
    0,9,0,4,0,6,0,7,0
    0,5,0,0,0,0,3,8,0
    0,0,0,0,7,8,0,0,3
    3,0,0,0,0,0,6,9,0
    5,4,0,6,0,0,0,2,0
    7,0,5,0,2,4,0,0,0
    9,8,4,0,6,5,2,0,0
    0,2,6,0,8,0,0,0,9

*/

class SudokuReader
{

// ATTRIBUTES //

private:

    // Path+name to the sudoku file (can be either relative or absolute)
    std::string m_sudokuFilePath;

    // Vector of ints that contains all cells provided on the text file
    std::vector<int> m_readSudoku;

// METHODS //

public:

    // CONSTRUCTOR AND DESTRUCTOR //

    // Constructor. The path+name of the file must be provided as a std::string
    SudokuReader(std::string sudokuFilePath);

    // Standard destructor
    ~SudokuReader() {}

    // Getter of the m_readSudoku member
    std::vector<int> getSudoku();
};

// Constructor
SudokuReader::SudokuReader(std::string sudokuFilePath)
{
    // Saves the path
    m_sudokuFilePath = sudokuFilePath;

    // Initializes a std::string that will contain each line of the text file
    std::string sudokuLine;

    // Creates a file stream with the text file
    std::ifstream sudokuFile(m_sudokuFilePath);

    // If the file is readable
    if(sudokuFile.is_open())
    {
        // For each line in file
        while(std::getline(sudokuFile, sudokuLine))
        {
            // Split it with commas
            std::vector<std::string> splitedString;
            split_string(sudokuLine, splitedString, ',');

            // Each value in splitedString is a sudoku cell value
            auto it = splitedString.begin();
            while(it != splitedString.end())
            {
                // Appends it to m_readSudoku member
                m_readSudoku.push_back(std::stoi(*it));
                it++;
            }
        }
    }
    else
    {
        // It the file is not readable or does not exist
        std::cout << "Unable to open sudoku file\n";
        std::vector<int> errorIndicator = {-1};
        m_readSudoku = errorIndicator;
    }
    
}

// Getter of the member m_readSudoku
std::vector<int> SudokuReader::getSudoku()
{
    return m_readSudoku;
}