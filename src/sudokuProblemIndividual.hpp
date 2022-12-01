#pragma once // Multiple inclusion guardian

#include "individual.hpp" // Individual class
#include "utils.hpp" // Random distributions
#include <set> // std::set
#include <vector> // std::vector

// Vector of sets that contains the cell number of each row
const std::vector<std::set<int>> coordinatesPerRow = {
                                                        {1, 2, 3, 4, 5, 6, 7, 8, 9 }, // Row 1
                                                        {10,11,12,13,14,15,16,17,18}, // Row 2
                                                        {19,20,21,22,23,24,25,26,27}, // Row 3
                                                        {28,29,30,31,32,33,34,35,36}, // Row 4
                                                        {37,38,39,40,41,42,43,44,45}, // Row 5
                                                        {46,47,48,49,50,51,52,53,54}, // Row 6
                                                        {55,56,57,58,59,60,61,62,63}, // Row 7
                                                        {64,65,66,67,68,69,70,71,72}, // Row 8
                                                        {73,74,75,76,77,78,79,80,81}  // Row 9
                                                        };


// Vector of sets that contains the cell number of each column
const std::vector<std::set<int>> coordinatesPerCol = {
                                                        {1,10,19,28,37,46,55,64,73}, // Column 1
                                                        {2,11,20,29,38,47,56,65,74}, // Column 2
                                                        {3,12,21,30,39,48,57,66,75}, // Column 3
                                                        {4,13,22,31,40,49,58,67,76}, // Column 4
                                                        {5,14,23,32,41,50,59,68,77}, // Column 5
                                                        {6,15,24,33,42,51,60,69,78}, // Column 6
                                                        {7,16,25,34,43,52,61,70,79}, // Column 7
                                                        {8,17,26,35,44,53,62,71,80}, // Column 8
                                                        {9,18,27,36,45,54,63,72,81}  // Column 9
                                                        };

// Vector of sets that contains the cell number of each subgrid
const std::vector<std::set<int>> corrdinatesPerSubgrid = {
                                                            {1, 2, 3, 10,11,12,19,20,21}, // Subgrid 1
                                                            {4, 5, 6, 13,14,15,22,23,24}, // Subgrid 2
                                                            {7, 8, 9, 16,17,18,25,26,27}, // Subgrid 3
                                                            {28,29,30,37,38,39,46,47,48}, // Subgrid 4
                                                            {31,32,33,40,41,42,49,50,51}, // Subgrid 5
                                                            {34,35,36,43,44,45,52,53,54}, // Subgrid 6
                                                            {55,56,57,64,65,66,73,74,75}, // Subgrid 7
                                                            {58,59,60,67,68,69,76,77,78}, // Subgrid 8
                                                            {61,62,63,70,71,72,79,80,81}  // Subgrid 9
                                                            };


// Function that returns the row (1-9) of a certain position (1-81) in the sudoku
int getRowFromPosition(int position);

// Function that returns the column (1-9) of a certain position (1-81) in the sudoku
int getColFromPosition(int position);

// Function that returns the subgrid (1-9) of a certain position (1-81) in the sudoku
int getSubgridFromPosition(int position);

/*

SDK_Individual class:

A class template that represents an individual of the sudoku probem. Thus, it represents a posible
sudoku solution. As it is an individual of a EA, it inherits from Individual (individual.hpp) and it
implements its pure virtual functions.

It has the same template parameters as Individual (individual.hpp):

    template <class RepresentationType, class FitnessValueType>

*/

template <class RepresentationType, class FitnessValueType>
class SDK_Individual : public Individual<RepresentationType, FitnessValueType>
{

// ATTRIBUTES //

private:

    // Mutation method
    char m_mutation_method;

// METHODS //

public:

    // CONSTRUCTORS AND DESTRUCTOR //

    // Constructor with creation generation, lenght of its genotype and intial values of the sudoku (read from a file)
    SDK_Individual(int creationGen, unsigned int lengthOfTheGenotype, std::vector<int> sudokuInitialValues, char mutation_method);

    // Default constructor and destructor
    SDK_Individual(){}
    ~SDK_Individual() {}

    // IMPLEMENTATION OF THE PURE VIRTUAL METHODS OF Individual (individual.hpp) //

    // Method that initializes the individual (not fixed cells, fitness value, ...)
    void initializeIndividual();

    // Method that calculates the fitness value of the individual
    void calculateFitnessValue();

    // Method that mutates the individual randomly
    void mutateIndividual(float pm);

