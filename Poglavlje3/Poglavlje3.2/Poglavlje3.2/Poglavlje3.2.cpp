#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono>
#include <matplotlibcpp.h>
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

std::vector<std::vector<double>> results;

void Measurement(std::string indexToFind, int threadNo)
{
	int noOfRepetitions = 100;
	for (int i = 0; i < noOfRepetitions; i++)
	{
		auto start = std::chrono::high_resolution_clock::now();
		FindManually("Name " + indexToFind);
		auto stop = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();
		results[threadNo].push_back(duration);
	}
}

namespace plt = matplotlibcpp;

int main()
{
	maxSize = 1000000;
	std::vector<int> x;
	for (int i = 0; i < maxSize; i++)
	{
		students.push_back("Name " + std::to_string(i));
		if (i < 100)
			x.push_back(i);
	}

	std::string indexToFind = std::to_string(maxSize - 1);

	int noOfThreads = 4;
	std::vector<std::thread> threads;
	for (int i = 0; i < noOfThreads; i++)
	{
		results.push_back(std::vector<double>());
		std::thread thread(Measurement, indexToFind, i);

		auto handle = thread.native_handle();
		int priority = -15;
		if (i > 1)
			priority = 15;
		SetThreadPriority(handle, priority);

		threads.push_back(std::move(thread));
	}

	for (int i = 0; i < noOfThreads; i++)
		threads[i].join();

	std::vector<std::string> colors { "cyan", "deepskyblue", "darkred", "red" };
	for (int i = 0; i < results.size(); i++)
		plt::plot(x, results[i], colors[i]);
	plt::xlabel("n (iteracija)");
	plt::ylabel("t (ms)");
	plt::ylim(0, 200);
	plt::show();
}