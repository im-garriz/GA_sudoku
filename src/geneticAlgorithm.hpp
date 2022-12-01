#pragma once // Multiple inclusion guardian

#include <vector> // std::vector
#include <chrono> // time measures

/*

GeneticAlgorithm class:

A class template that represents the genetic algorithm. Thus, it has the population as 
member and runs generations until the stop condition is satisfied.

As it has a member of a type that inherits from Population (population.hpp), it needs
to be a class template. Its template parameters are the following:

template <class PopulationObject, class IndividualClass, class FileParametersObject>

Where PopulationObject represents the class that inherits from Population (population.hpp),
IndividualClass represents the individual of the problem (class that inherits from
Individual (individual.hpp)) and FileParametersObject represents a class/struct that
contains all parameters that are read from a configuration file

*/

template <class PopulationObject, class IndividualClass, class FileParametersObject>
class GeneticAlgorithm
{

// ATRIBUTES //

private:

    // Dynamic PopulationObject that represents the population
    PopulationObject *m_population;

    // Member that represents the struct that holds all file parameters
    FileParametersObject m_fileParameters;

    // Object that contains the solution (or best individual in case of 
    // not converging) when the algorithm has finished
    IndividualClass m_solution;

    // True if the sudoku has been solved, False if not
    bool m_solutionFind;

    // Vector that contains all initial cells of the problem
    std::vector<int> m_initialSudokuCells;

    // Generation on which the algorithm currently is
    int m_generation;

    // Historical of best and average fitness values on each gen
    std::vector<int> m_bestIndividualOfEachGen;
    std::vector<float> m_avgIndividualOfEachGen;

    // Time when the algorithm started
    std::chrono::time_point<std::chrono::steady_clock> m_ceroTime;

    // Elapsed time in seconds
    int m_elapsedTime;

// METHODS //

public:

    // Constructor and destructor

    // Constructor that receives as parameters the FileParametersObject and the initial sudoku cells
    GeneticAlgorithm(FileParametersObject fileParameters, std::vector<int> initialSudokuCells);
    // Standard destructor
    ~GeneticAlgorithm() {}

    // Method that initialized the population
    void initializePopulation();

    // Method that runs the algorithm
    void runGeneticAlgorithm();

    // Method that runs one generation
    void runOneGeneration();

    // Method that checks if the stop condition is satisfied or not
    bool checkStopCondition();

    // Method that checks whether the solution has been reached or not
    bool checkSolution();

    // Getter for m_generation member
    int getGeneration();

    // Getter for the solution (or the best solution) get
    IndividualClass getSolution();

    // Method that prints m_solution on stdout
    void printReachedSolution();

    // Geters for m_bestIndividualOfEachGen and m_avgIndividualOfEachGen
    std::vector<int> getBestIndividualsPerGenArray();
    std::vector<float> getAvgIndividualsPerGenArray();

};

// Constructor
template <class PopulationObject, class IndividualClass, class FileParametersObject>
GeneticAlgorithm<PopulationObject, IndividualClass, FileParametersObject>::
GeneticAlgorithm(FileParametersObject fileParameters, std::vector<int> initialSudokuCells)
{
    // Saves the parameters on class members
    m_fileParameters = fileParameters;
    m_initialSudokuCells = initialSudokuCells;

    // Initialized the rest of members that need to be
    m_solutionFind = false;
    m_generation = 0;

    // Set the time 0
    m_ceroTime = std::chrono::steady_clock::now();
}

// Method that initialized the population
template <class PopulationObject, class IndividualClass, class FileParametersObject>
void GeneticAlgorithm<PopulationObject, IndividualClass, FileParametersObject>::
initializePopulation()
{

    // Instaciates the dynamic PopulationObject
    m_population = new PopulationObject(m_fileParameters, m_initialSudokuCells);

    // Initializes it
    m_population->initializePopulation();
}

// Method that runs the genetic algorithm
template <class PopulationObject, class IndividualClass, class FileParametersObject>
void GeneticAlgorithm<PopulationObject, IndividualClass, FileParametersObject>::
runGeneticAlgorithm()
{
    do
    {
        // Updates the generation number
        m_generation++;

        // Runs one generation until the stop condition is satisfied
        runOneGeneration();

        // Gets best and average fitness of the gen and adds it to the corresponding array
        int bestFitnessValue = this->m_population->getTheBestFitnessValue();
        float avgFitnessValue = this->m_population->getAverageFitnessValue();
        m_bestIndividualOfEachGen.push_back(bestFitnessValue);
        m_avgIndividualOfEachGen.push_back(avgFitnessValue);

        // Writes a log on stdout
        if(m_generation % m_fileParameters.logEachXGenerations == 0)
            std::cout << "Generation number " << m_generation << " finished.\n\tBest individuals fitness = " << bestFitnessValue
            << "\n\tAverage fitness value = " << avgFitnessValue << std::endl << std::endl;

        // Update elapsed time
        auto time = std::chrono::steady_clock::now();
        m_elapsedTime = std::chrono::duration_cast<std::chrono::seconds>(time-m_ceroTime).count();

    }
    while (!checkStopCondition());

}

