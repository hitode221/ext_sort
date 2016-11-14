#include <fstream>
#include <vector>
#include <string>
#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <iostream>
#include <sstream>

struct man {
	std::string last_name, first_name; 
	size_t year;
};

bool Sort(man a, man b) {
	return (a.last_name < b.last_name);
}

std::string generate_name(size_t i) {
	std::stringstream ss;
	ss << i << ".txt";
	std::string result = ss.str();
	return result;
}

void merge_files(std::string first_name, std::string second_name, std::string result_file) {
	std::fstream first(first_name), second(second_name);
	std::ofstream result(result_file);
	man temp1, temp2;
	int a;
	if(!first.eof()) first >> temp1.last_name >> temp1.first_name >> temp1.year;
	if (!second.eof()) second >> temp2.last_name >> temp2.first_name >> temp2.year;
	while (!first.eof() && !second.eof()) {
		if (Sort(temp1, temp2)) {
			result << temp1.last_name << " " << temp1.first_name << " " << temp1.year << std::endl;
			first >> temp1.last_name >> temp1.first_name >> temp1.year;
		}
		else {
			result << temp2.last_name << " " << temp2.first_name << " " << temp2.year << std::endl;
			second >> temp2.last_name >> temp2.first_name >> temp2.year;
		}
	}
	while(!first.eof()) {
		result << temp1.last_name << " " << temp1.first_name << " " << temp1.year << std::endl;
		first >> temp1.last_name >> temp1.first_name >> temp1.year;
	}
	while(!second.eof()) {
		result << temp2.last_name << " " << temp2.first_name << " " << temp2.year << std::endl;
		second >> temp2.last_name >> temp2.first_name >> temp2.year;
	}
	second.close();
	first.close();
	remove(first_name.c_str());
	remove(second_name.c_str());
}

void ext_sort(std::string file_name, size_t size_of_block) {
	std::fstream fin(file_name);
	if (!fin.is_open()) return;
	size_t i = 0;
	std::vector<man> people;
	man temp;
	int j = 0;
	while (!fin.eof()) {
		std::cout << generate_name(i) << std::endl;
		std::ofstream fout(generate_name(i));
		while ((sizeof(std::vector<man>) + (sizeof(man) * people.size())) < size_of_block) {
			if (fin.eof()) break;
			fin >> temp.last_name >> temp.first_name >> temp.year;
			people.insert(people.end(), temp);
			std::cout << j++ << " ";
		}
		sort(people.begin(), people.end(), Sort);
		for (size_t j = 0; j < people.size(); j++)
			fout << people[j].last_name << " " << people[j].first_name << " " << people[j].year << std::endl;
		i++;
		std::cout << std::endl << sizeof(std::vector<man>) + (sizeof(man) * people.size()) << std::endl;
		people.clear();
	}
	bool flag = false;
	std::string last_file, result_file;
	for (size_t a = i, plus = 0; a != 1; a /= 2, plus += 100) {
		for (size_t j = 0; j < a - 1; ++j) {
			merge_files(generate_name(j + plus), generate_name(j++ + plus), generate_name(j / 2 + plus + 100));
			std::cout << j + plus << " + " << j - 1 + plus << " Всего:" << a << std::endl;
			std::cout << "remove " << generate_name(j + plus).c_str() << " and " << (generate_name(j - 1 + plus).c_str()) << std::endl;
		}
		if (flag) {
			merge_files(last_file, generate_name(100 + plus), "temp.txt");
			rename("temp.txt", generate_name(100 + plus).c_str());
			std::cout << 100 + plus << " + " << last_file << " = " << " Всего:" << a << std::endl;
			flag = false;
		}
		else if (a % 2 == 1) {
			flag = true;
			last_file = generate_name(a - 1 + plus);
			std::cout << "Остался " << a - 1 + plus << std::endl;
		}
		size_t result = ((a == 3) ? 1 : 0) + plus + 100;
		result_file = generate_name(result);
	}
	std::cout << "Отсортированный файл: " << result_file << std::endl;
}

int main() {
	setlocale(LC_ALL, "Russian");
	ext_sort("random.txt", 1024 * 5);
	system("pause");
	return 0;
}
