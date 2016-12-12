#include "suffix_array.h"

StringHandler::StringHandler(const std::string& string) {
	string_ = string;
	suffix_array_ = BuildSuffixArray(string);
	positions_ = BuildInverseArrayForSuffix(suffix_array_);
}

std::vector<int> StringHandler::BuildSuffixArray(const std::string& string) const {
	// Этот метод принимает на вход строку и выдает для нее суффиксный массив
	// Производится сортировка перестановок данной строки с символом '\0' на конце
	std::vector<int> suffix_array(string.size());
	std::vector<int> order_of_permutations = SortCyclicPermutationsOfString(string + '\0');
	// Первый элемент массива order_of_permutations не несет никакой информации, т.к. соответствует символу '\0'
	std::copy(order_of_permutations.begin() + 1, order_of_permutations.end(), suffix_array.begin());
	return suffix_array;
}

std::vector<int> StringHandler::SortCyclicPermutationsOfString(const std::string& string) const {
	// Эта функция возвращает вектор отсортированной последовательности перестановок входной строки
	// Сортировка похожа на поразрядную сортировку 
	const size_t kCapacityOfAlphabet = 256; // Мощность алфавита, из которого сделана строка
	const size_t kMaxQuantityOfIndices = std::max(kCapacityOfAlphabet, string.size());
	std::vector<int> places(kMaxQuantityOfIndices); // результат текущего шага сортировки
	std::vector<int> classes(kMaxQuantityOfIndices); // каждая из перестановок соответствует некоторуму классу эквивалентности
	int quantity_of_different_classes = 0; // текущее количество классов эквивалентности
										   // 1) Инициализация, т.е. сортировка по первому символу строки(сортировка подсчетом)
	std::vector<int> quantity_of_symbol(kCapacityOfAlphabet, 0); // количество каждого символа
	for (int i = 0; i < string.size(); ++i) { // считаем кол-во вхождений каждого символа
		++quantity_of_symbol[string[i]];
	}
	for (int i = 1; i < kCapacityOfAlphabet; ++i) {
		// переводим количество символов в позицию конца их встречаемости в текущем шаге сортировки  
		quantity_of_symbol[i] += quantity_of_symbol[i - 1];
	}
	for (int i = 0; i < string.size(); ++i) { // заполняем массив places с учетом результата подсчета
		places[quantity_of_symbol[string[i]] - 1] = i;
		--quantity_of_symbol[string[i]];
	}
	classes[places[0]] = 0;
	quantity_of_different_classes = 1;
	for (int i = 1; i < string.size(); ++i) { // присваиваем классы эквивалентности
		if (string[places[i]] != string[places[i - 1]]) { // если встретили следующий новый символ
			++quantity_of_different_classes; // то он принадлежит новому классу эквивалентности
		}
		classes[places[i]] = quantity_of_different_classes - 1;
	}
	// 2) Зная сортировку строк длины k, сортируем строки длины 2k
	std::vector<int> places_new(kMaxQuantityOfIndices); // позиции сортировки для следующего шага
	std::vector<int> classes_new(kMaxQuantityOfIndices); // классы эквивалентности для следующего шага
	for (int step = 0; (1 << step) < string.size(); ++step) { // шаг сортировки; шагаем, пока номер шага < log2(n) 
		for (int i = 0; i < string.size(); ++i) {
			places_new[i] = places[i] - (1 << step); // LSD сортировка
			if (places_new[i] < 0) { // учитываем сортировку по второй половине
				places_new[i] += string.size(); // в случае, если вышли за пределы
			}
		}
		// вводим аналог подсчета символов, но на этот раз подсчет различных подстрок(их размер соответсвует шагу) 
		std::vector<int> count_different_substrings(quantity_of_different_classes, 0);
		// теперь аналогично предыдущему, но сортировка по первой половине слов 
		for (int i = 0; i < string.size(); ++i) {
			++count_different_substrings[classes[places_new[i]]];
		}
		for (int i = 1; i < quantity_of_different_classes; ++i) {
			count_different_substrings[i] += count_different_substrings[i - 1];
		}
		for (int i = string.size() - 1; i >= 0; --i) { // назначаем новые места
			places[count_different_substrings[classes[places_new[i]]] - 1] = places_new[i];
			--count_different_substrings[classes[places_new[i]]];
		}
		classes_new[places[0]] = 0;
		quantity_of_different_classes = 1;
		for (int i = 1; i < string.size(); ++i) { // назначаем новые классы эквивалентности
			int mid_element1 = (places[i] + (1 << step)) % string.size();
			int mid_element2 = (places[i - 1] + (1, step)) % string.size();
			// сравнение слов производится как по порвой половине слов, так и по второй
			if (classes[places[i]] != classes[places[i - 1]] || classes[mid_element1] != classes[mid_element2]) {
				++quantity_of_different_classes;
			}
			classes_new[places[i]] = quantity_of_different_classes - 1;
		}
		std::copy(classes_new.begin(), classes_new.end(), classes.begin()); // готовим вектор classes к новому шагу
	}
	// Избавляемся от последних(пустых) элементов вектора, которые образовались, если вдруг мощность алфавита больше размера строки 
	std::vector<int> result(string.size()); // то, что возвратит функция
	std::copy(places.begin(), places.begin() + string.size(), result.begin());
	return result;
}

