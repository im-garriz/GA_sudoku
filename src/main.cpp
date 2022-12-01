#include <iostream> // std::cout, std::endl
#include <random> // std::seed_seq, std::mt19937
#include <fstream> // std::ofstream
#include <algorithm> // std::accumulate, std::distance

// Some functions are for debugging purposes,so only are defined and called if
// the line below is not commented
//#define DEBUG_MODE_ON 

#include "sudokuProblemPopulation.hpp" // SDK_Population
#include "sudokuProblemIndividual.hpp" // SDK_Individual
#include "sudokuReader.hpp" // SudokuReader
#include "sudokuCell.hpp" // SudokuCell
#include "geneticAlgorithm.hpp" // GeneticAlgorithm
#include "fileParameters.hpp" // FileParameters
#include "fileParametersReader.hpp" // FileParametersReader

// Defines the types of the representation of each gen and the data type of the fitness function
#define GENOME_REPRESENTATION_TYPE SudokuCell
#define FITNESS_VALUE_DATA_TYPE int

// Defines individual and population types with their template values
#define INDIVIDUAL_TYPE SDK_Individual<GENOME_REPRESENTATION_TYPE, FITNESS_VALUE_DATA_TYPE>
#define POPULATION_TYPE SDK_Population<GENOME_REPRESENTATION_TYPE, FITNESS_VALUE_DATA_TYPE, INDIVIDUAL_TYPE>

// Function that prints the parameters read from the file on stdout
void printFileParameters(FileParameters f);

// Function that sets the random seed
void setRandomSeed(int seed);

// Function used to get the initial progress curve so that the max number of generations is logically set
void getInitialProgressCurves(FileParameters fileParameters, std::vector<int> sudoku_cells);

// Generate progress curves files
void generateProgressCurveFile(std::vector<int> bestIndividuals, std::vector<float> avgIndividuals, std::string name);

// Generate parameters for evaluation
void generateParametersForEvaluation(FileParameters fileParameters, std::vector<int> sudoku_cells, float pm, std::vector<int> seeds);

void generateLogFileWithParameters(std::vector<int> bestIndividuals, std::vector<float> avgIndividuals, int generationReached, std::string name);

// RandomGenerator object defined on utils.cpp
std::mt19937 RandomGenerator;

int main()
{
    // Initializes the FileParameters object
    FileParameters fileParameters;
    
    // Reads the configuration from the file and stores it on fileParameters
    FileParametersReader fileParametersReader("./data/parameters/genetic_algorithm_configuration.txt");
    fileParameters = fileParametersReader.getFileParameters();

    // Sets the random seed with the read value
    setRandomSeed(fileParameters.randomSeed);

    #if defined DEBUG_MODE_ON
        // Prints the read FileParameters object (debugging)
        printFileParameters(fileParameters);
    #endif

    // Reads the sudoku to be solved and creates the corresponding vector of ints
    //SudokuReader sudokuReader("./data/sudokus/easy_sudoku.txt");
    SudokuReader sudokuReader(fileParameters.sudokuFilePath);
    std::vector<int> sudoku_cells = sudokuReader.getSudoku();

    // If the sudoku has not been correcly read exits
    if(sudoku_cells[0] == -1)
        return -1;

    // Initializes the genetic algorithm
    GeneticAlgorithm<POPULATION_TYPE, INDIVIDUAL_TYPE, FileParameters> GeneticAlgorithm(fileParameters, sudoku_cells);

    // Initialized the population
    GeneticAlgorithm.initializePopulation();

    // Runs the algorithm
    GeneticAlgorithm.runGeneticAlgorithm();
    
    return 0;
}

// Function that prints the parameters read from the file on stdout
void printFileParameters(FileParameters f)
{
    std::cout << "Parameters read from file:\n";

    std::cout << "randomSeed = " << f.randomSeed << std::endl;
    std::cout << "sudokuFilePath = " << f.sudokuFilePath << std::endl;
    std::cout << "populationLength = " << f.populationLength << std::endl;
    std::cout << "maxGenerations = " << f.maxGenerations << std::endl;
    std::cout << "maximizationProblem = " << f.maximizationProblem << std::endl;
    std::cout << "parentSelectionMethod = " << f.parentSelectionMethod << std::endl;
    std::cout << "lambda_nOfParentsSelected = " << f.lambda_nOfParentsSelected << std::endl;
    std::cout << "gamma_tournamentSelection = " << f.gamma_tournamentSelection << std::endl;
    std::cout << "p_bestParentSelectedProb = " << f.p_bestParentSelectedProb << std::endl;
    std::cout << "excludingTournament = " << f.excludingTournament << std::endl;
    std::cout << "pm_mutationProb = " << f.pm_mutationProb << std::endl;
    std::cout << "mutation_method = " << f.mutation_method << std::endl;
    std::cout << "generationalModel = " << f.generationalModel << std::endl;
    std::cout << "elitism = " << f.elitism << std::endl;

    std::cout << std::endl;
}

// Function that sets the random seed
void setRandomSeed(int seed)
{
    std::seed_seq seq{seed, seed+100, seed+200};
    std::mt19937 rng(seq);
    RandomGenerator = rng;
}

/// ALL FUNCTIONS BELOW ARE USED TO GENERATE THE CORRESPONDING LOG FILES ///

