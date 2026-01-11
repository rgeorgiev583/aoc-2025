#include <iostream>
#include <sstream>

int main() {
  using namespace std;

  int dial_number{50};
  unsigned int password{0};
  while (cin.good()) {
    string rotation;
    getline(cin, rotation);
    if (cin.bad()) {
      cerr << "error: could not read rotation from standard input" << endl;
      return 1;
    }
    if (rotation.empty())
      continue;

    bool is_left_rotation;
    switch (rotation[0]) {
    case 'L':
      is_left_rotation = true;
      break;

    case 'R':
      is_left_rotation = false;
      break;

    default:
      cerr << "error: invalid first character of rotation `" << rotation
           << "`: " << rotation[0] << endl;
      return 1;
    }

    istringstream rotation_stream{rotation.substr(1)};
    int rotation_distance;
    rotation_stream >> rotation_distance;
    if (!rotation_stream) {
      cerr << "error: could not parse distance from rotation `" << rotation
           << "`" << endl;
      return 1;
    }

    const bool was_dial_number_positive{dial_number > 0};
    const bool was_dial_number_negative{dial_number < 0};

    if (is_left_rotation)
      dial_number -= rotation_distance;
    else
      dial_number += rotation_distance;

    password += abs(dial_number) / 100;
    if (was_dial_number_positive && dial_number <= 0 ||
        was_dial_number_negative && dial_number >= 0)
      password++;

    dial_number %= 100;
  }

  cout << password << endl;

  return 0;
}