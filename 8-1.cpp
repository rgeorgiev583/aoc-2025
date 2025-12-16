#include <algorithm>
#include <iostream>
#include <set>
#include <sstream>
#include <unordered_set>
#include <vector>

struct threed_pos {
  long x;
  long y;
  long z;

  friend unsigned long long distance(const threed_pos *lhs,
                                     const threed_pos *rhs) {
    return ((lhs->x - rhs->x) * (lhs->x - rhs->x) +
            (lhs->y - rhs->y) * (lhs->y - rhs->y) +
            (lhs->z - rhs->z) * (lhs->z - rhs->z));
  }
};

struct connection {
  const threed_pos *from;
  const threed_pos *to;

  friend bool operator<(const connection &lhs, const connection &rhs) {
    return distance(lhs.from, lhs.to) < distance(rhs.from, rhs.to);
  }
};

int main() {
  using namespace std;

  int num_shortest_connections;
  cin >> num_shortest_connections;
  if (!cin) {
    cerr << "error: could not read number of shortest connections from "
            "standard input"
         << endl;
    return 1;
  }
  if (num_shortest_connections < 0) {
    cerr << "error: number of shortest connections is negative" << endl;
    return 1;
  }

  vector<threed_pos> junction_boxes;
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

    auto parse_comma{[&pos_stream]() {
      const auto comma{pos_stream.get()};
      if (!pos_stream) {
        cerr << "error: could not parse comma between coordinates of junction "
                "box position"
             << endl;
        return false;
      }
      if (comma != ',') {
        cerr << "error: coordinates of junction box position are not separated "
                "by comma (,)"
             << endl;
        return false;
      }

      return true;
    }};

    threed_pos &junction_box{junction_boxes.emplace_back()};

    pos_stream >> junction_box.x;
    if (!pos_stream) {
      cerr << "error: could not parse X coordinate of junction box position"
           << endl;
      return 1;
    }

    if (!parse_comma())
      return 1;

    pos_stream >> junction_box.y;
    if (!pos_stream) {
      cerr << "error: could not parse Y coordinate of junction box position"
           << endl;
      return 1;
    }

    if (!parse_comma())
      return 1;

    pos_stream >> junction_box.z;
    if (!pos_stream) {
      cerr << "error: could not parse Z coordinate of junction box position"
           << endl;
      return 1;
    }
  }

  if (junction_boxes.empty()) {
    cerr << "error: empty standard input" << endl;
    return 1;
  }

  using circuit = unordered_set<const threed_pos *>;
  using circuit_container = vector<circuit>;
  circuit_container circuits;
  for (const threed_pos &junction_box : junction_boxes)
    circuits.push_back({&junction_box});

  set<connection> connections;
  for (auto i{junction_boxes.begin()}; i != junction_boxes.end() - 1; i++) {
    for (auto j{i + 1}; j != junction_boxes.end(); j++) {
      connections.insert({&*i, &*j});
    }
  }

  int num_connections{0};
  for (auto i{connections.begin()};
       i != connections.end() && num_connections < num_shortest_connections;
       i++, num_connections++) {
    auto get_existing_circuit_containing_junction_box{
        [&circuits](const threed_pos *junction_box) -> circuit * {
          auto circuit_iter{find_if(circuits.begin(), circuits.end(),
                                    [junction_box](const circuit &c) {
                                      return c.find(junction_box) != c.end();
                                    })};
          if (circuit_iter == circuits.end())
            return nullptr;

          return &*circuit_iter;
        }};

    const connection &conn{*i};
    circuit *circuit_from{
        get_existing_circuit_containing_junction_box(conn.from)};
    if (!circuit_from) {
      std::cerr << "error: junction box is not in any circuit" << endl;
      return 1;
    }
    circuit *circuit_to{get_existing_circuit_containing_junction_box(conn.to)};
    if (!circuit_to) {
      std::cerr << "error: junction box is not in any circuit" << endl;
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
    move_junction_box_between_two_circuits(*circuit_from, *circuit_to);
  }

  if (circuits.size() < 3) {
    cerr << "error: number of circuits is less than three" << endl;
    return 1;
  }

  sort(circuits.begin(), circuits.end(),
       [](const circuit &lhs, const circuit &rhs) {
         return lhs.size() > rhs.size();
       });

  cout << circuits[0].size() * circuits[1].size() * circuits[2].size() << endl;

  return 0;
}