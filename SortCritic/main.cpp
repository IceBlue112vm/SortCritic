#include <algorithm>
#include <chrono>
#include <iostream>
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

bool verifySorted(const vector<int>& original, const vector<int>& sorted) {
	vector<int> expected = original;
	sort(expected.begin(), expected.end());

	return expected == sorted;
}

int main() {
	cout << "=== SortCritic Benchmark v0.1 ===\n";
	cout << "Algorithm: Bubble Sort\n";
	cout << "Seed: " << SEED << '\n';
	cout << "Runs: " << RUNS << "\n\n";

	for (int N : SIZES) {
		vector<int> original = generateRandomVector(N, SEED);

		cout << "N: " << N << '\n';

		for (int run = 0; run < RUNS; run++) {
			vector<int> sorted = original;

			auto start = chrono::steady_clock::now();
			sorting::bubble(sorted);
			auto end = chrono::steady_clock::now();

			auto duration =
				chrono::duration<double, milli>(end - start).count();

			cout << "Run " << run + 1
				<< ": " << duration << " ms"
				<< " | "
				<< (verifySorted(original, sorted) ? "PASS" : "FAIL")
				<< '\n';
		}

		cout << '\n';
	}
}
