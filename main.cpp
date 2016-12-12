// Ёта программа сначала строит суффисный массив дл€ заданнаго слова, 
// затем использу€ алгоритм  асаи ищет количество различных подстрок дл€ двнного слова.
// —уффиксный массив строим за O(nlogn), сортиру€ циклические перестановки слова, состо€щего
// из заданного слова и нового уникального символа, который меньше всех других символов('\0').

#include <fstream>
#include <string>
#include <vector>
#include "suffix_array.h"

int main() {
	std::fstream file;
	file.open("input.txt", std::fstream::in);
	std::string string;
	file >> string;
	file.close();
	StringHandler string_handler(string);
	int quantity_of_different_substrings = string_handler.QuantityOfDifferentSubstrings();
	file.open("output.txt", std::fstream::out);
	file << quantity_of_different_substrings;
	file.close();
	return 0;
}