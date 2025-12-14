#include <iostream>
#include <vector>

using namespace std;

static unsigned travel(const vector<string> &manifold, size_t i, size_t j) {
  unsigned num_timelines{0};

  if (i >= manifold.size())
    return num_timelines;

  if (manifold[i][j] == '^') {
    if (j > 0) {
      num_timelines += travel(manifold, i + 1, j - 1);
    }
    if (j < manifold[i].length() - 1) {
      num_timelines += 1 + travel(manifold, i + 1, j + 1);
    }
  } else {
    num_timelines += travel(manifold, i + 1, j);
  }

  return num_timelines;
}

int main() {
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
      cerr << "error: line is of different length" << endl;
      return 1;
    }

    manifold.push_back(std::move(line));
  }

  if (manifold.empty()) {
    cerr << "error: empty standard input" << endl;
    return 1;
  }

  unsigned num_timelines{0};
  for (size_t i{0}; i < manifold.size(); i++) {
    bool found_particle{false};

    for (size_t j{0}; j < manifold[i].length(); j++) {
      if (manifold[i][j] == 'S') {
        num_timelines = 1 + travel(manifold, i + 1, j);
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