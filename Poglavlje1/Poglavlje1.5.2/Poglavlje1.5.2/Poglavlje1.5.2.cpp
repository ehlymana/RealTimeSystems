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
    maxSize = 1000000;
    for (int i = 0; i < maxSize; i++)
        students.push_back("Name " + std::to_string(i));
    std::string indexToFind = std::to_string(int(maxSize - 1));

    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 1000; i++)
        FindManually("Name " + indexToFind);
    auto stop = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::seconds>(stop - start).count();
    std::cout << "FindManually, Size: " << maxSize << ", Duration : " << duration << " s" << std::endl;

    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 1000; i++)
        FindAutomatically("Name " + indexToFind);
    stop = std::chrono::high_resolution_clock::now();

    duration = std::chrono::duration_cast<std::chrono::seconds>(stop - start).count();
    std::cout << "FindAutomatically, Size: " << maxSize << ", Duration: " << duration << " s" << std::endl;

    system("pause>0");
}