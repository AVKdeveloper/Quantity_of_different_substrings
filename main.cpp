// ��� ��������� ������� ������ ��������� ������ ��� ��������� �����, 
// ����� ��������� �������� ����� ���� ���������� ��������� �������� ��� ������� �����.
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
	StringHandler string_handler(string + '$');

	return 0;
}