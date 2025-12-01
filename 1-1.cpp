#include <iostream>
#include <sstream>

int main() {
  using namespace std;

  int dial_pos{50};
  int password{0};
  while (cin.good()) {
    string line;
    getline(cin, line);
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

    istringstream is{line.substr(1)};
    int num_rotate;
    is >> num_rotate;

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