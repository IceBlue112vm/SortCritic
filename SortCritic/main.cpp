#include <algorithm>
#include <chrono>
#include <cstddef>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <random>
#include <sstream>
#include <string>
#include <vector>

#include "sorting.h"


// Types
enum class InputDistribution {
	Random,
	Sorted,
	Reversed
};

struct BenchmarkTarget {
	std::string name;
	sorting::SortFunction function;
};

struct BenchmarkResult {
	std::string algorithm;
	InputDistribution distribution;
	unsigned int seed;
	std::size_t inputSize;
	int run;
	double duration;
	bool success;
};

struct BenchmarkStatistics {
	double mean;
	double median;
	double min;
	double max;
};


// Benchmark configuration
constexpr int RUNS = 5;
constexpr unsigned int SEED = 42;
constexpr InputDistribution DISTRIBUTION = InputDistribution::Random;

const std::vector<std::size_t> INPUT_SIZES = {
	1000,
	10000,
	25000,
	50000,
	75000,
	100000,
};


// Utilities
std::string distributionToString(InputDistribution distribution) {
	switch (distribution) {
	case InputDistribution::Random:
		return "Random";

	case InputDistribution::Sorted:
		return "Sorted";

	case InputDistribution::Reversed:
		return "Reversed";
	}

	return "Unknown";
}


std::vector<int> generateInputVector(
	std::size_t inputSize,
	unsigned int seed,
	InputDistribution distribution
) {
	std::mt19937 rng(seed);
	std::uniform_int_distribution<int> dist(0, 1000000);

	std::vector<int> values(inputSize);

	for (auto& value : values)
		value = dist(rng);

	switch (distribution) {
	case InputDistribution::Random:
		break;

	case InputDistribution::Sorted:
		std::sort(values.begin(), values.end());
		break;

	case InputDistribution::Reversed:
		std::sort(values.begin(), values.end());
		std::reverse(values.begin(), values.end());
		break;
	}

	return values;
}


BenchmarkStatistics calculateStatistics(
	const std::vector<double>& durations
) {
	double mean =
		std::accumulate(
			durations.begin(),
			durations.end(),
			0.0
		)
		/ durations.size();

	auto [minIt, maxIt] = std::minmax_element(durations.begin(), durations.end());

	std::vector<double> sortedDurations = durations;
	std::sort(sortedDurations.begin(), sortedDurations.end());

	std::size_t count = sortedDurations.size();

	double median =
		(count % 2 == 1)
		? sortedDurations[count / 2]
		: (
			sortedDurations[count / 2 - 1]
			+ sortedDurations[count / 2]
		  ) / 2.0;

	return BenchmarkStatistics{
		.mean = mean,
		.median = median,
		.min = *minIt,
		.max = *maxIt
	};
}


std::string createTimestampString() {
	auto now = std::chrono::system_clock::now();

	std::time_t nowTime = std::chrono::system_clock::to_time_t(now);

	std::tm localTime{};
	localtime_s(&localTime, &nowTime);

	std::ostringstream oss;

	oss << std::put_time(&localTime, "%Y%m%d_%H%M%S");

	return oss.str();
}


std::filesystem::path createResultFilePath() {
	const std::filesystem::path resultDirectory = "results";

	std::filesystem::create_directories(resultDirectory);

	return resultDirectory /
		("benchmark_" + createTimestampString() + ".csv");
}


void saveResultsToCsv(
	const std::vector<BenchmarkResult>& results,
	const std::filesystem::path& filePath
) {
	std::ofstream file(filePath);

	if (!file.is_open()) {
		std::cerr
			<< "Failed to open file: "
			<< filePath
			<< '\n';

		return;
	}

	file
		<< "algorithm,"
		<< "distribution,"
		<< "seed,"
		<< "inputSize,"
		<< "run,"
		<< "durationMs,"
		<< "success\n";

	file << std::setprecision(10);

	for (const auto& result : results) {
		file
			<< result.algorithm << ','
			<< distributionToString (result.distribution) << ','
			<< result.seed << ','
			<< result.inputSize << ','
			<< result.run << ','
			<< result.duration << ','
			<< (result.success ? "true" : "false")
			<< '\n';
	}

	std::cout
		<< "Results saved: "
		<< filePath
		<< '\n';
}


// Main
int main() {
	const BenchmarkTarget target{
		.name = "Quick Sort",
		.function = sorting::quick
	};

	std::cout << "=== SortCritic Benchmark v0.1 ===\n";
	std::cout << "Algorithm: " << target.name << '\n';
	std::cout
		<< "Distribution: "
		<< distributionToString(DISTRIBUTION)
		<< '\n';
	std::cout << "Seed: " << SEED << '\n';
	std::cout << "Runs: " << RUNS << "\n\n";

	std::vector<BenchmarkResult> results;

	results.reserve(
		INPUT_SIZES.size() * RUNS
	);


	for (std::size_t inputSize : INPUT_SIZES) {
		std::vector<int> original =
			generateInputVector(
				inputSize,
				SEED,
				DISTRIBUTION
			);

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

			double duration
				= std::chrono::duration<double, std::milli>(end - start).count();

			bool success = (expected == sorted);

			durations.push_back(duration);

			BenchmarkResult result{
				.algorithm = target.name,
				.distribution = DISTRIBUTION,
				.seed = SEED,
				.inputSize = inputSize,
				.run = run,
				.duration = duration,
				.success = success
			};

			results.push_back(result);

			std::cout << "Run " << run << ": "
				<< duration << " ms" << " | "
				<< (success ? "PASS" : "FAIL")
				<< '\n';
		}

		BenchmarkStatistics statistics = calculateStatistics(durations);

		std::cout << "\nMean: " << statistics.mean << " ms\n";
		std::cout << "Median: " << statistics.median << " ms\n";
		std::cout << "Min: " << statistics.min << " ms\n";
		std::cout << "Max: " << statistics.max << " ms\n\n";
	}


	std::filesystem::path resultFilePath =
		createResultFilePath();

	saveResultsToCsv(results, resultFilePath);


	return 0;
}