#include <iostream>
#include <vector>

int main() {
  using namespace std;

  vector<string> manifold;

  while (cin.good()) {
    string line;
    getline(cin, line);
    if (cin.bad()) {
      cerr << "error: could not read line from standard input" << endl;
      return 1;
    }
    if (line.empty())
      continue;

    if (!manifold.empty() && line.length() != manifold[0].length()) {
      cerr << "error: line `" << line << "` is of different length ("
           << line.length() << ") from " << manifold[0].length() << endl;
      return 1;
    }

    manifold.push_back(line);
  }

  if (manifold.empty()) {
    cerr << "error: empty standard input" << endl;
    return 1;
  }

  unsigned int num_splits{0};
  for (size_t i{0}; i < manifold.size(); i++) {
    for (size_t j{0}; j < manifold[i].length(); j++) {
      switch (manifold[i][j]) {
      case 'S':
        manifold[i][j] = '|';
        break;

      case '^':
        if (i > 0 && manifold[i - 1][j] == '|') {
          if (j > 0)
            manifold[i][j - 1] = '|';
          if (j < manifold[i].length() - 1)
            manifold[i][j + 1] = '|';

          num_splits++;
        }
        break;

      default:
        if (i > 0 && manifold[i - 1][j] == '|')
          manifold[i][j] = '|';
      }
    }
  }

  cout << num_splits << endl;

  return 0;
}