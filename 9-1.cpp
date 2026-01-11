#include <iostream>
#include <set>
#include <sstream>
#include <vector>

using namespace std;

struct point {
  long x;
  long y;

  friend unsigned long distance_x(const point *lhs, const point *rhs) {
    return abs(lhs->x - rhs->x) + 1;
  }

  friend unsigned long distance_y(const point *lhs, const point *rhs) {
    return abs(lhs->y - rhs->y) + 1;
  }
};

struct rectangle {
  const point *corner1;
  const point *corner2;

  unsigned long long area() const {
    return distance_x(corner1, corner2) * distance_y(corner1, corner2);
  }

  friend bool operator<(const rectangle &lhs, const rectangle &rhs) {
    return lhs.area() < rhs.area();
  }
};

int main() {
  vector<point> tiles;
  while (cin.good()) {
    string tile_str;
    getline(cin, tile_str);
    if (cin.bad()) {
      cerr << "error: could not read tile from standard input" << endl;
      return 1;
    }
    if (tile_str.empty())
      continue;

    istringstream tile_stream{tile_str};

    point &tile{tiles.emplace_back()};

    tile_stream >> tile.x;
    if (!tile_stream) {
      cerr << "error: could not parse X coordinate of tile position `"
           << tile_str << "`" << endl;
      return 1;
    }

    const auto comma{tile_stream.get()};
    if (!tile_stream) {
      cerr << "error: could not parse comma between coordinates of tile "
              "position `"
           << tile_str << "`" << endl;
      return 1;
    }
    if (comma != ',') {
      cerr << "error: coordinates of tile position `" << tile_str
           << "` are not separated by comma "
              "(,): they are separated by `"
           << comma << "` instead" << endl;
      return 1;
    }

    tile_stream >> tile.y;
    if (!tile_stream) {
      cerr << "error: could not parse Y coordinate of tile position `"
           << tile_str << "`" << endl;
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