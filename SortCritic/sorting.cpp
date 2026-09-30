#include "sorting.h"

// Impls
namespace {

void mergeRange(
	std::vector<int>& v,
	std::vector<int>& temp,
	std::size_t left,
	std::size_t mid,
	std::size_t right
) {
	std::size_t i = left;
	std::size_t j = mid;
	std::size_t k = left;

	while (i < mid && j < right) {
		if (v[i] <= v[j])
			temp[k++] = v[i++];
		else
			temp[k++] = v[j++];
	}

	while (i < mid)
		temp[k++] = v[i++];

	while (j < right)
		temp[k++] = v[j++];

	for (std::size_t index = left; index < right; index++)
		v[index] = temp[index];
}


void mergeSortImpl(
	std::vector<int>& v,
	std::vector<int>& temp,
	std::size_t left,
	std::size_t right
) {
	if (right - left <= 1)
		return;

	std::size_t mid = left + (right - left) / 2;

	mergeSortImpl(v, temp, left, mid);
	mergeSortImpl(v, temp, mid, right);

	mergeRange(v, temp, left, mid, right);
}


void quickSortImpl(std::vector<int>& v, int left, int right) {
	if (left >= right)
		return;

	int i = left;
	int j = right;
	int pivot = v[left + (right - left) / 2];

	while (i <= j) {
		while (v[i] < pivot)
			i++;

		while (v[j] > pivot)
			j--;

		if (i <= j) {
			std::swap(v[i], v[j]);
			i++;
			j--;
		}
	}

	if (left < j)
		quickSortImpl(v, left, j);

	if (i < right)
		quickSortImpl(v, i, right);
}

}


// algorithms
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

void sorting::selection(std::vector<int>& v) {
	int N = static_cast<int>(v.size());

	for (int i = 0; i < N - 1; i++) {
		int minIndex = i;

		for (int j = i + 1; j < N; j++)
			if (v[j] < v[minIndex])
				minIndex = j;

		std::swap(v[i], v[minIndex]);
	}
}

void sorting::merge(std::vector<int>& v) {
	std::vector<int> temp(v.size());

	mergeSortImpl(v, temp, 0, v.size());
}

void sorting::quick(std::vector<int>& v) {
	if (v.empty())
		return;

	quickSortImpl(
		v,
		0,
		static_cast<int>(v.size()) - 1
	);
}
