#pragma once

#include <vector>

namespace sorting {

	using SortFunction = void(*)(std::vector<int>&);

	void bubble(std::vector<int>& v);
	void insertion(std::vector<int>& v);
}