// Method that runs one generation
template <class PopulationObject, class IndividualClass, class FileParametersObject>
void GeneticAlgorithm<PopulationObject, IndividualClass, FileParametersObject>::
runOneGeneration()
{
    // Initialized parents and descendency arrays
    std::vector<IndividualClass> parentsArray, descendencyArray;

    // Parent selection
    parentsArray = m_population->selectParentsFromPopulation();

    // Recombination
    descendencyArray = m_population->recombineParents(parentsArray, m_generation);

    // Mutation
    m_population->mutatePopulationDescendency(descendencyArray);    

    // Survival selection
    m_population->selectSurvivals(descendencyArray);

    // Update of the ages of individuals
    m_population->updateAgeOfPopulation(m_generation);

}

// Method that checks the stop condition
template <class PopulationObject, class IndividualClass, class FileParametersObject>
bool GeneticAlgorithm<PopulationObject, IndividualClass, FileParametersObject>::
checkStopCondition()
{
    // Gets the best fitness value
    auto bestFitnessValue = m_population->getTheBestFitnessValue();

    // If it is 0 -> Solution found
    if(bestFitnessValue == 0 )
    {
        m_solutionFind = true;
        m_solution = m_population->getTheBestIndividual();
        std::cout << "SOLUTION FOUND\n";

        // Prints the solution on the screen
        std::cout << "Solution:\n";
        printReachedSolution();

        // The stop condition is satisfied
        return true;
    }

    // If the max number of generations has been reached
    if(m_generation >= m_fileParameters.maxGenerations && m_fileParameters.maxGenerations != 0)
    {
        std::cout << "Last generation finished. NO SOLUTION FOUND\n";
        m_solution = m_population->getTheBestIndividual();

        // Prints the best individual on the screen
        std::cout << "Best individual:\n";
        printReachedSolution();

        // The stop condition is satisfied
        return true;
    }

    if(m_elapsedTime > m_fileParameters.maxSecondsOfExecution && m_fileParameters.maxSecondsOfExecution != 0)
    {
        std::cout << "Timeout. NO SOLUTION FOUND\n" << std::endl;
        m_solution = m_population->getTheBestIndividual();

        // Prints the best individual on the screen
        std::cout << "Best individual:\n";
        printReachedSolution();

        // The stop condition is satisfied
        return true;
    }
        
    // The stop condition is not satisfied
    return false;
}

// Method that returns m_solutionFind
template <class PopulationObject, class IndividualClass, class FileParametersObject>
bool GeneticAlgorithm<PopulationObject, IndividualClass, FileParametersObject>::
checkSolution()
{
    return m_solutionFind;
}

// Getter for m_generation member
template <class PopulationObject, class IndividualClass, class FileParametersObject>
int GeneticAlgorithm<PopulationObject, IndividualClass, FileParametersObject>::
getGeneration()
{
    return m_generation;    
}

// Method that returns the solution
template <class PopulationObject, class IndividualClass, class FileParametersObject>
IndividualClass GeneticAlgorithm<PopulationObject, IndividualClass, FileParametersObject>::
getSolution()
{
    return m_solution;
}

// Method that prints m_solution on stdout
template <class PopulationObject, class IndividualClass, class FileParametersObject>
void GeneticAlgorithm<PopulationObject, IndividualClass, FileParametersObject>::
printReachedSolution()
{
    for(int i=1; i<=81; i++)
    {
        auto genotype = m_solution.getGenotype();
        std::cout << genotype[i-1].cellValue;
        if(i % 9 == 0)
            std::cout << "\n";
        else
        {
                std::cout << ", ";
        }   
    }
}

// Geters for m_bestIndividualOfEachGen and m_avgIndividualOfEachGen
template <class PopulationObject, class IndividualClass, class FileParametersObject>
std::vector<int> GeneticAlgorithm<PopulationObject, IndividualClass, FileParametersObject>::
getBestIndividualsPerGenArray()
{
    return m_bestIndividualOfEachGen;
}

template <class PopulationObject, class IndividualClass, class FileParametersObject>
std::vector<float> GeneticAlgorithm<PopulationObject, IndividualClass, FileParametersObject>::
getAvgIndividualsPerGenArray()
{
    return m_avgIndividualOfEachGen;
}