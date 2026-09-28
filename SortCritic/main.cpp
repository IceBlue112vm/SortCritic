#include <algorithm>
#include <chrono>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>

#include "sorting.h"

using namespace std;


// Benchmark configuration
constexpr int RUNS = 5;
constexpr unsigned int SEED = 42;

const vector<int> SIZES = {
	1000,
	10000,
};


vector<int> generateRandomVector(size_t N, unsigned int seed) {
	mt19937 rng(seed);
	uniform_int_distribution<int> dist(0, 1000000);

	vector<int> v(N);

	for (auto& value : v)
		value = dist(rng);

	return v;
}

void printVector(const vector<int>& v) {
	for (size_t i = 0; i < v.size(); i++)
		cout << v[i] << ' ';
	cout << endl;
}

int main() {
	cout << "=== SortCritic Benchmark v0.1 ===\n";
	cout << "Algorithm: Bubble Sort\n";
	cout << "Seed: " << SEED << '\n';
	cout << "Runs: " << RUNS << "\n\n";

	for (int N : SIZES) {
		vector<int> original = generateRandomVector(N, SEED);

		vector<int> expected = original;
		sort(expected.begin(), expected.end());

		vector<double> durations;
		durations.reserve(RUNS);

		cout << "N: " << N << '\n';

		for (int run = 0; run < RUNS; run++) {
			vector<int> sorted = original;

			auto start = chrono::steady_clock::now();
			sorting::bubble(sorted);
			auto end = chrono::steady_clock::now();

			auto duration =
				chrono::duration<double, milli>(end - start).count();
			durations.push_back(duration);

			bool success = (expected == sorted);

			cout << "Run " << run + 1
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
		vector<double> sortedDurations = durations;
		sort(sortedDurations.begin(), sortedDurations.end());
		size_t n_durations = sortedDurations.size();
		median = (n_durations % 2 == 1 ?
			      sortedDurations[n_durations / 2] :
			      (sortedDurations[n_durations / 2 - 1] + sortedDurations[n_durations / 2]) / 2.0
			);

		cout << "\nMean: " << mean << " ms\n";
		cout << "Median: " << median << " ms\n";
		cout << "Min: " << minVal << " ms\n";
		cout << "Max: " << maxVal << " ms\n";

		cout << '\n';
	}
}
