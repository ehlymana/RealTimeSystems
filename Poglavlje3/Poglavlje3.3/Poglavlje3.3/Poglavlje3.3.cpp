#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono>
#include <thread>
#include <windows.h>
#include <processthreadsapi.h>

int maxSize = 10;
std::vector<std::string> students;

bool FindManually(std::string name)
{
	for (int i = 0; i < maxSize; i++)
		if (students[i] == name)
			return true;
	return false;
}

void Measurement(std::string indexToFind, int threadNo)
{
	std::cout << "THREAD NO. " << threadNo << " MEASUREMENT BEGINS" << std::endl;

	int noOfRepetitions = 100;
	for (int i = 0; i < noOfRepetitions; i++)
	{
		auto start = std::chrono::high_resolution_clock::now();
		FindManually("Name " + indexToFind);
		auto stop = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();
		std::cout << "Thread no. " << threadNo << ", Iteration " << i + 1 << ": " << duration << std::endl;
	}
	std::cout << "THREAD NO. " << threadNo << " MEASUREMENT ENDS" << std::endl;
}

void TimerTrigger(std::string indexToFind, int threadNo)
{
	std::thread thread(Measurement, indexToFind, threadNo);
	thread.detach();
}

int main()
{
	maxSize = 1000000;
	for (int i = 0; i < maxSize; i++)
		students.push_back("Name " + std::to_string(i));

	std::string indexToFind = std::to_string(maxSize - 1);

	double delay = 1000;
	delay *= CLOCKS_PER_SEC / 1000;
	int i = 0;
	while (1 == 1)
	{
		TimerTrigger(indexToFind, i);
		i++;

		clock_t now = clock();
		while (clock() - now < delay);
	}
}