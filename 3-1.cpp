#include <iostream>

int main() {
  using namespace std;

  int total_joltage{0};

  while (cin.good()) {
    string bank;
    getline(cin, bank);
    if (cin.bad()) {
      cerr << "error: could not read bank from standard input" << endl;
      return 1;
    }
    if (bank.empty())
      continue;

    if (bank.length() < 2) {
      cerr << "error: bank has fewer than two batteries" << endl;
      return 1;
    }

    int max_joltage{0};
    size_t max_joltage_pos{string::npos};
    for (size_t i{0}; i < bank.length() - 1; i++) {
      int joltage{bank[i] - '0'};
      if (joltage > max_joltage) {
        max_joltage = joltage;
        max_joltage_pos = i;
      }
    }

    int second_max_joltage{0};
    for (size_t i{max_joltage_pos + 1}; i < bank.length(); i++) {
      int joltage{bank[i] - '0'};
      if (joltage > second_max_joltage)
        second_max_joltage = joltage;
    }

    total_joltage += max_joltage * 10 + second_max_joltage;
  }

  cout << total_joltage << endl;

  return 0;
}