// This header includes functions, which build suffix array for string for time O(nlogn).
// 
#ifndef SUFFIX_ARRAY_H
#define SUFFIX_ARRAY_H

#include <vector>
#include <string>
#include <cmath>
#include <algorithm>

class StringHandler {
	std::string string_;
	std::vector<int> suffix_array_;
public:
	StringHandler(const std::string& string);
	std::vector<int> BuildSuffixArray(const std::string& string) const;
	std::vector<int> SortCyclicPermutationsOfString(const std::string& string) const;

};



#endif // SUFFIX_ARRAY_H