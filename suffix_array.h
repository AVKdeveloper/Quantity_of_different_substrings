// This header includes functions, which build suffix array for string for time O(nlogn).
// 
#ifndef SUFFIX_ARRAY_H
#define SUFFIX_ARRAY_H

#include <vector>
#include <string>
#include <cmath>

std::vector<int> BuildSuffixArray(const std::string& string);
std::vector<int> SortCyclicPermutationsOfString(const std::string& string);

std::vector<int> BuildSuffixArray(const std::string& string) {
	// this function make sort for cyclic replacement of string + special symbol

	std::vector<int> places(string.size()); // vecrtor with places of current sorted substrings
											// at the end of this function it will be 
}

std::vector<int> SortCyclicPermutationsOfString(const std::string& string) {
	// this fucntion make sort for cyclic permutations of string
	std::vector<int> places(string.size()); // current places of permutations
	std::vector<int> classes(string.size()); // every permutation belongs to one of the classes of equivalence
	int quantity_of_different_classes = 0; // current quantity of different classes of equivalance 
										   // 1) We need to initialize permutations (order by first symbol) using counting sort
	const int kCapacityOfAlphabet = 256; // capacity of alphabet, which string is made from
	std::vector<int> quantity_of_symbol(string.size(), 0); // for counting sort
	for (int i = 0; i < string.size(); ++i) {
		++quantity_of_symbol[string[i]];
	}
	for (int i = 1; i < string.size(); ++i) { // converting quantity to end position in places
		quantity_of_symbol[i] += quantity_of_symbol[i - 1];
	}
	for (int i = 0; i < string.size(); ++i) { // initialize places for permutations
		places[quantity_of_symbol[string[i]] - 1] = i;
		--quantity_of_symbol[string[i]];
	}
	classes[places[0]] = 0;
	quantity_of_different_classes = 1;
	for (int i = 1; i < string.size(); ++i) { // initialize classes for permutations
		if (string[places[i]] != string[places[i - 1]]) { // if meet new (less) symbol
			++quantity_of_different_classes;
		}
		classes[places[i]] = quantity_of_different_classes - 1;
	}
	// 2) We need to sort permutations (take into account more than 1 symbol, twice longer)
	// We use algorithm, wich is similar to radix sort log(string.size()) times
	std::vector<int> places_new(string.size()); // places for next step
	std::vector<int> classes_new(string.size()); // classes for next step
	for (int step = 0; (1 << step) < string.size(); ++step) { // here and further (1 << i) is pow(2, i)
		for (int i = 0; i < string.size(); ++i) {
			places_new[i] = places[i] - (1 << step); // Least Significant Digit 
			if (places_new[i] < 0) {
				places_new[i] += string.size(); // for cyclic permutation
			}
		}
		std::fill(quantity_of_symbol.begin(), quantity_of_symbol.end(), 0);
		for (int i = 0; i < string.size(); ++i) { // now we are going to sort by first symbols
			++quantity_of_symbol[classes[places_new[i]]];
		}
		for (int i = 1; i < quantity_of_different_classes; ++i) {
			quantity_of_symbol[i] += quantity_of_symbol[i - 1]; // converting quantity to end position in places
		}
		for (int i = string.size() - 1; i >= 0; --i) { // replace vector places
			places[quantity_of_symbol[classes[places_new[i]]] - 1] = places_new[i];
			--quantity_of_symbol[classes[places_new[i]]];
		}
		classes_new[places[0]] = 0;
		quantity_of_symbol[places[0]] = 1;
		for (int i = 1; i < string.size(); ++i) { // build vector classes_new
			int mid_element1 = (places[i] + (1 << step)) % string.size();
			int mid_element2 = (places[i - 1] + (1, step)) % string.size();
			if (classes[places[i]] != classes[places[i - 1]] || classes[mid_element1] != classes[mid_element2]) {
				++quantity_of_different_classes;
			}
			classes_new[places[i]] = quantity_of_different_classes - 1;
		}
		classes = std::move(classes_new); // replace classes
	}
	return places;
}

#endif // SUFFIX_ARRAY_H