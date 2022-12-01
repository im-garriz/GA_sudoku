#pragma once // Multiple inclusion guardian

#include <vector> // std::vector
#include <iostream> //borrar

/*

Individual class:

A class template that represents a generic individual. As it is an abstract class, a new class
that inherits from this one must be created in order to represent the individual of a certain
optimization task. Therefore, the following virtual functions must be defined:

    virtual void initializeIndividual() = 0;

    virtual void calculateFitnessValue() = 0;

    virtual void mutateIndividual(float pm) = 0;

As both the representation type and the fitness value type may be different depending on the task,
they have been declared ad templates, with the following names:

    template <class RepresentationType, class FitnessValueType>

Where RepresentationType is the data type of which the genotype is going to be composed of and
FitnessValueType is the data type of the fitness function value (int or float most times)

*/

template <class RepresentationType, class FitnessValueType>
class Individual
{

// ATTRIBUTES //

// m_ prefix means member

// Protected so that the child class that inherints from this can access this class's attributes
protected:
    // Number of genes the genotype has
    unsigned int m_lengthOfTheGenotype;
    
    // A chain of a certain data type that represents the genotype of the individual
    std::vector<RepresentationType> m_genotype;

    // A value (normally int or float/double) that represents the fitness value of the individual
    FitnessValueType m_fitnessValue;

    // Generation on that the individual was created
    unsigned int m_creationGen;
    
    // Age of the individual (currentGen - creationGen + 1)
    unsigned int m_age;

// METHODS //

public:

    // PURE VIRTUAL FUNCTIONS //
    // Any individual of any kind in a genetic algorithm will have at least the following methods:

    // A initializer for the genotype (random or informed)
    virtual void initializeIndividual() = 0;
    // A fitness value (provided a fitness function)
    virtual void calculateFitnessValue() = 0;
    // A mutation method
    virtual void mutateIndividual(float pm) = 0;

    // As their nature will depend on representation, fitness function etc., they have
    // been created as pure virtual methods, and they will be defined o a class that
    // inherits from this one

    // CONSTRUCTORS //

    // Genotype of initial population will be initialized by initializeIndividual()
    Individual(int creationGen, unsigned int lengthOfTheGenotype);

    // Genotype of post-initial populations will get their genotype from recombination
    Individual(int creationGen, unsigned int lengthOfTheGenotype, std::vector<RepresentationType> genotype);

    // Standard constructor
    Individual(){}

    // STANDARD DESTRUCTOR //

    ~Individual(){};

    // SETERS, GETTERS AND UPDATERS //

    // Getter and setter for the length of the genotype
    unsigned int getLengthOfTheGenotype();
    void setLengthOfTheGenotype(unsigned int lengthOfTheGenotype);

    // Setter and getter for genotype
    void updateGenotype(std::vector<RepresentationType> genotype);
    std::vector<RepresentationType> getGenotype();

    // Getter for fitnessValue (not a setter because it must not be changed unless it is changed by calculateFitnessValue())
    FitnessValueType getFitnessValue();

    // setter for creation gen
    void setCreationGen(int creationGen);

    // Updater and getter for age
    void updateAgeOfIndividual(unsigned int currentGen);
    unsigned int getIndividualAge();

};

// IMPLEMENTATIONS OF METHODS OF Individual CLASS //

// CONSTRUCTORS //

template <class RepresentationType, class FitnessValueType>
Individual<RepresentationType, FitnessValueType>::
Individual(int creationGen, unsigned int lengthOfTheGenotype):
m_creationGen(creationGen), m_lengthOfTheGenotype(lengthOfTheGenotype)
{

}

template <class RepresentationType, class FitnessValueType>
Individual<RepresentationType, FitnessValueType>::
Individual(int creationGen, unsigned int lengthOfTheGenotype, std::vector<RepresentationType> genotype):
m_creationGen(creationGen), m_lengthOfTheGenotype(lengthOfTheGenotype)
{
    m_genotype = genotype;
}


// SETERS, GETTERS AND UPDATERS //

// Getter for m_lengthOfTheGenotype
template <class RepresentationType, class FitnessValueType>
unsigned int Individual<RepresentationType, FitnessValueType>::
getLengthOfTheGenotype()
{
    return m_lengthOfTheGenotype;
}

// Setter for m_lengthOfTheGenotype 
template <class RepresentationType, class FitnessValueType>
void Individual<RepresentationType, FitnessValueType>::
setLengthOfTheGenotype(unsigned int lengthOfTheGenotype)
{
    m_lengthOfTheGenotype = lengthOfTheGenotype;
}

// Updater for m_genotype
template <class RepresentationType, class FitnessValueType>
void Individual<RepresentationType, FitnessValueType>::
updateGenotype(std::vector<RepresentationType> genotype)
{
    m_genotype = genotype;
}

// Getter for m_genotype
template <class RepresentationType, class FitnessValueType>
std::vector<RepresentationType> Individual<RepresentationType, FitnessValueType>::
getGenotype()
{
    return m_genotype;
}

// Getter for m_fitnessValue
template <class RepresentationType, class FitnessValueType>
FitnessValueType Individual<RepresentationType, FitnessValueType>::
getFitnessValue()
{
    return m_fitnessValue;
}

// Setter for creation gen
template <class RepresentationType, class FitnessValueType>
void Individual<RepresentationType, FitnessValueType>::
setCreationGen(int creationGen)
{
    m_creationGen = creationGen;
}

// Updater for m_age
template <class RepresentationType, class FitnessValueType>
void Individual<RepresentationType, FitnessValueType>::
updateAgeOfIndividual(unsigned int currentGen)
{
    // The age of a individual is the diference between the current gen and the gen in which it was created
    // (+1 assuming that the first gen is number 1, not number 0)
    m_age = currentGen - m_creationGen + 1;
}

// Getter for m_age
template <class RepresentationType, class FitnessValueType>
unsigned int Individual<RepresentationType, FitnessValueType>::
getIndividualAge()
{
    return m_age;
}