#include <algorithm>
#include <iostream>
#include <set>
#include <sstream>
#include <unordered_set>
#include <vector>

using namespace std;

struct point {
  long x;
  long y;
  long z;

  friend unsigned long long distance(const point *lhs, const point *rhs) {
    return ((lhs->x - rhs->x) * (lhs->x - rhs->x) +
            (lhs->y - rhs->y) * (lhs->y - rhs->y) +
            (lhs->z - rhs->z) * (lhs->z - rhs->z));
  }
};

struct connection {
  const point *from;
  const point *to;

  friend bool operator<(const connection &lhs, const connection &rhs) {
    return distance(lhs.from, lhs.to) < distance(rhs.from, rhs.to);
  }
};

int main() {
  using namespace std;

  vector<point> junction_boxes;
  while (cin.good()) {
    string junction_box_str;
    getline(cin, junction_box_str);
    if (cin.bad()) {
      cerr << "error: could not read junction box from standard input" << endl;
      return 1;
    }
    if (junction_box_str.empty())
      continue;

    istringstream junction_box_stream{junction_box_str};

    auto parse_comma{[&junction_box_str, &junction_box_stream]() {
      const auto comma{junction_box_stream.get()};
      if (!junction_box_stream) {
        cerr << "error: could not parse comma between coordinates of junction "
                "box position `"
             << junction_box_str << "`" << endl;
        return false;
      }
      if (comma != ',') {
        cerr << "error: coordinates of junction box position `"
             << junction_box_str
             << "` are not separated "
                "by comma (,): they are separated by `"
             << comma << "` instead" << endl;
        return false;
      }

      return true;
    }};

    point &junction_box{junction_boxes.emplace_back()};

    junction_box_stream >> junction_box.x;
    if (!junction_box_stream) {
      cerr << "error: could not parse X coordinate of junction box position `"
           << junction_box_str << "`" << endl;
      return 1;
    }

    if (!parse_comma())
      return 1;

    junction_box_stream >> junction_box.y;
    if (!junction_box_stream) {
      cerr << "error: could not parse Y coordinate of junction box position `"
           << junction_box_str << "`" << endl;
      return 1;
    }

    if (!parse_comma())
      return 1;

    junction_box_stream >> junction_box.z;
    if (!junction_box_stream) {
      cerr << "error: could not parse Z coordinate of junction box position `"
           << junction_box_str << "`" << endl;
      return 1;
    }
  }

  if (junction_boxes.empty()) {
    cerr << "error: empty standard input" << endl;
    return 1;
  }

  using circuit = unordered_set<const point *>;
  using circuit_container = vector<circuit>;
  circuit_container circuits;
  for (const point &junction_box : junction_boxes)
    circuits.push_back({&junction_box});

  set<connection> connections;
  for (auto i{junction_boxes.begin()}; i != junction_boxes.end() - 1; i++) {
    for (auto j{i + 1}; j != junction_boxes.end(); j++) {
      connections.insert({&*i, &*j});
    }
  }

  const connection *last_connection{nullptr};
  for (const connection &conn : connections) {
    auto get_existing_circuit_containing_junction_box{
        [&circuits](const point *junction_box) -> circuit * {
          auto circuit_iter{find_if(circuits.begin(), circuits.end(),
                                    [junction_box](const circuit &c) {
                                      return c.find(junction_box) != c.end();
                                    })};
          if (circuit_iter == circuits.end())
            return nullptr;

          return &*circuit_iter;
        }};

    circuit *circuit_from{
        get_existing_circuit_containing_junction_box(conn.from)};
    if (!circuit_from) {
      cerr << "error: junction box is not in any circuit" << endl;
      return 1;
    }
    circuit *circuit_to{get_existing_circuit_containing_junction_box(conn.to)};
    if (!circuit_to) {
      cerr << "error: junction box is not in any circuit" << endl;
      return 1;
    }
    if (circuit_from == circuit_to)
      continue;

    auto move_junction_box_between_two_circuits{[](circuit &from, circuit &to) {
      if (to.size() >= from.size()) {
        to.insert(from.begin(), from.end());
        from.clear();
      }
    }};
    move_junction_box_between_two_circuits(*circuit_to, *circuit_from);
    if (circuit_from->size() == junction_boxes.size()) {
      last_connection = &conn;
      break;
    }

    move_junction_box_between_two_circuits(*circuit_from, *circuit_to);
    if (circuit_to->size() == junction_boxes.size()) {
      last_connection = &conn;
      break;
    }
  }

  if (!last_connection) {
    cerr << "error: no last connection" << endl;
    return 1;
  }

  cout << last_connection->from->x * last_connection->to->x << endl;

  return 0;
}