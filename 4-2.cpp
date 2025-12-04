#include <iostream>
#include <vector>

int main() {
  using namespace std;

  vector<string> lines;
  size_t line_length{string::npos};

  while (cin.good()) {
    string line;
    getline(cin, line);
    if (cin.bad()) {
      cerr << "error: could not read line from standard input" << endl;
      return 1;
    }
    if (line.empty())
      continue;

    lines.push_back(line);
    if (line_length == string::npos)
      line_length = line.length();
    else if (line.length() != line_length) {
      cerr << "error: line is of different length" << endl;
      return 1;
    }
  }

  if (lines.empty()) {
    cerr << "error: empty standard input" << endl;
    return 1;
  }

  int num_removed_rolls{0};
  int num_accessible_rolls;
  do {
    num_accessible_rolls = 0;
    for (size_t i{0}; i < lines.size(); i++) {
      for (size_t j{0}; j < line_length; j++) {
        if (lines[i][j] != '@')
          continue;

        int num_adjacent_rolls{0};
        if (i > 0 && j > 0 && lines[i - 1][j - 1] == '@')
          num_adjacent_rolls++;
        if (i > 0 && lines[i - 1][j] == '@')
          num_adjacent_rolls++;
        if (i > 0 && j < line_length - 1 && lines[i - 1][j + 1] == '@')
          num_adjacent_rolls++;
        if (j < line_length - 1 && lines[i][j + 1] == '@')
          num_adjacent_rolls++;
        if (i < lines.size() - 1 && j < line_length - 1 &&
            lines[i + 1][j + 1] == '@')
          num_adjacent_rolls++;
        if (i < lines.size() - 1 && lines[i + 1][j] == '@')
          num_adjacent_rolls++;
        if (i < lines.size() - 1 && j > 0 && lines[i + 1][j - 1] == '@')
          num_adjacent_rolls++;
        if (j > 0 && lines[i][j - 1] == '@')
          num_adjacent_rolls++;

        if (num_adjacent_rolls < 4) {
          lines[i][j] = 'x';
          num_removed_rolls++;
          num_accessible_rolls++;
        }
      }
    }
  } while (num_accessible_rolls > 0);

  cout << num_removed_rolls << endl;

  return 0;
}