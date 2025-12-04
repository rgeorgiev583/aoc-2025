#include <iostream>

int main() {
  using namespace std;

  long total_joltage{0};

  while (cin.good()) {
    string line;
    getline(cin, line);
    if (cin.bad()) {
      cerr << "error: could not read line from standard input" << endl;
      return 1;
    }
    if (line.empty())
      continue;

    if (line.length() < 12) {
      cerr << "error: line has fewer than twelve characters" << endl;
      return 1;
    }

    size_t max_number_pos{string::npos};
    long multiplier{100000000000};
    for (int i{11}; i >= 0; i--) {
      int max_number{0};
      for (size_t j{max_number_pos + 1}; j < line.length() - i; j++) {
        int number{line[j] - '0'};
        if (number > max_number) {
          max_number = number;
          max_number_pos = j;
        }
      }

      total_joltage += max_number * multiplier;
      multiplier /= 10;
    }
  }

  cout << total_joltage << endl;

  return 0;
}