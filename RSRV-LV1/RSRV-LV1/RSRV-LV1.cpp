#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <chrono>

const int maxSize = 100000;
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

    std::string indexToFind = std::to_string(int(maxSize / 2));

    int noOfRepetitions = 10;
    while (noOfRepetitions < 100000)
    {
        std::cout << "No. of repetitions: " << noOfRepetitions << std::endl;
        for (int j = 0; j < 2; j++)
        {
            std::cout << "Function: ";
            if (j == 0)
                std::cout << "FindManually";
            else
                std::cout << "FindAutomatically";
            std::cout << std::endl;

            double result = 0;

            for (int i = 0; i < noOfRepetitions; i++)
            {
                // start measurement
                auto start = std::chrono::high_resolution_clock::now();

                // call operation
                if (j == 0)
                    FindManually("Name " + indexToFind);
                else
                    FindAutomatically("Name " + indexToFind);

                // end measurement
                auto stop = std::chrono::high_resolution_clock::now();

                // calculate duration
                auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();

                result += duration;
            }

            // write results
            result /= noOfRepetitions;
            std::cout << "Average duration of search: " << result << " ms" << std::endl;
        }
        noOfRepetitions *= 10;
    }
}