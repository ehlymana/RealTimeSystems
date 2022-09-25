#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <chrono>

const int maxSize = 1000000;
std::vector<std::string> students;

bool FindManually(std::string name)
{
    for (int i = 0; i < maxSize; i++)
        if (students[i] == name)
            return true;

    return false;
}

bool FindAutomatically(std::string name)
{
    return (std::find(students.begin(), students.end(), name) != students.end());
}

int main()
{
    // initialization
    for (int i = 0; i < maxSize; i++)
        students.push_back("Name " + std::to_string(i));

    std::string indexToFind = std::to_string(maxSize - 1);

    std::cout << "Start?";
    getchar();

    int noOfRepetitions = 100;
    for (int i = 0; i < noOfRepetitions; i++)
    {
        // start measurement
        auto start = std::chrono::high_resolution_clock::now();

        // call operation
        FindManually("Name " + indexToFind);

        // end measurement
        auto stop = std::chrono::high_resolution_clock::now();

        // calculate duration
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();

        std::cout << duration << std::endl;
    }
}