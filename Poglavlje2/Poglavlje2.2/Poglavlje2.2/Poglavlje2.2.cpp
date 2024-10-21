#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono>
#include <matplotlibcpp.h>

int maxSize = 10;
std::vector<std::string> students;

bool FindManually(std::string name)
{
    for (int i = 0; i < maxSize; i++)
        if (students[i] == name)
            return true;
    return false;
}

namespace plt = matplotlibcpp;

int main()
{
    maxSize = 1000000;
    int noOfRepetitions = 100;
    for (int i = 0; i < maxSize; i++)
        students.push_back("Name " + std::to_string(i));
    std::string indexToFind = std::to_string(int(maxSize - 1));
    std::vector<int> measurements, x;

    std::cout << "Start?";
    getchar();

    for (int i = 0; i < noOfRepetitions; i++)
    {
        x.push_back(i);
        auto start = std::chrono::high_resolution_clock::now();
        FindManually("Name " + indexToFind);
        auto stop = std::chrono::high_resolution_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();
        measurements.push_back(duration);
    }
    plt::plot(x, measurements, "b");
    plt::xlabel("n (iteracija)");
    plt::ylabel("t (ms)");
    plt::ylim(0, 200);
    plt::show();
}