#pragma once // Multiple inclusion guardian

#include "population.hpp" // Population class
#include "fileParameters.hpp" // FileParameters class
#include <iostream> //std::cout, std::endl
#include <algorithm> // std::copy
#include <vector> // std::vector

/*

SDK_Population class:

A class template that represents the population of the sudoku problem. As it is
a population of a GA, it inherits from Population (population.hpp)

It has the same template parameters as Population (population.hpp):

    template <class RepresentationType, class FitnessValueType, class IndividualClass>


*/

template <class RepresentationType, class FitnessValueType, class IndividualClass>
class SDK_Population : public Population<RepresentationType, FitnessValueType, IndividualClass>
{

// ATRIBUTES //

private:

    // Vector of 81 ints that contains the initial values (read from a file) of
    // the sudoku problem to be solved
    std::vector<int> m_initialSudokuCells;

    // FileParameters type member that contain all file parameters
    FileParameters m_fileParameters;


// METHODS //

public:

    // CONSTRUCTOR AND DESTRUCTOR //

    // Constructor
    SDK_Population(FileParameters fileParameters, std::vector<int> initialSudokuCells);
    // Standard destructor
    ~SDK_Population() {}

    // Methods corresponding to the recombination step
    arrayOfIndividuals recombineParents(arrayOfIndividuals parents, int generation);
    arrayOfIndividuals onePointRecombination(arrayOfIndividuals parents, int generation);

    // Methods corresponding to survival selection step
    void selectSurvivals(arrayOfIndividuals currentPopulationDescendency);
    void survivalSelection_generationalModel();

    // Initializer of the population
    void initializePopulation();

    // Getter of the best individual provided an array of them
    IndividualClass getBestIndividualOfDescendency(arrayOfIndividuals descendency);

};

// Constructor
template <class RepresentationType, class FitnessValueType, class IndividualClass>
SDK_Population<RepresentationType, FitnessValueType, IndividualClass>::
SDK_Population(FileParameters fileParameters, std::vector<int> initialSudokuCells):
Population<RepresentationType, FitnessValueType, IndividualClass>()
{
    // Sets the m_fileParameters member
    m_fileParameters = fileParameters;

    // Sets all parameters defined on population.hpp
    this->m_populationLength = fileParameters.populationLength;
    this->m_lengthOfTheGenotype = initialSudokuCells.size();
    this->m_gamma = fileParameters.gamma_tournamentSelection;
    this->m_p_bestParentSelectedProb = fileParameters.p_bestParentSelectedProb;
    this->m_maximization = fileParameters.maximizationProblem;
    this->m_excludingTournament = fileParameters.excludingTournament;
    this->m_parentSelectionMethod = fileParameters.parentSelectionMethod;
    this->m_lambda_nOfParentsSelected = fileParameters.lambda_nOfParentsSelected;
    this->m_pm_mutationProb = fileParameters.pm_mutationProb;
    this->m_pc_crossoverProb = fileParameters.pc_crossoverProb;

    // Sets the m_initialSudokuCells member
    m_initialSudokuCells = initialSudokuCells;

    // Allocates memory for population
    this->m_populationArray.reserve(fileParameters.populationLength);
}

// Recombination step
template <class RepresentationType, class FitnessValueType, class IndividualClass>
arrayOfIndividuals SDK_Population<RepresentationType, FitnessValueType, IndividualClass>::
recombineParents(arrayOfIndividuals parents, int generation)
{
    // Calls the corresponding recombination method (read from the file)
    return onePointRecombination(parents, generation);
}

// Implementation of one point recombination
template <class RepresentationType, class FitnessValueType, class IndividualClass>
arrayOfIndividuals SDK_Population<RepresentationType, FitnessValueType, IndividualClass>::
onePointRecombination(arrayOfIndividuals parents, int generation)
{
    // Initialized the array to be returned
    arrayOfIndividuals sons;
    sons.reserve(parents.size());

    // Gets the crossover point randomly
    int crossoverPoint = get_random_integer(1, this->m_lengthOfTheGenotype);

    // For each pair of parents
    for(int i=0; i<parents.size(); i+=2)
    {
        // Copies each parent to parent1 or parent2
        IndividualClass parent1 = parents.at(i);
        IndividualClass parent2 = parents.at(i+1);

        // Do the crossover in function of m_pc_crossoverProb
        float randomProb = get_random_float_0_to_1();
        if(randomProb <= this->m_pc_crossoverProb)
        {
            // Intializes sons 1 and 2
            IndividualClass son1;
            IndividualClass son2;

            // Copies the genotype of parent1 to son2 and the one of parent2 to son1
            std::vector<RepresentationType> parent1_genotype = parent1.getGenotype();
            std::vector<RepresentationType> parent2_genotype = parent2.getGenotype();
            std::vector<RepresentationType> son1_genotype = parent2_genotype;
            std::vector<RepresentationType> son2_genotype = parent1_genotype;

            // Copies the genotype of parent1 to son1 until the crossover point and vice versa
            std::copy(parent1_genotype.begin(), parent1_genotype.begin()+crossoverPoint, son1_genotype.begin());
            std::copy(parent2_genotype.begin(), parent2_genotype.begin()+crossoverPoint, son2_genotype.begin());

            // Sets the generated genotypes to each son
            son1.updateGenotype(son1_genotype);
            son2.updateGenotype(son2_genotype);

            #if defined DEBUG_MODE_ON
                // Checks and corrects each son (only while debugging)
                if(!son1.checkFixedModified(this->m_initialSudokuCells))
                {
                    son1.correctFixedModified(this->m_initialSudokuCells);
                    std::cout << "bad recombination\n";
                }
                
                if(!son2.checkFixedModified(this->m_initialSudokuCells))
                {
                    son2.correctFixedModified(this->m_initialSudokuCells);
                    std::cout << "bad recombination\n";
                }
            #endif
                
            // Calculates the fitnes value of the generated sons
            son1.calculateFitnessValue();
            son2.calculateFitnessValue();

            // Updates the legal values of each cell of the generated sons
            son1.updateLegalValuesOfEachCell();
            son2.updateLegalValuesOfEachCell();

            // Sets the inicial generation of each son so as to calculate their age
            son1.setCreationGen(generation);
            son2.setCreationGen(generation);

            // Adds them to the sons array
            sons.emplace_back(son1);
            sons.emplace_back(son2);
        }
        else
        {
            // If there is not crossover, parets are returned
            sons.emplace_back(parent1);
            sons.emplace_back(parent2);
        }
    }

    return sons;
}

