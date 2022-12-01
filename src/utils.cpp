#include "utils.hpp"

// Takes the RandomGenerator defined in main.cpp
extern std::mt19937 RandomGenerator;

// Funciton that returns a number between 1 and 9 randomly
int get_random_number_1_to_9()
{
    std::uniform_int_distribution<> uniform_int_dist(1, 9);
    return uniform_int_dist(RandomGenerator);
}

// Function that return a float betwen 0 and 1 randomly
float get_random_float_0_to_1()
{
    std::uniform_real_distribution<> uniform_real_dist(0, 1);
    return uniform_real_dist(RandomGenerator);
}

// Function that returns a random integer, providin the upper and lower limits
int get_random_integer(int lower, int upper)
{
    std::uniform_int_distribution<> uniform_int_dist(lower, upper);
    return uniform_int_dist(RandomGenerator);
}

// Function that return a random element from a std::set of integers
int get_random_element_setOfIntegers(std::set<int> container)
{
    // Gets a random idx of the set
    int position = get_random_integer(0, container.size() - 1);

    // As std::set<>::iterator is not random access, it must be incremented
    // on a loop
    auto set_iter = container.begin();
    for(int j = 0; j<position; j++)
    {
        set_iter++;
    }

    // Return the value of the std::set on position
    return (*set_iter);
}