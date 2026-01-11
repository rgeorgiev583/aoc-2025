#include <iostream>

int main() {
  using namespace std;

  long total_joltage{0};

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

    size_t max_joltage_pos{string::npos};
    long joltage_multiplier{100000000000};
    for (int i{11}; i >= 0; i--) {
      int max_joltage{0};
      for (size_t j{max_joltage_pos + 1}; j < bank.length() - i; j++) {
        int joltage{bank[j] - '0'};
        if (joltage > max_joltage) {
          max_joltage = joltage;
          max_joltage_pos = j;
        }
      }

      total_joltage += max_joltage * joltage_multiplier;
      joltage_multiplier /= 10;
    }
  }

  cout << total_joltage << endl;

  return 0;
}