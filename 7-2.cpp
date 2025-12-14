#include <iostream>
#include <vector>

using namespace std;

class teleporter {
  vector<string> manifold;
  vector<vector<unsigned>> num_timelines;
  unsigned num_timelines_from_start{0};

  unsigned travel(size_t i, size_t j) {
    if (i >= manifold.size())
      return 0;

    if (num_timelines[i][j] > 0)
      return num_timelines[i][j];

    if (manifold[i][j] == '^') {
      if (j > 0) {
        num_timelines[i][j] += travel(i + 1, j - 1);
      }
      if (j < manifold[i].length() - 1) {
        num_timelines[i][j] += 1 + travel(i + 1, j + 1);
      }
    } else {
      num_timelines[i][j] += travel(i + 1, j);
    }

    return num_timelines[i][j];
  }

public:
  explicit teleporter(std::istream &input) {
    while (input.good()) {
      string line;
      getline(input, line);
      if (input.bad()) {
        throw string{"error: could not read line from input"};
      }
      if (line.empty())
        continue;

      if (!manifold.empty() && line.length() != manifold[0].length()) {
        throw string{"error: line is of different length"};
      }

      manifold.push_back(line);
    }

    if (manifold.empty()) {
      throw string{"error: empty input"};
    }

    num_timelines.resize(manifold.size());
    for (size_t i{0}; i < manifold.size(); i++)
      num_timelines[i].resize(manifold[i].length());
  }

  unsigned travel() {
    if (num_timelines_from_start > 0)
      return num_timelines_from_start;

    for (size_t i{0}; i < manifold.size(); i++) {
      for (size_t j{0}; j < manifold[i].length(); j++) {
        if (manifold[i][j] == 'S') {
          num_timelines_from_start = num_timelines[i][j] = 1 + travel(i + 1, j);
          return num_timelines_from_start;
        }
      }
    }

    return 0;
  }
};

int main() {
  teleporter t{cin};
  cout << t.travel() << endl;

  return 0;
}