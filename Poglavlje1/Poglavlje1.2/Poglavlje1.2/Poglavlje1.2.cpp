#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

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
	for (int i = 0; i < maxSize; i++)
		students.push_back("Name " +
			std::to_string(i));
	std::string indexToFind = std::to_string
	(int(maxSize / 2));
	bool result = FindManually("Name " + indexToFind);
	std::cout << (result ? "Found" : "Not found");
	system("pause>0");
}