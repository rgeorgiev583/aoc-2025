#include <iostream>
#include <vector>

using namespace std;

static unsigned travel(vector<string> &manifold, size_t i, size_t j) {
  unsigned num_timelines{0};

  if (i >= manifold.size())
    return num_timelines;

  auto visit{[&manifold](size_t i, size_t j) {
    manifold[i][j] = '|';
    unsigned retval{travel(manifold, i + 1, j)};
    manifold[i][j] = '.';
    return retval;
  }};

  if (manifold[i][j] == '^') {
    if (j > 0) {
      num_timelines += visit(i, j - 1);
    }
    if (j < manifold[i].length() - 1) {
      num_timelines += 1 + visit(i, j + 1);
    }
  } else {
    num_timelines += visit(i, j);
  }

  return num_timelines;
}

int main() {
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

  unsigned num_timelines{0};
  for (size_t i{0}; i < lines.size(); i++) {
    bool found_particle{false};

    for (size_t j{0}; j < lines[i].length(); j++) {
      if (lines[i][j] == 'S') {
        lines[i][j] = '|';
        num_timelines = 1 + travel(lines, i + 1, j);
        found_particle = true;
        break;
      }
    }

    if (found_particle)
      break;
  }

  cout << num_timelines << endl;

  return 0;
}