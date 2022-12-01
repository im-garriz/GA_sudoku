#pragma once // Multiple inclusion guardian

#include "fileParameters.hpp" // FileParameters
#include "utils.hpp" // split_string
#include <string> // std::string
#include <fstream> // std::ifstream
#include <vector> // std::vector
#include <iostream> // std::cout, std::endl

/*

FileParametersReader class:

A class that reads all the configuration data from the configuration file

*/

class FileParametersReader
{

// ATRIBUTES // 

private:
    // FileParameters struct where read data will be stored
    FileParameters m_fileParameters;

    // Path+name to the configutarion file
    std::string m_filepath;

// METHODS //

public:

    // Constructor and destructor

    // Constructor takes the path+file to be read
    FileParametersReader(std::string filepath);

    // Standard destructor
    ~FileParametersReader() {}

    // Getter for m_fileParameters member
    FileParameters getFileParameters();

private:

    // Method that reads the parameters from the file and stores them on m_fileParameters
    void readParametersFromFile();

    // Method that process each line of the file
    void processLine(std::vector<std::string> line);

    // Method that checks whether a line is a comment or not
    bool lineIsAComment(std::string line);

};

// Constructor
FileParametersReader::FileParametersReader(std::string filepath)
{
    // Initialized the m_fileParameters member
    FileParameters fileParameters;
    m_fileParameters = fileParameters;

    // Saves the path on m_filepath and reads the file
    m_filepath = filepath;
    readParametersFromFile();
}

// Getter of m_fileParameters
FileParameters FileParametersReader::getFileParameters()
{
    return m_fileParameters;
}

// Method that reads the file and completes m_fileParameters
void FileParametersReader::readParametersFromFile()
{

    // Initializes the container for each line of the file
    std::string fileLine;

    // Creates a stream of the file
    std::ifstream configurationFile(m_filepath);

    // Opens the file
    if(configurationFile.is_open())
    {
        // For each line on file
        while(std::getline(configurationFile, fileLine))
        {
            // If it is not a comment
            if(!(lineIsAComment(fileLine)))
            {
                // Split it with delimiter '='
                std::vector<std::string> splitedLine;
                split_string(fileLine, splitedLine, '=');

                // Process it
                if(splitedLine.size() > 0)
                    processLine(splitedLine);
            }
        }
    }
    else
    {
        // If file could not be opened, write an error log
        std::cout << "Cannot open configuration file in:\n" <<  m_filepath << std::endl;
    }
    
}

// Method that process each line of the file. As the name of each variable is the
// same on the file and on FileParameters struct, the code is autoexplicative
void FileParametersReader::processLine(std::vector<std::string> line)
{

    if(line.size() == 2)
    {
        if(line.at(0) == "randomSeed")
            m_fileParameters.randomSeed = std::stoi(line.at(1));
        else if(line.at(0) == "sudokuFilePath")
            m_fileParameters.sudokuFilePath = "./data/sudokus/" + line.at(1);    
        else if(line.at(0) == "populationLength")
        {
            m_fileParameters.populationLength = std::stoi(line.at(1));
            // If its a odd number, increase it by 1 so that is becomes a even
            // number -> parent selection needs even numbers
            if(m_fileParameters.populationLength % 2 > 0)
            {
                m_fileParameters.populationLength++;
            } 
        }
        else if(line.at(0) == "maxGenerations")
            m_fileParameters.maxGenerations = std::stoi(line.at(1));
        else if(line.at(0) == "maxSecondsOfExecution")
            m_fileParameters.maxSecondsOfExecution = std::stoi(line.at(1));
        else if(line.at(0) == "logEachXGenerations")
            m_fileParameters.logEachXGenerations = std::stoi(line.at(1));
        else if(line.at(0) == "maximizationProblem")
            m_fileParameters.maximizationProblem = std::stoi(line.at(1));
        else if(line.at(0) == "parentSelectionMethod")
            m_fileParameters.parentSelectionMethod = std::stoi(line.at(1));
        else if(line.at(0) == "lambda_nOfParentsSelected")
        {
            if(line.at(1) == "0")
                m_fileParameters.lambda_nOfParentsSelected = m_fileParameters.populationLength;
            else
                m_fileParameters.lambda_nOfParentsSelected = std::stoi(line.at(1));
        }
        else if(line.at(0) == "pc_crossoverProb")
            m_fileParameters.pc_crossoverProb = std::stof(line.at(1));
        else if(line.at(0) == "gamma_tournamentSelection")
            m_fileParameters.gamma_tournamentSelection = std::stoi(line.at(1));
        else if(line.at(0) == "p_bestParentSelectedProb")
            m_fileParameters.p_bestParentSelectedProb = std::stof(line.at(1));
        else if(line.at(0) == "excludingTournament")
            m_fileParameters.excludingTournament = std::stoi(line.at(1));
        else if(line.at(0) == "pm_mutationProb")
            m_fileParameters.pm_mutationProb = std::stof(line.at(1));
        else if(line.at(0) == "mutation_method")
            m_fileParameters.mutation_method = std::stoi(line.at(1));
        else if(line.at(0) == "generationalModel")
            m_fileParameters.generationalModel = std::stoi(line.at(1));
        else if(line.at(0) == "elitism")
            m_fileParameters.elitism = std::stoi(line.at(1));

    }
    
}

// Checks whether a line is a comment (starts with #) or not
bool FileParametersReader::lineIsAComment(std::string line)
{
    if(line.size() > 0)
    {
        if(line.at(0) == '#')
            return true;
        else
            return false;
    }
    else
    {
        return false;
    }
}