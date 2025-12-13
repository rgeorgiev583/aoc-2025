#include <iostream>
#include <vector>

int main() {
  using namespace std;

  vector<string> lines;

  while (cin.good()) {
    string line;
    getline(cin, line);
    if (cin.bad()) {
      cerr << "error: could not read line from standard input" << endl;
      return 1;
    }
    if (line.empty())
      continue;

    if (!lines.empty() && line.size() != lines[0].size()) {
      cerr << "warning: line has a different number of characters" << endl;
      return 1;
    }

    lines.push_back(std::move(line));
  }

  if (lines.empty()) {
    cerr << "error: empty standard input" << endl;
    return 1;
  }

  unsigned long long sum_answers{0};
  char op{'\0'};
  vector<unsigned long long> operands;
  for (size_t i{0}; i < lines[0].size(); i++) {
    vector<char> digits;
    digits.reserve(lines.size() - 1);
    bool isColumnOnlySpaces{true};
    for (auto lines_iter{lines.begin()}; lines_iter != lines.end();
         lines_iter++) {
      char ch{(*lines_iter)[i]};
      if (ch != ' ') {
        if (ch >= '0' && ch <= '9')
          digits.push_back(ch - '0');
        else
          op = ch;

        isColumnOnlySpaces = false;
      }
    }

    if (!isColumnOnlySpaces) {
      unsigned long long number{0};
      unsigned long long multiplier{1};
      for (auto digits_iter{digits.rbegin()}; digits_iter != digits.rend();
           digits_iter++) {
        number += *digits_iter * multiplier;
        multiplier *= 10;
      }
      operands.push_back(number);
    }

    if ((isColumnOnlySpaces || i == lines[0].size() - 1) && !operands.empty()) {
      unsigned long long answer{operands[0]};
      for (auto operands_iter{operands.begin() + 1};
           operands_iter != operands.end(); operands_iter++) {
        unsigned long long operand{*operands_iter};
        switch (op) {
        case '+':
          answer += operand;
          break;
        case '*':
          answer *= operand;
          break;
        default:
          if (op != '\0')
            cerr << "error: invalid operator: " << op << endl;
          else
            cerr << "error: no operator specified" << endl;
          return 1;
        }
      }
      sum_answers += answer;

      op = '\0';
      operands.clear();
    }
  }

  cout << sum_answers << endl;

  return 0;
}