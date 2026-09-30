#pragma once

#include <vector>

namespace sorting {

	using SortFunction = void(*)(std::vector<int>&);

	// Core
	void bubble(std::vector<int>& v);
	void insertion(std::vector<int>& v);
	void selection(std::vector<int>& v);
	void merge(std::vector<int>& v);
}
