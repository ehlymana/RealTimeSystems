#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono>

int maxSize = 1000000;
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
    return (std::find(students.begin(), students.end(),
        name) != students.end());
}

int main()
{
    for (int i = 0; i < maxSize; i++)
            students.push_back("Name " +
                std::to_string(i));
        std::string indexToFind = std::to_string
        (int(maxSize / 2));

    for (int j = 0; j < 2; j++)
    {
        std::cout << "Function: " << (j == 0 ? "FindManually" : "FindAutomatically") << std::endl;

        double result = 0;
        for (int noOfRepetitions = 10; noOfRepetitions < 10000; noOfRepetitions *= 10)
        {
            for (int i = 0; i < noOfRepetitions; i++)
            {
                auto start = std::chrono::high_resolution_clock::now();
                j == 0 ? FindManually("Name " + indexToFind) : FindAutomatically("Name " + indexToFind);
                auto stop = std::chrono::high_resolution_clock::now();

                auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();
                result += duration;
            }

            result /= noOfRepetitions;
            std::cout << "Number of repetitions: " << noOfRepetitions << " Average duration: " << result << " ms" << std::endl;
        }
    }

    system("pause>0");
}