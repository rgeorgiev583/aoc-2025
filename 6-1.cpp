#include <iostream>
#include <sstream>
#include <vector>

int main() {
  using namespace std;

  vector<vector<string>> problems;

  while (cin.good()) {
    string line;
    getline(cin, line);
    if (cin.bad()) {
      cerr << "error: could not read line from standard input" << endl;
      return 1;
    }
    if (line.empty())
      continue;

    vector<string> row;
    if (!problems.empty())
      row.reserve(problems[0].size());
    istringstream line_stream{line};
    while (line_stream.good()) {
      string field;
      line_stream >> field;
      if (!field.empty())
        row.push_back(field);
    }

    if (!problems.empty() && row.size() != problems[0].size()) {
      cerr << "error: row `" << line << "` has a different number of fields ("
           << row.size() << ") from " << problems[0].size() << endl;
      return 1;
    }

    problems.push_back(std::move(row));
  }

  if (problems.empty()) {
    cerr << "error: empty standard input" << endl;
    return 1;
  }

  auto parse_number{[](const string &str) -> unsigned long long {
    istringstream str_stream{str};
    unsigned long long number;
    str_stream >> number;
    if (!str_stream) {
      cerr << "error: could not parse number from field `" << str << "`"
           << endl;
      return 1;
    }

    return number;
  }};
  vector<unsigned long long> answers;
  if (!problems.empty()) {
    answers.reserve(problems[0].size());
    for (const string &field : problems[0])
      answers.push_back(parse_number(field));
    const auto &operators{problems[problems.size() - 1]};
    for (size_t i{1}; i < problems.size() - 1; i++) {
      for (size_t j{0}; j < operators.size(); j++) {
        unsigned long long field{parse_number(problems[i][j])};

        if (operators[j] == "+")
          answers[j] += field;
        else if (operators[j] == "*")
          answers[j] *= field;
        else {
          cerr << "error: invalid operator: " << operators[j] << endl;
          return 1;
        }
      }
    }
  }

  unsigned long long sum_answers{0};
  for (const auto &answer : answers)
    sum_answers += answer;

  cout << sum_answers << endl;

  return 0;
}