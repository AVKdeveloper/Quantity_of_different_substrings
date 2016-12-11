// This header includes functions, which build suffix array for string for time O(nlogn).
// 
#ifndef SUFFIX_ARRAY_H
#define SUFFIX_ARRAY_H

#include <vector>
#include <string>
#include <cmath>
#include <algorithm>

std::vector<int> BuildSuffixArray(const std::string& string);
std::vector<int> SortCyclicPermutationsOfString(const std::string& string);

/*
std::vector<int> BuildSuffixArray(const std::string& string) {
    // 
	std::vector<int> places(string.size()); // vecrtor with places of current sorted substrings
											// at the end of this function it will be 
	return;
} */

std::vector<int> SortCyclicPermutationsOfString(const std::string& string) {
	// ��� ������� ���������� ������ ��������������� ������������������ ������������ ������� ������
	// ���������� ������ �� ����������� ���������� 
	const size_t kCapacityOfAlphabet = 256; // �������� ��������, �� �������� ������� ������
	std::vector<int> places(string.size()); // ��������� �������� ���� ����������
	std::vector<int> classes(string.size()); // ������ �� ������������ ������������� ���������� ������ ���������������
	int quantity_of_different_classes = 0; // ������� ���������� ������� ���������������
	// 1) �������������, �.�. ���������� �� ������� ������� ������(���������� ���������)
	std::vector<int> quantity_of_symbol(std::max(kCapacityOfAlphabet, string.size()), 0); // ���������� ������� �������
	for (int i = 0; i < string.size(); ++i) { // ������� ���-�� ��������� ������� �������
		++quantity_of_symbol[string[i]];
	}
	for (int i = 1; i < string.size(); ++i) { 
		// ��������� ���������� �������� � ������� ����� �� ������������� � ������� ���� ����������  
		quantity_of_symbol[i] += quantity_of_symbol[i - 1];
	}
	for (int i = 0; i < string.size(); ++i) { // ��������� ������ places � ������ ���������� ��������
		places[quantity_of_symbol[string[i]] - 1] = i;
		--quantity_of_symbol[string[i]];
	}
	classes[places[0]] = 0;
	quantity_of_different_classes = 1;
	for (int i = 1; i < string.size(); ++i) { // ����������� ������ ���������������
		if (string[places[i]] != string[places[i - 1]]) { // ���� ��������� ��������� ����� ������
			++quantity_of_different_classes; // �� �� ����������� ������ ������ ���������������
		}
		classes[places[i]] = quantity_of_different_classes - 1;
	}
	// 2) ���� ���������� ����� ����� k, ��������� ������ ����� 2k
	std::vector<int> places_new(string.size()); // ������� ���������� ��� ���������� ����
	std::vector<int> classes_new(string.size()); // ������ ��������������� ��� ���������� ����
	for (int step = 0; (1 << step) < string.size(); ++step) { // ��� ����������; ������, ���� ����� ���� < log2(n) 
		for (int i = 0; i < string.size(); ++i) {
			places_new[i] = places[i] - (1 << step); // LSD ����������
			if (places_new[i] < 0) { // ��������� ���������� �� ������ ��������
				places_new[i] += string.size(); // � ������, ���� ����� �� �������
			}
		}
		std::fill(quantity_of_symbol.begin(), quantity_of_symbol.end(), 0);
		// ������ ���������� �����������, �� ���������� �� ������ �������� ���� 
		for (int i = 0; i < string.size(); ++i) { 
			++quantity_of_symbol[classes[places_new[i]]];
		}
		for (int i = 1; i < quantity_of_different_classes; ++i) {
			quantity_of_symbol[i] += quantity_of_symbol[i - 1];
		}
		for (int i = string.size() - 1; i >= 0; --i) { // ��������� ����� �����
			places[quantity_of_symbol[classes[places_new[i]]] - 1] = places_new[i];
			--quantity_of_symbol[classes[places_new[i]]];
		}
		classes_new[places[0]] = 0;
		quantity_of_symbol[places[0]] = 1;
		for (int i = 1; i < string.size(); ++i) { // ��������� ����� ������ ���������������
			int mid_element1 = (places[i] + (1 << step)) % string.size();
			int mid_element2 = (places[i - 1] + (1, step)) % string.size();
			// ��������� ���� ������������ ��� �� ������ �������� ����, ��� � �� ������
			if (classes[places[i]] != classes[places[i - 1]] || classes[mid_element1] != classes[mid_element2]) {
				++quantity_of_different_classes;
			}
			classes_new[places[i]] = quantity_of_different_classes - 1;
		}
		classes = std::move(classes_new); // ������� ������ classes � ������ ����
	}
	return places;
}

#endif // SUFFIX_ARRAY_H