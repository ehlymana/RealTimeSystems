#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono>

int maxSize = 10;
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
    maxSize = 100000;
    for (int i = 0; i < maxSize; i++)
        students.push_back("Name " + std::to_string(i));

    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 100000; i++)
        FindManually("Name 0");
    auto stop = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();
    std::cout << "FindManually, Duration : " << duration << " ms" << std::endl;

    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 100000; i++)
        FindAutomatically("Name 0");
    stop = std::chrono::high_resolution_clock::now();

    duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();
    std::cout << "FindAutomatically, Duration: " << duration << " ms" << std::endl;

    system("pause>0");
}