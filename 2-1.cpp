#include <iostream>
#include <queue>
#include <sstream>
#include <stack>

int main() {
  using namespace std;

  unsigned long long sum_ids{0};

  string line;
  getline(cin, line);
  if (line.empty()) {
    cerr << "error: empty line" << endl;
    return 1;
  }

  istringstream line_stream{line};
  while (line_stream.good()) {
    string range;
    getline(line_stream, range, ',');
    if (!line_stream) {
      cerr << "error: could not parse range from line" << endl;
      return 1;
    }

    istringstream range_stream{range};
    unsigned long long first_id;
    unsigned long long last_id;
    range_stream >> first_id;
    if (!range_stream) {
      cerr << "error: could not parse first ID in range" << endl;
      return 1;
    }
    if (const auto dash{range_stream.get()}; dash != '-') {
      cerr << "error: range is not separated by dash (-)" << endl;
      return 1;
    }
    range_stream >> last_id;
    if (!range_stream) {
      cerr << "error: could not parse last ID in range" << endl;
      return 1;
    }

    for (unsigned long long i{first_id}; i <= last_id; i++) {
      unsigned long long id{i};
      stack<unsigned char> digits;
      while (id > 0) {
        digits.push(id % 10);
        id /= 10;
      }

      const size_t digit_count{digits.size()};
      if (digit_count % 2 == 0) {
        queue<unsigned char> half_digits;
        const size_t half_digit_count{digit_count / 2};
        for (size_t j{0}; j < half_digit_count; j++) {
          half_digits.push(digits.top());
          digits.pop();
        }

        size_t j{half_digit_count};
        while (j < digit_count && half_digits.front() == digits.top()) {
          half_digits.pop();
          digits.pop();
          j++;
        }

        if (j == digit_count)
          sum_ids += i;
      }
    }
  }

  cout << sum_ids << endl;

  return 0;
}