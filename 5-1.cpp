#include <iostream>
#include <sstream>
#include <vector>

int main() {
  using namespace std;

  struct ingredient_range {
    unsigned long long first_id;
    unsigned long long last_id;
  };
  vector<ingredient_range> fresh_ingredients;
  while (cin.good()) {
    string line;
    getline(cin, line);
    if (cin.bad()) {
      cerr << "error: could not read line from standard input" << endl;
      return 1;
    }
    if (line.empty())
      break;

    istringstream range_stream{line};
    unsigned long long first_ingredient_id;
    unsigned long long last_ingredient_id;
    range_stream >> first_ingredient_id;
    if (!range_stream) {
      cerr << "error: could not parse first ID in range" << endl;
      return 1;
    }
    const auto dash{range_stream.get()};
    if (!range_stream) {
      cerr << "error: could not parse dash in range" << endl;
      return 1;
    }
    if (dash != '-') {
      cerr << "error: range is not separated by dash (-)" << endl;
      return 1;
    }
    range_stream >> last_ingredient_id;
    if (!range_stream) {
      cerr << "error: could not parse last ID in range" << endl;
      return 1;
    }

    fresh_ingredients.push_back({first_ingredient_id, last_ingredient_id});
  }

  unsigned long long num_available_fresh_ingredients{0};
  while (cin.good()) {
    string line;
    getline(cin, line);
    if (cin.bad()) {
      cerr << "error: could not read line from standard input" << endl;
      return 1;
    }
    if (line.empty())
      break;

    istringstream line_stream{line};
    unsigned long long available_ingredient_id;
    line_stream >> available_ingredient_id;
    if (!line_stream) {
      cerr << "error: could not parse ingredient ID from line" << endl;
      return 1;
    }

    for (const auto &fresh_ingredient_range : fresh_ingredients) {
      if (available_ingredient_id >= fresh_ingredient_range.first_id &&
          available_ingredient_id <= fresh_ingredient_range.last_id) {
        num_available_fresh_ingredients++;
        break;
      }
    }
  }

  cout << num_available_fresh_ingredients << endl;

  return 0;
}