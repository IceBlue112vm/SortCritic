#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>
#include <vector>

#include "sorting.h"

using namespace std;

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
	for (int j = 0; j < 3; j++) {
		for (int i = 10000; i <= 10000; i += 10000) {
			int N = i;
			unsigned int seed = 42;

			vector<int> original = generateRandomVector(N, seed);
			vector<int> sorted = original;

			auto start = chrono::steady_clock::now();
			sorting::bubble(sorted);
			auto end = chrono::steady_clock::now();

			auto duration = chrono::duration<double, milli>(end - start).count();

			cout << "N: " << N << endl;
			cout << "Seed: " << seed << endl;
			cout << "Duration: " << duration << " ms" << endl;
			cout << "Is sorted: "
				<< boolalpha
				<< verifySorted(original, sorted)
				<< endl;
		}
	}
}