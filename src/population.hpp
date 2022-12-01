#pragma once // Multiple inclusion guardian

#include <vector> // std::vector
#include <set> // std::set
#include <algorithm> // std::min_element, std::max_element and std::accumulate
#include <iterator> // std::vector<T>::iterator and std::set<T>::iterator

#include "utils.hpp" // Random distributions

// To refer to an array of individuals (std::vector<IndividualClass>) more intuitively
#define arrayOfIndividuals std::vector<IndividualClass>

/* 

Population class:

A class template that represents a population of a genetic algorithm.  As it is an abstract class, a new class
that inherits from this one must be created in order to represent the population of a certain
optimization task. Therefore, the following virtual functions must be defined:

    virtual arrayOfIndividuals recombineParents(arrayOfIndividuals parents) = 0;

    virtual void selectSurvivals(arrayOfIndividuals currentPopulationDescendency) = 0;

Those functions represent the recombination and survival selection steps respectively, as they will
be different on each optimization task. Conversely, as parent selection is always the same (only depends on
the fitness value of each individual + some other generic parameters like age of each individual and elitism), it
is defined here so that it can be always used in any generic algorithm that uses this class template without
having to redefine it again.

Template types of this class are the following:

    template <class RepresentationType, class FitnessValueType, class IndividualClass>

where RepresentationType and FitnessValueType are the same as at individual.hpp. IndividualClass
represents the data type of each indiviudal. That is to say, it represents the class that will
inherit from Individual (individual.hpp) and is going to represent an individual of the population.

*/

template <class RepresentationType, class FitnessValueType, class IndividualClass>
class Population
{

// ATTRIBUTES //

// m_ prefix means member

// Protected so that the child class that inherints from this can access this class's attributes
protected:

    // Array of individuals that contain the current population
    arrayOfIndividuals m_populationArray;

    // Current lenght of the population
    unsigned int m_populationLength;

    // Length of the genotype of each individual
    int m_lengthOfTheGenotype;

    // Gamma parameter of tournament selection
    unsigned int m_gamma;

    // Prob of the selection of the best individual of the tournament in tournament selection
    float m_p_bestParentSelectedProb;

    // Whether it is a maximization or minimization 
    bool m_maximization;

    // Whether the tournament selection is excluding or not
    bool m_excludingTournament;

    // Parent selection method: 0->tournament selection
    int m_parentSelectionMethod;

    // Number of parents selected on each recombination step
    int m_lambda_nOfParentsSelected;

    // Probability of mutation
    float m_pm_mutationProb;

    // Probability of crossover
    float m_pc_crossoverProb;

    // Number of generations on which the best individuals fitness value has not changed
    unsigned int m_gensWithoutChangingMaxFitness;

    // Fitness value of the best individual of the last population
    FitnessValueType m_lastBestFitnessValue;

public:

    // PURE VIRTUAL FUNCTIONS //

    // Recombination step
    virtual arrayOfIndividuals recombineParents(arrayOfIndividuals parents, int generation) = 0;

    // Survival selection step
    virtual void selectSurvivals(arrayOfIndividuals currentPopulationDescendency) = 0;

    // CONSTRUCTOR AND DESTRUCTOR //

    // Standard onstructor and destructor
    Population() {}
    ~Population() {}

    // Parent selection step
    arrayOfIndividuals selectParentsFromPopulation();

    // Mutation step
    void mutatePopulationDescendency(arrayOfIndividuals& descendency);

    // Best individual and it's fitness value
    FitnessValueType getTheBestFitnessValue();
    IndividualClass getTheBestIndividual();

    // Average fitness value of the population
    float getAverageFitnessValue();

    // Different parent selection method
    arrayOfIndividuals tournamentSelection();

    // Method used in tournament selection fot getting individuals
    IndividualClass getIndividualFromTournament(std::set<int>& selectableParents);

    // Update age of population
    void updateAgeOfPopulation(int currentGen);

    // Geter for m_populationArray
    arrayOfIndividuals getPopulation();
};

// Parent selection
template <class RepresentationType, class FitnessValueType, class IndividualClass>
arrayOfIndividuals Population<RepresentationType, FitnessValueType, IndividualClass>::
selectParentsFromPopulation()
{
    // Depending on the method used (m_parentSelectionMethod), it cals the
    // respective method
    switch (m_parentSelectionMethod)
    {
    case 0:
        return tournamentSelection();
        break;
    
    default:
        return tournamentSelection();
        break;
    }
}

