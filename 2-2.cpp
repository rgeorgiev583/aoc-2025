#include <iostream>
#include <sstream>
#include <vector>

int main() {
  using namespace std;

  unsigned long long sum_ids{0};

  while (cin.good()) {
    string range;
    getline(cin, range, ',');
    if (!cin) {
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
      for (size_t j{2}; j <= digit_count; j++) {
        if (digit_count % j == 0) {
          const size_t digit_sequence_size{digit_count / j};

          size_t k{1};
          for (; k < j; k++) {
            size_t l{0};
            for (; l < digit_sequence_size; l++) {
              if (digits[l] != digits[k * digit_sequence_size + l])
                break;
            }

            if (l != digit_sequence_size)
              break;
          }

          if (k == j) {
            sum_ids += i;
            break;
          }
        }
      }
    }
  }

  cout << sum_ids << endl;

  return 0;
}