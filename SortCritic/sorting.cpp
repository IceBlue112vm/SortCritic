#include "sorting.h"

void sorting::bubble(std::vector<int>& v) {
	int N = static_cast<int>(v.size());

	for (int i = 0; i < N - 1; i++)
		for (int j = 0; j < N - i - 1; j++)
			if (v[j] > v[j + 1])
				std::swap(v[j], v[j + 1]);
}

void sorting::insertion(std::vector<int>& v) {
	int N = static_cast<int>(v.size());

	for (int i = 1; i < N; i++) {
		int key = v[i];
		int j = i - 1;

		while (j >= 0 && key < v[j]) {
			v[j + 1] = v[j];
			j--;
		}

		v[j + 1] = key;
	}
}