    // Function that updates the member posibleValues of each SudokuCell (sudokuCell.hpp) so
    // that the initialization and mutation is not completely random, but it uses the information
    // that the current solution has (which cells are repeated on each row/col/subgrid, which ones
    // do not appear and so on)
    void updateLegalValuesOfEachCell();

    // Method that initializes randomly each cell of the individual
    void initializeAllCells();

    // Functions that check and correct the individual's fixed cells when called. They are for debugging
    // purpose so that the programmer ensures that fixed cells remain as such during generations
    #if defined DEBUG_MODE_ON
        bool checkFixedModified(std::vector<int> sudokuInitialValues);
        void correctFixedModified(std::vector<int> sudokuInitialValues);
    #endif
};

//Constructor 1
template <class RepresentationType, class FitnessValueType>
SDK_Individual<RepresentationType, FitnessValueType>::
SDK_Individual(int creationGen, unsigned int lengthOfTheGenotype, std::vector<int> sudokuInitialValues, char mutation_method): 
Individual<RepresentationType, FitnessValueType>(creationGen, lengthOfTheGenotype)
{
    // Stores the creation generation, the length of the genotype and the mutation method in memory
    this->m_creationGen = creationGen;
    this->m_lengthOfTheGenotype = lengthOfTheGenotype;
    this->m_mutation_method = mutation_method;

    // Reserves memory for the genotype
    this->m_genotype.reserve(lengthOfTheGenotype);

    // For each intial value of the sudoku initialized its respective cell
    auto it = sudokuInitialValues.begin();
    int i = 1;
    while(it != sudokuInitialValues.end())
    {
        // sudokuInitialValues is a vector of 81 ints that represents each initial value 
        // of the sudoku to be solved. All cells that are to be solved are represented with
        // a 0. Fixed cells are represented with its value.

        // Configure the cell with the info of sudokuInitialValues
        // more info abut the struct available at sudokuCell.hpp
        RepresentationType cell;

        // Saves the value of the cell
        cell.cellValue = (*it);

        // Set whether is a fixed cell (not 0) or not (equal to 0)
        if((*it) == 0)
            cell.fixed = false;
        else
            cell.fixed = true;

        // Gets its row, column and subgrid
        cell.row = getRowFromPosition(i);
        cell.col = getColFromPosition(i);
        cell.subgrid = getSubgridFromPosition(i);

        // Stores its cell number (1-81)
        cell.cellNumber = i;

        // Appends it to the genotype
        this->m_genotype.emplace_back(cell);

        it++;
        i++;
    }

    // As the individual has changed, so has its legal value of each cell, 
    // so it updates them
    this->updateLegalValuesOfEachCell();
}

// Initializes the individual
template <class RepresentationType, class FitnessValueType>
void SDK_Individual<RepresentationType, FitnessValueType>::
initializeIndividual()
{
    // Initializes each cell of the individual
    this->initializeAllCells();

    // Updates the new legal value of each cell
    this->updateLegalValuesOfEachCell();

    // Calculates the fitness value of the initialized individual
    this->calculateFitnessValue();
}

// Calculates the fitness value of the individual
template <class RepresentationType, class FitnessValueType>
void SDK_Individual<RepresentationType, FitnessValueType>::
calculateFitnessValue()
{

    /*

    Fitness function = (fitness_fi + fitness_ci + fitness_si) / 2, where:
        - fitness_fi is the number of timer a value is repeated on each row * 2 
        - fitness_ci is the number of timer a value is repeated on each column * 2 
        - fitness_si is the number of timer a value is repeated on each subgrid * 2 
        - the / 2 term eliminates the * 2 term of the explained above

    */

    // Separates the add terms as fi(repeated values on each row), ci(repeated values on each column)
    // and si(repeated values on each subgrid) as it
    int fitness_fi = 0;
    int fitness_ci = 0;
    int fitness_si = 0;

    // For each cell in the sudoku
    auto it = this->m_genotype.begin();
    while(it != this->m_genotype.end())
    {
        // calculates fitness_fi term (number of times the value on it is repeated on the row)

        // Get the cell numbers of the row of cell i
        std::set<int> positionsInCellsRow = coordinatesPerRow.at((it->row)-1);

        // For each element in the row
        auto pos = positionsInCellsRow.begin();
        while(pos != positionsInCellsRow.end())
        {
            int checkValue = this->m_genotype[(*pos)-1].cellValue;
            int checkCellNum = this->m_genotype[(*pos)-1].cellNumber;

            // If the number of the cell on it is repeated, increase fitness_fi
            if(checkValue == (it->cellValue) && checkCellNum != (it->cellNumber))
                fitness_fi++;
            pos++;
        }

        // calculates fitness_ci term (number of times the value on it is repeated on the column)

        // Get the cell numbers of the column of cell i
        std::set<int> positionsInCellsCol = coordinatesPerCol.at((it->col)-1);

        // For each element in the col
        pos = positionsInCellsCol.begin();
        while(pos != positionsInCellsCol.end())
        {
            int checkValue = this->m_genotype[(*pos)-1].cellValue;
            int checkCellNum = this->m_genotype[(*pos)-1].cellNumber;

            // If the number of the cell on it is repeated, increase fitness_ci
            if(checkValue == (it->cellValue) && checkCellNum != (it->cellNumber))
                fitness_ci++;
            pos++;
        }

        // calculates fitness_si term (number of times the value on it is repeated on the subgrid)

        std::set<int> positionsInCellsSubgrid = corrdinatesPerSubgrid.at((it->subgrid)-1);

        // For each element in the subgrid
        pos = positionsInCellsSubgrid.begin();
        while(pos != positionsInCellsSubgrid.end())
        {
            int checkValue = this->m_genotype[(*pos)-1].cellValue;
            int checkCellNum = this->m_genotype[(*pos)-1].cellNumber;

            // If the number of the cell on it is repeated, increase fitness_si
            if(checkValue == (it->cellValue) && checkCellNum != (it->cellNumber))
                fitness_si++;
            pos++;
        }


        it++;
    }

    // Calculates the fitness value
    this->m_fitnessValue = static_cast<FitnessValueType>(fitness_fi + fitness_ci + fitness_si) / 2.0f;

}

