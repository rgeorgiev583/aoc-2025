#include <algorithm>
#include <iostream>
#include <limits>
#include <optional>
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

  friend bool operator==(const twod_pos &lhs, const twod_pos &rhs) {
    return lhs.x == rhs.x && lhs.y == rhs.y;
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

  cout << endl;

  vector<long> min_y_for_x;
  vector<long> max_y_for_x;
  vector<long> min_x_for_y;
  vector<long> max_x_for_y;
  multiset<rectangle> rectangles;
  for (auto i{tiles.begin()}; i != tiles.end() - 1; i++) {
    for (auto j{i + 1}; j != tiles.end(); j++) {
      auto set_bounds_for_tile{[&min_y_for_x, &max_y_for_x, &min_x_for_y,
                                &max_x_for_y](const twod_pos &tile) {
        if (tile.x >= min_y_for_x.size())
          min_y_for_x.resize(tile.x + 1, numeric_limits<long>::max());
        if (tile.x >= max_y_for_x.size())
          max_y_for_x.resize(tile.x + 1, numeric_limits<long>::min());
        if (tile.y >= min_x_for_y.size())
          min_x_for_y.resize(tile.y + 1, numeric_limits<long>::max());
        if (tile.y >= max_x_for_y.size())
          max_x_for_y.resize(tile.y + 1, numeric_limits<long>::min());

        if (tile.y < min_y_for_x[tile.x])
          min_y_for_x[tile.x] = tile.y;
        if (tile.y > max_y_for_x[tile.x])
          max_y_for_x[tile.x] = tile.y;
        if (tile.x < min_x_for_y[tile.y])
          min_x_for_y[tile.y] = tile.x;
        if (tile.x > max_x_for_y[tile.y])
          max_x_for_y[tile.y] = tile.x;
      }};

      const twod_pos *corner1{&*i};
      const twod_pos *corner2{&*j};

      if (corner1->x == corner2->x) {
        const long x{corner1->x};
        for (long y{min(corner1->y, corner2->y)};
             y <= max(corner1->y, corner2->y); y++) {
          set_bounds_for_tile({x, y});
        }
      }
      if (corner1->y == corner2->y) {
        const long y{corner1->y};
        for (long x{min(corner1->x, corner2->x)};
             x <= max(corner1->x, corner2->x); x++) {
          set_bounds_for_tile({x, y});
        }
      }

      rectangles.insert({corner1, corner2});
    }
  }

  long max_y{numeric_limits<long>::min()};
  for (long current_max_y_for_x : max_y_for_x) {
    if (current_max_y_for_x > max_y)
      max_y = current_max_y_for_x;
  }

  long max_x{numeric_limits<long>::min()};
  for (long current_max_x_for_y : max_x_for_y) {
    if (current_max_x_for_y > max_x)
      max_x = current_max_x_for_y;
  }

  cout << "tiles:" << endl;
  for (long i{0}; i <= max_y; i++) {
    for (long j{0}; j <= max_x; j++) {
      if (find(tiles.begin(), tiles.end(), twod_pos{j, i}) != tiles.end())
        cout << '#';
      else
        cout << '.';
    }
    cout << endl;
  }
  cout << endl;

  cout << "loop:" << endl;
  for (long i{0}; i <= max_y; i++) {
    for (long j{0}; j <= max_x; j++) {
      if (i >= min_y_for_x[j] && i <= max_y_for_x[j] && j >= min_x_for_y[i] &&
          j <= max_x_for_y[i])
        cout << '#';
      else
        cout << '.';
    }
    cout << endl;
  }
  cout << endl;

  cout << "rectangles:" << endl << endl;
  std::optional<unsigned long long> max_area;
  size_t pos{0};
  for (auto i{rectangles.rbegin()}; i != rectangles.rend(); i++) {
    const twod_pos upper_left_corner{min(i->corner1->x, i->corner2->x),
                                     min(i->corner1->y, i->corner2->y)};
    const twod_pos upper_right_corner{max(i->corner1->x, i->corner2->x),
                                      min(i->corner1->y, i->corner2->y)};
    const twod_pos lower_left_corner{min(i->corner1->x, i->corner2->x),
                                     max(i->corner1->y, i->corner2->y)};
    const twod_pos lower_right_corner{max(i->corner1->x, i->corner2->x),
                                      max(i->corner1->y, i->corner2->y)};

    for (long j{0}; j <= max_y; j++) {
      for (long k{0}; k <= max_x; k++) {
        if (j >= upper_left_corner.y && j <= lower_right_corner.y &&
            k >= upper_left_corner.x && k <= lower_right_corner.x)
          cout << '#';
        else
          cout << '.';
      }
      cout << endl;
    }

    const unsigned long long area{i->area()};
    cout << "area: " << area << endl;

    auto is_within_bounds{[&min_y_for_x, &max_y_for_x, &min_x_for_y,
                           &max_x_for_y](const twod_pos &tile) {
      return tile.y >= min_y_for_x[tile.x] && tile.y <= max_y_for_x[tile.x] &&
             tile.x >= min_x_for_y[tile.y] && tile.x <= max_x_for_y[tile.y];
    }};

    if (is_within_bounds(upper_left_corner) &&
        is_within_bounds(upper_right_corner) &&
        is_within_bounds(lower_left_corner) &&
        is_within_bounds(lower_right_corner)) {
      cout << "within bounds" << endl;
      cout << "position in set: " << pos << endl;

      if (area > max_area)
        max_area = area;
    }

    cout << endl;

    pos++;
  }

  if (!max_area.has_value()) {
    cerr << "error: there are no rectangles with the given requirements"
         << endl;
    return 1;
  }

  cout << "max area: " << *max_area << endl;

  return 0;
}