// Function used to get the initial progress curve so that the max number of generations is logically set
void getInitialProgressCurves(FileParameters fileParameters, std::vector<int> sudoku_cells)
{
    // Seed thal will be used to get each curve
    const std::vector<int> randomSeeds = {66,58,45,89,123,4,8,56,148,15};

    int i = 1; // Counter that gives a name to each file

    // VAMM vs t
    auto it = randomSeeds.begin();
    while(it != randomSeeds.end())
    {
        fileParameters.randomSeed = *it;
        setRandomSeed(fileParameters.randomSeed);

        // Initializes the genetic algorithm
        GeneticAlgorithm<POPULATION_TYPE, INDIVIDUAL_TYPE, FileParameters> GeneticAlgorithm(fileParameters, sudoku_cells);

        // Initialized the population
        GeneticAlgorithm.initializePopulation();

        // Runs the algorithm
        GeneticAlgorithm.runGeneticAlgorithm();

        // Get fitness value vectors
        std::vector<int> bestFitnesses = GeneticAlgorithm.getBestIndividualsPerGenArray();
        std::vector<float> avgFitnesses = GeneticAlgorithm.getAvgIndividualsPerGenArray();

        std::string filename = std::to_string(i) + ".txt";
        generateProgressCurveFile(bestFitnesses, avgFitnesses, filename);

        i++;
        it++;
    }

}

// Generate progress curves files
void generateProgressCurveFile(std::vector<int> bestIndividuals, std::vector<float> avgIndividuals, std::string name)
{
    // Generate the file with the given name
    std::ofstream file;
    file.open ("./data/curves/easy_sudoku/first_progress_curves/" + name);

    // Write each line: generation,bestFitness,avgFitness
    for(int i=0; i < bestIndividuals.size(); i++)
    {
        file << i+1 << "," << bestIndividuals[i] << "," << avgIndividuals[i] << "\n";
    }

    // Close the file
    file.close();
}

// Generate parameters for evaluation
void generateParametersForEvaluation(FileParameters fileParameters, std::vector<int> sudoku_cells, float pm, std::vector<int> seeds)
{

    // Gets the number of executions per algorithm
    int N_of_executions = seeds.size();

    // Stores the mutation prob
    fileParameters.pm_mutationProb = pm;

    // Counter of successful executions
    int numberOfExecutions_solutionReached = 0;

    // Initializes arrays in which results will be stord
    std::vector<int> arrayOf_VAMM;
    std::vector<int> arrayOf_ElapsedGens;

    // For each different seed
    for(int i=0; i<seeds.size(); i++)
    {
        fileParameters.randomSeed = seeds[i];
        setRandomSeed(fileParameters.randomSeed);

        // Initializes the genetic algorithm
        GeneticAlgorithm<POPULATION_TYPE, INDIVIDUAL_TYPE, FileParameters> GeneticAlgorithm(fileParameters, sudoku_cells);

        // Initialized the population
        GeneticAlgorithm.initializePopulation();

        // Runs the algorithm
        GeneticAlgorithm.runGeneticAlgorithm();

        // Get fitness value vectors
        std::vector<int> bestFitnesses = GeneticAlgorithm.getBestIndividualsPerGenArray();
        std::vector<float> avgFitnesses = GeneticAlgorithm.getAvgIndividualsPerGenArray();

        // If it has found the solution increase the counter
        if(GeneticAlgorithm.checkSolution())
            numberOfExecutions_solutionReached++;

        // Append VAMM value
        arrayOf_VAMM.push_back(bestFitnesses[bestFitnesses.size()-1]);

        // Elapsed number of genrations
        int generationReached = GeneticAlgorithm.getGeneration();

        // If it has reached the solution append it to arrayOf_ElapsedGens
        if(GeneticAlgorithm.getGeneration() < fileParameters.maxGenerations)
            arrayOf_ElapsedGens.push_back(GeneticAlgorithm.getGeneration());

        // Write the results to a txt file
        generateLogFileWithParameters(bestFitnesses, avgFitnesses, generationReached, std::to_string(pm) + ".txt");
    }

    // Calculate TE
    float TE = static_cast<float>(numberOfExecutions_solutionReached) / static_cast<float>(N_of_executions);
    
    // Calculate VAMM
    int sum_of_VAMM = std::accumulate(arrayOf_VAMM.begin(), arrayOf_VAMM.end(), 0);
    float VAMM = static_cast<float>(sum_of_VAMM) / static_cast<float>(arrayOf_VAMM.size());

    // Calculcate PEX
    float PEX;
    if(!arrayOf_ElapsedGens.empty())
    {
        unsigned long sum_of_PEX = std::accumulate(arrayOf_ElapsedGens.begin(), arrayOf_ElapsedGens.end(), 0) * fileParameters.populationLength;
        PEX = static_cast<float>(sum_of_PEX) / static_cast<float>(arrayOf_ElapsedGens.size());
    }
    else
    {
        PEX = -1.0f;
    }

    // Write them into the file
    std::ofstream file;
    file.open ("./data/curves/easy_sudoku/logs/" + std::to_string(pm) + ".txt", std::ios::app);
    
    file << "TE=" << TE << std::endl;
    file << "VAMM=" << VAMM << std::endl;
    file << "PEX=" << PEX << std::endl;

    file.close();
}

void generateLogFileWithParameters(std::vector<int> bestIndividuals, std::vector<float> avgIndividuals, int generationReached, std::string name)
{
    // Open the file
    std::ofstream file;
    file.open ("./data/curves/easy_sudoku/logs/" + name, std::ios::app);

    // Write fitness value of the best individual of each gen
    auto it1 = bestIndividuals.begin();
    while (it1 != bestIndividuals.end())
    {
        file << *it1;
        if(std::distance(it1, bestIndividuals.end()) > 1)
            file << ",";
        else
            file << ";";
        
        it1++;
    }

    // Write average fitness value of each gen
    auto it2 = avgIndividuals.begin();
    while (it2 != avgIndividuals.end())
    {
        file << *it2;
        if(std::distance(it2, avgIndividuals.end()) > 1)
            file << ",";
        else
            file << ";";
        
        it2++;
    }

    // Write the reached gen
    file << generationReached << "\n";

    // Close the file
    file.close();
}
