#include <iostream>
#include <sstream>
#include <unordered_set>

int main() {
  using namespace std;

  unordered_set<unsigned long long> fresh_ingredients;
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

    for (unsigned long long i{first_ingredient_id}; i <= last_ingredient_id;
         i++)
      fresh_ingredients.insert(i);
  }

  unsigned long long num_available_fresh_ingredients{0};
  while (cin.good()) {
    unsigned long long available_ingredient_id;
    cin >> available_ingredient_id;
    if (!cin) {
      cerr << "error: could not read ingredient ID from standard input" << endl;
      return 1;
    }

    if (fresh_ingredients.count(available_ingredient_id))
      num_available_fresh_ingredients++;
  }

  cout << num_available_fresh_ingredients << endl;

  return 0;
}