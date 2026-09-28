#include <algorithm>
#include <chrono>
#include <cstddef>
#include <iostream>
#include <numeric>
#include <random>
#include <string>
#include <vector>

#include "sorting.h"


// Benchmark configuration
constexpr int RUNS = 5;
constexpr unsigned int SEED = 42;

const std::vector<int> INPUT_SIZES = {
	1000,
	10000,
};

struct BenchmarkTarget {
	std::string name;
	sorting::SortFunction function;
};

struct BenchmarkResult {
	std::string algorithm;
	unsigned int seed;
	int inputSize;
	int run;
	double duration;
	bool success;
};


// utilities
std::vector<int> generateRandomVector(std::size_t inputSize, unsigned int seed) {
	std::mt19937 rng(seed);
	std::uniform_int_distribution<int> dist(0, 1000000);

	std::vector<int> v(inputSize);

	for (auto& value : v)
		value = dist(rng);

	return v;
}


int main() {
	const BenchmarkTarget target{
		.name = "Bubble Sort",
		.function = sorting::bubble
	};

	std::cout << "=== SortCritic Benchmark v0.1 ===\n";
	std::cout << "Algorithm: " << target.name << '\n';
	std::cout << "Seed: " << SEED << '\n';
	std::cout << "Runs: " << RUNS << "\n\n";

	std::vector<BenchmarkResult> results;
	results.reserve(INPUT_SIZES.size() * RUNS);

	for (int inputSize : INPUT_SIZES) {
		std::vector<int> original = generateRandomVector(inputSize, SEED);

		std::vector<int> expected = original;
		std::sort(expected.begin(), expected.end());

		std::vector<double> durations;
		durations.reserve(RUNS);

		std::cout << "Input size: " << inputSize << '\n';

		for (int run = 1; run <= RUNS; run++) {
			std::vector<int> sorted = original;

			auto start = std::chrono::steady_clock::now();
			target.function(sorted);
			auto end = std::chrono::steady_clock::now();

			double duration = 
				std::chrono::duration<double, std::milli>(end - start).count();
			durations.push_back(duration);

			bool success = (expected == sorted);

			BenchmarkResult result{
				.algorithm = target.name,
				.seed = SEED,
				.inputSize = inputSize,
				.run = run,
				.duration = duration,
				.success = success
			};
			results.push_back(result);

			std::cout << "Run " << run
					  << ": " << duration << " ms"
					  << " | "
					  << (success ? "PASS" : "FAIL")
					  << '\n';
		}

		double mean =
			std::accumulate(durations.begin(), durations.end(), 0.0)
			/ durations.size();
		
		auto [minIt, maxIt] =
			std::minmax_element(durations.begin(), durations.end());
		double minVal = *minIt;
		double maxVal = *maxIt;

		double median;
		std::vector<double> sortedDurations = durations;
		std::sort(sortedDurations.begin(), sortedDurations.end());
		std::size_t durationCount = sortedDurations.size();
		median = (durationCount % 2 == 1 ?
			      sortedDurations[durationCount / 2] :
			      (sortedDurations[durationCount / 2 - 1] + sortedDurations[durationCount / 2]) / 2.0
			);

		std::cout << "\nMean: " << mean << " ms\n";
		std::cout << "Median: " << median << " ms\n";
		std::cout << "Min: " << minVal << " ms\n";
		std::cout << "Max: " << maxVal << " ms\n";

		std::cout << '\n';
	}
}
