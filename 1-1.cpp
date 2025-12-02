#include <iostream>
#include <sstream>

int main() {
  using namespace std;

  int dial_pos{50};
  int password{0};
  while (cin.good()) {
    string line;
    getline(cin, line);
    if (!cin) {
      cerr << "error: could not read line from standard input" << endl;
      return 1;
    }
    if (line.empty())
      continue;

    bool is_left_rotate;
    switch (line[0]) {
    case 'L':
      is_left_rotate = true;
      break;

    case 'R':
      is_left_rotate = false;
      break;

    default:
      cerr << "error: invalid first character on line" << endl;
      return 1;
    }

    istringstream line_stream{line.substr(1)};
    int num_rotate;
    line_stream >> num_rotate;
    if (!line_stream) {
      cerr << "error: could not parse number from line" << endl;
      return 1;
    }

    if (is_left_rotate)
      dial_pos -= num_rotate;
    else
      dial_pos += num_rotate;

    dial_pos %= 100;

    if (dial_pos == 0)
      password++;
  }

  cout << password << endl;

  return 0;
}