// Survival selection step
template <class RepresentationType, class FitnessValueType, class IndividualClass>
void SDK_Population<RepresentationType, FitnessValueType, IndividualClass>::
selectSurvivals(arrayOfIndividuals currentPopulationDescendency)
{
    // Initialized the array containing the new population
    arrayOfIndividuals newPopulation;

    // If it is a generational model, descendency replaces parents
    if(m_fileParameters.generationalModel)
    {
        newPopulation = currentPopulationDescendency;
    }

    // If elitism is being applied, the best individual of the previous population is
    // always on the next population (provided that it is better than all sons)
    if(m_fileParameters.elitism)
    {
        // Gets the best individual of the current population and its fitness value
        FitnessValueType currentGenBestFitnessValue = this->getTheBestFitnessValue();
        IndividualClass bestIndividualOfDescendency = this->getBestIndividualOfDescendency(currentPopulationDescendency);

        // Gets the fitness value of the best individual on descendency
        FitnessValueType descendencyBestFitnessValue = bestIndividualOfDescendency.getFitnessValue();

        // The condition changes whether it is a maximization or minimization
        if(this->m_maximization)
        {
            // If the best parent is to be put on descendency
            if(descendencyBestFitnessValue < currentGenBestFitnessValue)
            {
                // Choose one son randomly and replace it with the best parent
                int randomIdx = get_random_integer(0, newPopulation.size()-1);
                newPopulation[randomIdx] = this->getTheBestIndividual();
            }
        }
        else
        {
            // If the best parent is to be put on descendency
            if(descendencyBestFitnessValue > currentGenBestFitnessValue)
            {
                // Choose one son randomly and replace it with the best parent
                int randomIdx = get_random_integer(0, newPopulation.size()-1);
                newPopulation[randomIdx] = this->getTheBestIndividual();
            }
        }

    }

    // Set the new population on its corresponding member
    this->m_populationArray = newPopulation;
}

// Population initialization step
template <class RepresentationType, class FitnessValueType, class IndividualClass>
void SDK_Population<RepresentationType, FitnessValueType, IndividualClass>::
initializePopulation()
{
    // m_populationLength times:
    for(int i=0; i < this->m_populationLength; i++)
    {
        // Generate a new individual
        IndividualClass individual(0, this->m_lengthOfTheGenotype, m_initialSudokuCells, this->m_fileParameters.mutation_method);

        // Initialize it
        individual.initializeIndividual();

        // Append it to m_populationArray
        this->m_populationArray.emplace_back(individual);
    }
}

// Method that returns the best individual of the descendency, providing it as an array
template <class RepresentationType, class FitnessValueType, class IndividualClass>
IndividualClass SDK_Population<RepresentationType, FitnessValueType, IndividualClass>::
getBestIndividualOfDescendency(arrayOfIndividuals descendency)
{
    // Takes the first individual of descendency
    auto it = descendency.begin();

    // Sets it as the best, as it is the first
    FitnessValueType bestFitnessValue = it->getFitnessValue();
    IndividualClass bestIndividual = *it;

    // Increases the iterator so as to start checking from the second
    it++;
    
    // For each element in descendency
    while(it != descendency.end())
    {
        // The condition changes whether it is a maximization/minimization
        if(this->m_maximization)
        {
            // Checks if it is better than the current best
            if(bestFitnessValue < it->getFitnessValue())
            {
                // If it is so, it updates it
                bestFitnessValue = it->getFitnessValue();
                bestIndividual = *it;
            }
        }
        else
        {
            // Checks if it is better than the current best
            if(bestFitnessValue > it->getFitnessValue())
            {
                // If it is so, it updates it
                bestFitnessValue = it->getFitnessValue();
                bestIndividual = *it;
            }
        }

        it++;
    }

    // Return the best individual
    return bestIndividual;
}
