#include <iostream>

int main() {
  using namespace std;

  int total_joltage{0};

  while (cin.good()) {
    string line;
    getline(cin, line);
    if (cin.bad()) {
      cerr << "error: could not read line from standard input" << endl;
      return 1;
    }
    if (line.empty())
      continue;

    if (line.length() < 2) {
      cerr << "error: line has fewer than two characters" << endl;
      return 1;
    }

    int max_number{0};
    size_t max_number_pos{string::npos};
    for (size_t i{0}; i < line.length() - 1; i++) {
      int number{line[i] - '0'};
      if (number > max_number) {
        max_number = number;
        max_number_pos = i;
      }
    }

    int second_max_number{0};
    for (size_t i{max_number_pos + 1}; i < line.length(); i++) {
      int number{line[i] - '0'};
      if (number > second_max_number)
        second_max_number = number;
    }

    total_joltage += max_number * 10 + second_max_number;
  }

  cout << total_joltage << endl;

  return 0;
}