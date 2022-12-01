#pragma once // Multiple inclusion guardian

#include <string> // std::string

/*

FileParameters struct:

A struct that groups all parameters that are read from the configuration file

*/

struct FileParameters
{

    // GENERAL SETTINGS

    // Random seed for the random distributions
    int randomSeed;
    // File of the sudoku to be solved
    std::string sudokuFilePath;
    // Length of the population
    int populationLength;
    // Max number of generations to iterate
    int maxGenerations;
    // Max number of seconds to be executing the algorithm
    int maxSecondsOfExecution;
    // Frequency of log
    int logEachXGenerations;
    // true if the problem is a maximization, false if it is a minimization
    bool maximizationProblem;

    // PARENT SELECTION SETTINGS

    // Method that will be used for parenst selction
        // 0: tournamenet selection
        // 1: ...
    int parentSelectionMethod;

    // Number of parents that will be selected on each generation
    int lambda_nOfParentsSelected;

    // Settings for tournament selection

    // Number of individuals that will form each tournament
    int gamma_tournamentSelection;
    // Probability that has each tournament winner to be selected as parent
    float p_bestParentSelectedProb;
    // Whether the tournament selection is excluding or not
    bool excludingTournament;

    // RECOMBINATIONS SETTINGS
    float pc_crossoverProb;

    // MUTATION SETTINGS

    // Probability of mutation
    float pm_mutationProb;

    int mutation_method;

    // SURVIVAL SELECTION SETTINGS

    // 1 if it is a generational model (all sons replace all individuals), 0 if  not
    bool generationalModel;
    // 1 if elitism is applied, 0 if not
    bool elitism;

};
