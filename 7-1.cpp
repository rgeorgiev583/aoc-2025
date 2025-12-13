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

    if (!lines.empty() && line.length() != lines[0].length()) {
      cerr << "error: line is of different length" << endl;
      return 1;
    }

    lines.push_back(line);
  }

  if (lines.empty()) {
    cerr << "error: empty standard input" << endl;
    return 1;
  }

  unsigned num_splits{0};
  for (size_t i{0}; i < lines.size(); i++) {
    for (size_t j{0}; j < lines[i].length(); j++) {
      switch (lines[i][j]) {
      case 'S':
        lines[i][j] = '|';
        break;

      case '^':
        if (i > 0 && lines[i - 1][j] == '|') {
          if (j > 0)
            lines[i][j - 1] = '|';
          if (j < lines[i].length() - 1)
            lines[i][j + 1] = '|';

          num_splits++;
        }
        break;

      default:
        if (i > 0 && lines[i - 1][j] == '|')
          lines[i][j] = '|';
      }
    }
  }

  cout << num_splits << endl;

  return 0;
}