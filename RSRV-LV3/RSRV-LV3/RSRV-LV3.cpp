#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <chrono>
#include <thread>
#include <windows.h>
#include <processthreadsapi.h>

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

void Measurement(std::string indexToFind)
{
    std::cout << "THREAD MEASUREMENT BEGINS" << std::endl;

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

        std::cout << "Iteration " << i + 1 << ": " << duration << std::endl;
    }

    std::cout << "THREAD MEASUREMENT ENDS" << std::endl;
}

void TimerTrigger(std::string indexToFind)
{
    // start measurement thread
    std::thread thread(Measurement, indexToFind);

    // run thread independently
    thread.detach();
}

int main()
{
    // initialization
    for (int i = 0; i < maxSize; i++)
        students.push_back("Name " + std::to_string(i));

    std::string indexToFind = std::to_string(maxSize - 1);

    double delay = 100;
    delay *= CLOCKS_PER_SEC / 1000;

    while (1 == 1)
    {
        // trigger event
        TimerTrigger(indexToFind);

        // wait specified amount of time
        clock_t now = clock();
        while (clock() - now < delay);
    }
}