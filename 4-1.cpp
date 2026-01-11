#include <iostream>
#include <vector>

int main() {
  using namespace std;

  vector<string> grid;
  size_t row_length{string::npos};

  while (cin.good()) {
    string row;
    getline(cin, row);
    if (cin.bad()) {
      cerr << "error: could not read row from standard input" << endl;
      return 1;
    }
    if (row.empty())
      continue;

    grid.push_back(row);
    if (row_length == string::npos)
      row_length = row.length();
    else if (row.length() != row_length) {
      cerr << "error: row `" << row << "` is of different length ("
           << row.length() << ") from " << row_length << endl;
      return 1;
    }
  }

  if (grid.empty()) {
    cerr << "error: empty standard input" << endl;
    return 1;
  }

  unsigned int num_accessible_rolls{0};
  for (size_t i{0}; i < grid.size(); i++) {
    for (size_t j{0}; j < row_length; j++) {
      if (grid[i][j] != '@')
        continue;

      unsigned int num_adjacent_rolls{0};
      if (i > 0 && j > 0 && grid[i - 1][j - 1] == '@')
        num_adjacent_rolls++;
      if (i > 0 && grid[i - 1][j] == '@')
        num_adjacent_rolls++;
      if (i > 0 && j < row_length - 1 && grid[i - 1][j + 1] == '@')
        num_adjacent_rolls++;
      if (j < row_length - 1 && grid[i][j + 1] == '@')
        num_adjacent_rolls++;
      if (i < grid.size() - 1 && j < row_length - 1 &&
          grid[i + 1][j + 1] == '@')
        num_adjacent_rolls++;
      if (i < grid.size() - 1 && grid[i + 1][j] == '@')
        num_adjacent_rolls++;
      if (i < grid.size() - 1 && j > 0 && grid[i + 1][j - 1] == '@')
        num_adjacent_rolls++;
      if (j > 0 && grid[i][j - 1] == '@')
        num_adjacent_rolls++;

      if (num_adjacent_rolls < 4)
        num_accessible_rolls++;
    }
  }

  cout << num_accessible_rolls << endl;

  return 0;
}