std::vector<int> StringHandler::BuildInverseArrayForSuffix(const std::vector<int> suffix_array) const {
	// Строим массив, обратный к суффиксному, т.е. отвечающий на вопрос:
	// "Какую префикс-функцию имеет k-ый суффикс слова?" = "На какой позиции стоит k-ый суффикс массива?"
	std::vector<int> positions(suffix_array.size());
	for (int i = 0; i < suffix_array.size(); ++i) {
		positions[suffix_array[i]] = i;
	}
	return positions;
}

std::vector<int> StringHandler::BuildLongestCommonPrefixArray() const {
	// Для эффективного построения массива с длинами наибольших общих префиксов используем алгоритм Касаи (O(n))
	// Для каждого индекса i массив LCP хранит размер общего префикса i-ого суффикса с i+1-ым суффиксом
	std::vector<int> longest_common_prefix(string_.size()); // здесь будет результат
	int current_quantity_of_symbols = 0; // текущее значение количества общих символов
	for (int i = 0; i < string_.size(); ++i) { 
		if (current_quantity_of_symbols > 0) { // в случае k > 0 уменьшаем на единицу
			--current_quantity_of_symbols;
		}
		if (positions_[i] == string_.size() - 1) { // если позиция i-ого суффикса последняя
			// То у него нет следующего за ним
			longest_common_prefix[string_.size() - 1] = 0; // ноль для удобной обработки кол-ва различных подстрок
			current_quantity_of_symbols = 0;
		} else { // иначе рассмотрим суффикс, следующий по позиции
			int j = suffix_array_[positions_[i] + 1];
			while (std::max(i + current_quantity_of_symbols, j + current_quantity_of_symbols) < string_.size() &&
				   string_[i + current_quantity_of_symbols] == string_[j + current_quantity_of_symbols]) {
				++current_quantity_of_symbols;
			}
			longest_common_prefix[positions_[i]] = current_quantity_of_symbols;
		}
	}
	return longest_common_prefix;
}

int StringHandler::QuantityOfDifferentSubstrings() const {
	// Для каждого нового суффикса новые подстроки состоят из всех его префиксов, кроме совпадающих с префиксами предыдущего суффикса
	std::vector<int> longest_common_prefix = BuildLongestCommonPrefixArray();
	int quantity_of_different_substrings = 0;
	for (int i = 0; i < string_.size(); ++i) {
		quantity_of_different_substrings += string_.size() - suffix_array_[i] - longest_common_prefix[i];
	}
	return quantity_of_different_substrings;
}