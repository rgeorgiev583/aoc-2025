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

  bool is_at_least_one_match;
  do {
    is_at_least_one_match = false;
    vector<ingredient_range> non_overlapping_fresh_ingredients;
    for (const auto &fresh_ingredient_range : fresh_ingredients) {
      bool is_match{false};
      for (auto &non_overlapping_fresh_ingredient_range :
           non_overlapping_fresh_ingredients) {
        if (fresh_ingredient_range.first_id >=
                non_overlapping_fresh_ingredient_range.first_id &&
            fresh_ingredient_range.last_id <=
                non_overlapping_fresh_ingredient_range.last_id) {
          is_match = true;
        }

        if (fresh_ingredient_range.first_id <
                non_overlapping_fresh_ingredient_range.first_id &&
            fresh_ingredient_range.last_id >=
                non_overlapping_fresh_ingredient_range.first_id) {
          non_overlapping_fresh_ingredient_range.first_id =
              fresh_ingredient_range.first_id;
          is_match = true;
        }

        if (fresh_ingredient_range.first_id <=
                non_overlapping_fresh_ingredient_range.last_id &&
            fresh_ingredient_range.last_id >
                non_overlapping_fresh_ingredient_range.last_id) {
          non_overlapping_fresh_ingredient_range.last_id =
              fresh_ingredient_range.last_id;
          is_match = true;
        }

        if (is_match) {
          is_at_least_one_match = true;
          break;
        }
      }

      if (!is_match)
        non_overlapping_fresh_ingredients.push_back(fresh_ingredient_range);
    }

    fresh_ingredients = non_overlapping_fresh_ingredients;
  } while (is_at_least_one_match);

  unsigned long long num_fresh_ingredients{0};
  for (const auto &fresh_ingredient_range : fresh_ingredients) {
    num_fresh_ingredients +=
        fresh_ingredient_range.last_id - fresh_ingredient_range.first_id + 1;
  }

  cout << num_fresh_ingredients << endl;

  return 0;
}