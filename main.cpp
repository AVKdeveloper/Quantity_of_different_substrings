// ��� ��������� ������� ������ ��������� ������ ��� ��������� �����, �����
// ���������� ������ ������ �� O(nlogn), �������� ����������� ������������ �����, ���������� 
// �� ��������� ����� � ������ ����������� �������, ������� ������ ���� ������ ��������('\0').

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
	file.open("output.txt", std::fstream::out);
	std::vector<int> result_of_sorting = SortCyclicPermutationsOfString(string);

	return 0;
}