// Mutation of an individual
template <class RepresentationType, class FitnessValueType>
void SDK_Individual<RepresentationType, FitnessValueType>::
mutateIndividual(float pm)
{
    // As recombination comes before mutation, legal values of each cell
    // must be updated
    this->updateLegalValuesOfEachCell();

    // For each cell in the individual
    auto it = this->m_genotype.begin();
    while(it != this->m_genotype.end())
    {
        // Only not fixed cells are mutable
        if(!(it->fixed))
        {
            // Mutate if a random value is less than the probability of mutation
            float randomValue = get_random_float_0_to_1();
            if(randomValue <= pm)
            {
                // If there are legal values for the cell to be mutated, only mutate to a
                // randomly chosen one of them 
                if(!(it->posibleValues.empty()))
                {
                    // Get a random element from a std::set<int> and replace it at it->cellValue
                    it->cellValue = get_random_element_setOfIntegers(it->posibleValues);
                }
                else
                {
                    // If there are not legal values, just mutate randomly
                    it->cellValue = get_random_number_1_to_9();
                }
            }
        }
        
        it++;
    }

    // As the individual has (probably) changed after mutation step, new legal values of 
    // each cell and fitness value must be updated
    this->calculateFitnessValue();
    this->updateLegalValuesOfEachCell();

}

