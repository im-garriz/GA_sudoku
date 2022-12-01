#pragma once // Multiple inclusion guardian

#include <string> // std::string
#include <sstream> // std::stringstream
#include <random> // random distributions
#include <iterator> // std::set<>::iterator
#include <set> // std::set

// Declares the RandomGenerator as extern, as it is being initialized on utils.cpp
extern std::mt19937 RandomGenerator;

// Function template that stplits an string with a delimiter and returns the
// result on cont
template <class Container>
void split_string(const std::string& str, Container& cont, char delim)
{
    std::stringstream ss(str);
    std::string token;
    while (std::getline(ss, token, delim))
    {
        cont.push_back(token);
    }
}

// Function template that returns a random index from a container
template<typename Container>
int select_random_item(Container container)
{
    std::uniform_int_distribution<> uniform_int_dist(0, container.size()-1);
    int random_idx = uniform_int_dist(RandomGenerator);

    return random_idx;
}

// Declaration of some other functions defines in utils.cpp

// Funciton that returns a number between 1 and 9 randomly
int get_random_number_1_to_9();

// Function that return a float betwen 0 and 1 randomly
float get_random_float_0_to_1();

// Function that returns a random integer, providin the upper and lower limits
int get_random_integer(int lower, int upper);

// Function that return a random element from a std::set of integers
int get_random_element_setOfIntegers(std::set<int> container);