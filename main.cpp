#include <fstream>
#include <string>
#include "suffix_array.h"

int main() {
	std::fstream file;
	file.open("input.txt", std::fstream::in);
	std::string string;
	file >> string;
	file.close();
	file.open("output.txt", std::fstream::out);

	return 0;
}