// Updates the legal value of each cell of an individual
template <class RepresentationType, class FitnessValueType>
void SDK_Individual<RepresentationType, FitnessValueType>::
updateLegalValuesOfEachCell()
{
    /*

    OPERATION: Creates a std::set with the legal values on the cell's row (posibleRowValues),
    columns (posibleColValues) and subgrid (posibleSubgridValues). At the same time, it creates
    a std::set called legalValues that is the logical AND of the three of above (legal values taking
    into account row, col and subgrid). It turns out this approach is very effective with sudokus with
    high fitness value, as they have a lot of repeated cells. Nevertheless, as the sudoku aproximates
    to the solution, is likely that no value is legal on rows, cols and subgrids at the same time. When
    that happens, (only if mutation method 2 is selected) it does a logical OR of the three std::set 
    mentioned above, as it will mutate to a values that at least will be legal on row, col or subgrid.

    */


    // For each cell on the individual
    auto it = this->m_genotype.begin();
    int currentCellNumber = 1;
    while(it != this->m_genotype.end())
    {
        // legalValues is only needed on not fixed cells
        if(!(it->fixed))
        {
            // Initializes the set with all values, as each illegal value will be erased
            std::set<int> legalValues = {1,2,3,4,5,6,7,8,9};

            // Initializes posible values splitting by row, col or subgrid
            it->posibleRowValues = legalValues;
            it->posibleColValues = legalValues;
            it->posibleSubgridValues = legalValues;

            // Check illegal values in *it's row, col and subgrid //

            // Rows //

            // Get the cell numbers that form the row of *it
            std::set<int> positionsInCellsRow = coordinatesPerRow.at((it->row)-1);

            // For each element in the row
            auto pos = positionsInCellsRow.begin();
            while(pos != positionsInCellsRow.end())
            {
                // Erase its cellValue from the set of legal values
                int illegalValue = this->m_genotype[(*pos)-1].cellValue;
                if(this->m_genotype[(*pos)-1].cellNumber != currentCellNumber)
                {
                    legalValues.erase(illegalValue);
                    it->posibleRowValues.erase(illegalValue);
                }
                pos++;

            }

            // Cols // 

            // Get the cell numbers that form the column of *it
            std::set<int> positionsInCellsCol = coordinatesPerCol.at((it->col)-1);

            // For each element in the column
            pos = positionsInCellsCol.begin();
            while(pos != positionsInCellsCol.end())
            {
                if(this->m_genotype[(*pos)-1].cellNumber != currentCellNumber)
                {
                    // Erase its cellValue from the set of legal values
                    int illegalValue = this->m_genotype[(*pos)-1].cellValue;
                    legalValues.erase(illegalValue);
                    it->posibleColValues.erase(illegalValue);
                }
                pos++;
            }

            // Subgrids //

            // Get the cell numbers that form the subgrid of *it
            std::set<int> positionsInCellsSubgrids = corrdinatesPerSubgrid.at((it->subgrid)-1);

            // For each element in the subgrid
            pos = positionsInCellsSubgrids.begin();
            while(pos != positionsInCellsSubgrids.end())
            {
                if(this->m_genotype[(*pos)-1].cellNumber != currentCellNumber)
                {
                    // Erase its cellValue from the set of legal values
                    int illegalValue = this->m_genotype[(*pos)-1].cellValue;
                    legalValues.erase(illegalValue);
                    it->posibleSubgridValues.erase(illegalValue);
                }
                pos++;
            }

            // If merging (logical NAND) rows, cols and subgrids ilegal values the set is empty
            if(legalValues.empty() && this->m_mutation_method == 2)
            {
                // Do a logical NOR with the illegal value of each row, col and subgrid
                if(!it->posibleRowValues.empty())
                    legalValues.insert(it->posibleRowValues.begin(), it->posibleRowValues.end());

                if(!it->posibleColValues.empty())
                    legalValues.insert(it->posibleColValues.begin(), it->posibleColValues.end());

                if(!it->posibleSubgridValues.empty())
                    legalValues.insert(it->posibleSubgridValues.begin(), it->posibleSubgridValues.end());
            }

            it->posibleValues = legalValues;
        }

        it++;
        currentCellNumber++;
    }

}

// Cells initializer
template <class RepresentationType, class FitnessValueType>
void SDK_Individual<RepresentationType, FitnessValueType>::
initializeAllCells()
{
    
    // For each cell
    auto it = this->m_genotype.begin();
    while(it != this->m_genotype.end())
    {

        // It only needs to be initialized if its not fixed
        if(!(it->fixed))
        {
            if(!(it->posibleValues.empty()))
            {
                // If the posibleValues of the cell is not empty, initialize it to a legal value
                // calculated on updateLegalValuesOfEachCell()
                it->cellValue = get_random_element_setOfIntegers(it->posibleValues);
            }
            else
            {
                // If all numbers between 1 and 9 are illegal pick one ramdomly
                int random_val = get_random_number_1_to_9();
                it->cellValue = random_val;
            }
        }

        it++;
    }
}

#if defined DEBUG_MODE_ON

    // Checks if any fixed cell has been modified (debugging purpouse)
    template <class RepresentationType, class FitnessValueType>
    bool SDK_Individual<RepresentationType, FitnessValueType>::
    checkFixedModified(std::vector<int> sudokuInitialValues)
    {
        // For each cell of the sudoku
        auto it = this->m_genotype.begin();
        int i = 0;
        while(it != this->m_genotype.end())
        {
            
            // If its fixed and its value is different to the initial
            // Incorrect cell -> return false
            if(it->fixed) 
                if(it->cellValue != sudokuInitialValues.at(i))
                    return false;

            it++;
            i++;
            
        }

        return  true;
    }

    // Corrects all mistakenly modified fixed cells
    template <class RepresentationType, class FitnessValueType>
    void SDK_Individual<RepresentationType, FitnessValueType>::
    correctFixedModified(std::vector<int> sudokuInitialValues)
    {
        // For each cell of the sudoku
        auto it = this->m_genotype.begin();
        int i = 0;
        while(it != this->m_genotype.end())
        {
            // If its fixed and its value is different to the initial
            // Incorrect cell -> correct it
            if(it->fixed)
                it->cellValue = sudokuInitialValues[i];

            it++;
            i++;
        }
    }

#endif