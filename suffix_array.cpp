#include "suffix_array.h"

StringHandler::StringHandler(const std::string& string) {
	string_ = string;
	suffix_array_ = BuildSuffixArray(string);
}

std::vector<int> StringHandler::BuildSuffixArray(const std::string& string) const {
	// ���� ����� ��������� �� ���� ������ � ������ ��� ��� ���������� ������
	// ������������ ���������� ������������ ������ ������ � �������� '\0' �� �����
	std::vector<int> suffix_array(string.size());
	std::vector<int> order_of_permutations = SortCyclicPermutationsOfString(string + '\0');
	return suffix_array;
}

std::vector<int> StringHandler::SortCyclicPermutationsOfString(const std::string& string) const {
	// ��� ������� ���������� ������ ��������������� ������������������ ������������ ������� ������
	// ���������� ������ �� ����������� ���������� 
	const size_t kCapacityOfAlphabet = 256; // �������� ��������, �� �������� ������� ������
	const size_t kMaxQuantityOfIndices = std::max(kCapacityOfAlphabet, string.size());
	std::vector<int> places(kMaxQuantityOfIndices); // ��������� �������� ���� ����������
	std::vector<int> classes(kMaxQuantityOfIndices); // ������ �� ������������ ������������� ���������� ������ ���������������
	int quantity_of_different_classes = 0; // ������� ���������� ������� ���������������
										   // 1) �������������, �.�. ���������� �� ������� ������� ������(���������� ���������)
	std::vector<int> quantity_of_symbol(kCapacityOfAlphabet, 0); // ���������� ������� �������
	for (int i = 0; i < string.size(); ++i) { // ������� ���-�� ��������� ������� �������
		++quantity_of_symbol[string[i]];
	}
	for (int i = 1; i < kCapacityOfAlphabet; ++i) {
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
	std::vector<int> places_new(kMaxQuantityOfIndices); // ������� ���������� ��� ���������� ����
	std::vector<int> classes_new(kMaxQuantityOfIndices); // ������ ��������������� ��� ���������� ����
	for (int step = 0; (1 << step) < string.size(); ++step) { // ��� ����������; ������, ���� ����� ���� < log2(n) 
		for (int i = 0; i < string.size(); ++i) {
			places_new[i] = places[i] - (1 << step); // LSD ����������
			if (places_new[i] < 0) { // ��������� ���������� �� ������ ��������
				places_new[i] += string.size(); // � ������, ���� ����� �� �������
			}
		}
		// ������ ������ �������� ��������, �� �� ���� ��� ������� ��������� ��������(�� ������ ������������ ����) 
		std::vector<int> count_different_substrings(quantity_of_different_classes, 0);
		// ������ ���������� �����������, �� ���������� �� ������ �������� ���� 
		for (int i = 0; i < string.size(); ++i) {
			++count_different_substrings[classes[places_new[i]]];
		}
		for (int i = 1; i < quantity_of_different_classes; ++i) {
			count_different_substrings[i] += count_different_substrings[i - 1];
		}
		for (int i = string.size() - 1; i >= 0; --i) { // ��������� ����� �����
			places[count_different_substrings[classes[places_new[i]]] - 1] = places_new[i];
			--count_different_substrings[classes[places_new[i]]];
		}
		classes_new[places[0]] = 0;
		quantity_of_different_classes = 1;
		for (int i = 1; i < string.size(); ++i) { // ��������� ����� ������ ���������������
			int mid_element1 = (places[i] + (1 << step)) % string.size();
			int mid_element2 = (places[i - 1] + (1, step)) % string.size();
			// ��������� ���� ������������ ��� �� ������ �������� ����, ��� � �� ������
			if (classes[places[i]] != classes[places[i - 1]] || classes[mid_element1] != classes[mid_element2]) {
				++quantity_of_different_classes;
			}
			classes_new[places[i]] = quantity_of_different_classes - 1;
		}
		std::copy(classes_new.begin(), classes_new.end(), classes.begin()); // ������� ������ classes � ������ ����
	}
	// ����������� �� ���������(������) ��������� �������, ������� ������������, ���� ����� �������� �������� ������ ������� ������ 
	std::vector<int> result(string.size()); // ��, ��� ��������� �������
	std::copy(places.begin(), places.begin() + string.size(), result.begin());
	return result;
}