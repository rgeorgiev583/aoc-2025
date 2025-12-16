#include <iostream>
#include <set>
#include <sstream>
#include <vector>

using namespace std;

struct twod_pos {
  long x;
  long y;

  friend unsigned long distance_x(const twod_pos *lhs, const twod_pos *rhs) {
    return abs(lhs->x - rhs->x) + 1;
  }

  friend unsigned long distance_y(const twod_pos *lhs, const twod_pos *rhs) {
    return abs(lhs->y - rhs->y) + 1;
  }
};

struct rectangle {
  const twod_pos *corner1;
  const twod_pos *corner2;

  unsigned long long area() const {
    return distance_x(corner1, corner2) * distance_y(corner1, corner2);
  }

  friend bool operator<(const rectangle &lhs, const rectangle &rhs) {
    return lhs.area() < rhs.area();
  }
};

int main() {
  vector<twod_pos> tiles;
  while (cin.good()) {
    string line;
    getline(cin, line);
    if (cin.bad()) {
      cerr << "error: could not read line from standard input" << endl;
      return 1;
    }
    if (line.empty())
      continue;

    istringstream pos_stream{line};

    twod_pos &tile{tiles.emplace_back()};

    pos_stream >> tile.x;
    if (!pos_stream) {
      cerr << "error: could not parse X coordinate of tile position" << endl;
      return 1;
    }

    const auto comma{pos_stream.get()};
    if (!pos_stream) {
      cerr
          << "error: could not parse comma between coordinates of tile position"
          << endl;
      return 1;
    }
    if (comma != ',') {
      cerr << "error: coordinates of tile position are not separated by comma "
              "(,)"
           << endl;
      return 1;
    }

    pos_stream >> tile.y;
    if (!pos_stream) {
      cerr << "error: could not parse Y coordinate of tile position" << endl;
      return 1;
    }
  }

  if (tiles.empty()) {
    cerr << "error: empty standard input" << endl;
    return 1;
  }

  if (tiles.size() < 2) {
    cerr << "error: there should be at least two tiles" << endl;
    return 1;
  }

  set<rectangle> rectangles;
  for (auto i{tiles.begin()}; i != tiles.end() - 1; i++) {
    for (auto j{i + 1}; j != tiles.end(); j++) {
      rectangles.insert({&*i, &*j});
    }
  }

  cout << rectangles.rbegin()->area() << endl;

  return 0;
}