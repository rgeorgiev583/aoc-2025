#include <iostream>
#include <sstream>
#include <vector>

int main() {
  using namespace std;

  unsigned long long sum_ids{0};

  string line;
  getline(cin, line);
  if (cin.bad()) {
    cerr << "error: could not read line from standard input" << endl;
    return 1;
  }
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
    const auto dash{range_stream.get()};
    if (!range_stream) {
      cerr << "error: could not parse dash in range" << endl;
      return 1;
    }
    if (dash != '-') {
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
      vector<unsigned char> digits;
      while (id > 0) {
        digits.push_back(id % 10);
        id /= 10;
      }

      const size_t digit_count{digits.size()};
      if (digit_count % 2 == 0) {
        const size_t digit_sequence_size{digit_count / 2};

        size_t j{0};
        for (; j < digit_sequence_size; j++) {
          if (digits[j] != digits[digit_sequence_size + j])
            break;
        }

        if (j == digit_sequence_size)
          sum_ids += i;
      }
    }
  }

  cout << sum_ids << endl;

  return 0;
}