// Mutations
template <class RepresentationType, class FitnessValueType, class IndividualClass>
void Population<RepresentationType, FitnessValueType, IndividualClass>::
mutatePopulationDescendency(arrayOfIndividuals& descendency)
{
    // Foreach individual in population
    auto it = descendency.begin();
    while(it != descendency.end())
    {
        // Mutate the individual
        it->mutateIndividual(this->m_pm_mutationProb);

        it++;
    }
}

// Getter for the fitness value of the best individual
template <class RepresentationType, class FitnessValueType, class IndividualClass>
FitnessValueType Population<RepresentationType, FitnessValueType, IndividualClass>::
getTheBestFitnessValue()
{
    // Each fitness value will be stored on this array
    std::vector<FitnessValueType> fitnessValuesOnPopulation;

    // For each individual in population
    auto it = this->m_populationArray.begin();
    while(it != this->m_populationArray.end())
    {
        // Get its fitness value
        FitnessValueType fitnessValue = it->getFitnessValue();

        // Add it to fitnessValuesOnPopulation
        fitnessValuesOnPopulation.push_back(fitnessValue);

        it++;
    }

    // Initialize the pointer that will point to the best fitness value in the next step
    auto best_fitness_it = fitnessValuesOnPopulation.begin();

    // Get the best fitness value depending of it is a maximization or minimization task
    if(m_maximization)
        best_fitness_it = std::max_element(fitnessValuesOnPopulation.begin(), fitnessValuesOnPopulation.end());
    else
        best_fitness_it = std::min_element(fitnessValuesOnPopulation.begin(), fitnessValuesOnPopulation.end());

    // Return the best fitness value
    return (*best_fitness_it);  
}

// Getter for the best individual of the population
template <class RepresentationType, class FitnessValueType, class IndividualClass>
IndividualClass Population<RepresentationType, FitnessValueType, IndividualClass>::
getTheBestIndividual()
{
    // Each fitness value will be stored on this array
    std::vector<FitnessValueType> fitnessValuesOnPopulation;

    // Foreach individual in population
    auto it = this->m_populationArray.begin();
    while(it != this->m_populationArray.end())
    {
        FitnessValueType fitnessValue = it->getFitnessValue();
        fitnessValuesOnPopulation.push_back(fitnessValue);
        it++;
    }

    // Initialize the pointer that will point to the best fitness value in the next step
    auto best_fitness_it = fitnessValuesOnPopulation.begin();

    // Get the best fitness value depending of it is a maximization or minimization task
    if(m_maximization)
        best_fitness_it = max_element(fitnessValuesOnPopulation.begin(), fitnessValuesOnPopulation.end());
    else
        best_fitness_it = min_element(fitnessValuesOnPopulation.begin(), fitnessValuesOnPopulation.end());

    // Get the position of the best individual in the array
    int bestIndividualIdx = std::distance(fitnessValuesOnPopulation.begin(), best_fitness_it);

    // Get the best individual
    IndividualClass bestIndividual = this->m_populationArray.at(bestIndividualIdx);

    // Return it
    return bestIndividual;
}

// Average fitness value of the population
template <class RepresentationType, class FitnessValueType, class IndividualClass>
float Population<RepresentationType, FitnessValueType, IndividualClass>::
getAverageFitnessValue()
{
    // Each fitness value will be stored on this array
    std::vector<FitnessValueType> fitnessValuesOnPopulation;

    // For each individual in population
    auto it = this->m_populationArray.begin();
    while(it != this->m_populationArray.end())
    {
        // Get its fitness value
        FitnessValueType fitnessValue = it->getFitnessValue();

        // Add it to fitnessValuesOnPopulation
        fitnessValuesOnPopulation.push_back(fitnessValue);

        it++;
    }

    // Gets the sum of all fitness values
    int sumOfVector = std::accumulate(fitnessValuesOnPopulation.begin(), fitnessValuesOnPopulation.end(), 0);

    // Calculates the average
    float averageFitnessValue = static_cast<float>(sumOfVector) / static_cast<float>(fitnessValuesOnPopulation.size());

    return averageFitnessValue;
}

// Tournament selection
template <class RepresentationType, class FitnessValueType, class IndividualClass>
arrayOfIndividuals Population<RepresentationType, FitnessValueType, IndividualClass>::
tournamentSelection()
{
    // Array on which parents will be stored
    arrayOfIndividuals parents_array;

    // Initialize the set containing all selectable parents indexn (all population)
    std::set<int> selectableParents;
    for(int i=0; i<m_populationLength; i++)
    {
        selectableParents.insert(i);
    }

    // Select lambda parents and store then on parents_array
    for(int i=0; i < m_lambda_nOfParentsSelected; i++)
    {
        IndividualClass parent = getIndividualFromTournament(selectableParents);
        parents_array.push_back(parent);
    }
    
    // return the parents array
    return parents_array;
}

// Method that gets individuals using tournament selection
template <class RepresentationType, class FitnessValueType, class IndividualClass>
IndividualClass Population<RepresentationType, FitnessValueType, IndividualClass>::
getIndividualFromTournament(std::set<int>& selectableParents)
{

    // Copy selectableParents
    std::set<int> selectableParentsInThisTournament = selectableParents;

    // Initilalize tournament winner index and fitness variables
    FitnessValueType tournamentWinnerFitnessValue;
    int tournamentWinnerIndex;

    // Array where the indexes of the individuals of the tournament will be stored
    std::vector<int> tournamentIndividualsIndexes;
    tournamentIndividualsIndexes.reserve(m_gamma);

    // Get a random element from selectableParentsInThisTournament
    int index = get_random_element_setOfIntegers(selectableParentsInThisTournament);

    // Set it as the winner
    tournamentWinnerIndex = index;
    tournamentWinnerFitnessValue = this->m_populationArray.at(index).getFitnessValue();

    // Add it to tournamentIndividualsIndexes
    tournamentIndividualsIndexes.emplace_back(index);

    // Delete it from selectableParentsInThisTournament as it cannot be selected twice on
    // the same tournament
    selectableParentsInThisTournament.erase(index);

    // Get the rest of individuals of the tournament until having gamma individuals
    for(int i=0; i<m_gamma-1; i++)
    {
        // Get random individual and its fitness value
        index = get_random_element_setOfIntegers(selectableParentsInThisTournament);
        FitnessValueType fitnessValue = this->m_populationArray.at(index).getFitnessValue();

        // Add it to tournamentIndividualsIndexes
        tournamentIndividualsIndexes.emplace_back(index);

        // Delete it from selectableParentsInThisTournament as it cannot be selected twice on
        // the same tournament
        selectableParentsInThisTournament.erase(index);

        // Evaluate the individual and if its better than the one that was the best, update
        // tournamentWinnerFitnessValue and tournamentWinnerIndex
        if(m_maximization)
        {
            if(fitnessValue > tournamentWinnerFitnessValue)
            {
                tournamentWinnerFitnessValue = fitnessValue;
                tournamentWinnerIndex = index;
            }
        }
        else
        {
            if(fitnessValue < tournamentWinnerFitnessValue)
            {
                tournamentWinnerFitnessValue = fitnessValue;
                tournamentWinnerIndex = index;
            }
        }
        
    }

    // Variable where the winner will be stored
    IndividualClass tournamentWinner;

    // Get random float number and compare it with m_p_bestParentSelectedProb (probability of selecting the best parent)
    float randomValue = get_random_float_0_to_1();
    if(randomValue <= m_p_bestParentSelectedProb)
        // Select the tournament winner
        tournamentWinner = this->m_populationArray.at(tournamentWinnerIndex);
    else
    {
        // Select a random individual from the tournament
        int randomIndex = get_random_integer(0, tournamentIndividualsIndexes.size()-1);
        tournamentWinnerIndex = tournamentIndividualsIndexes.at(randomIndex);
        tournamentWinner = this->m_populationArray.at(tournamentWinnerIndex);
    }
    
    // If excluding tournament selection, exclude the winner so that it will no be choosen again
    if(m_excludingTournament)
        selectableParents.erase(tournamentWinnerIndex);
    
    // Return the tournament winner
    return tournamentWinner;
}

// Update age of population
template <class RepresentationType, class FitnessValueType, class IndividualClass>
void Population<RepresentationType, FitnessValueType, IndividualClass>::
updateAgeOfPopulation(int currentGen)
{
    auto it = this->m_populationArray.begin();
    while(it != this->m_populationArray.end())
    {
        it->updateAgeOfIndividual(currentGen);
        it++;
    }
}

// Geter for m_populationArray
template <class RepresentationType, class FitnessValueType, class IndividualClass>
arrayOfIndividuals Population<RepresentationType, FitnessValueType, IndividualClass>::
getPopulation()
{
    return m_populationArray;
}