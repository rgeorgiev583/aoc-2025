#include <algorithm>
#include <iostream>
#include <sstream>
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

  using circuit = vector<const threed_pos *>;
  using circuit_container = vector<circuit>;
  circuit_container circuits;
  for (const threed_pos &junction_box : junction_boxes)
    circuits.push_back({&junction_box});

  struct connection {
    const threed_pos *from;
    const threed_pos *to;
  };
  vector<connection> connections;
  int num_connections{0};
  while (num_connections < num_shortest_connections) {
    auto does_connection_exist_between_junction_boxes{
        [&connections](const threed_pos *lhs, const threed_pos *rhs) {
          return any_of(connections.begin(), connections.end(),
                        [&lhs, &rhs](const connection &conn) {
                          return conn.from == lhs && conn.to == rhs ||
                                 conn.from == rhs && conn.to == lhs;
                        });
        }};
    const threed_pos *closest_junction_box1{nullptr};
    const threed_pos *closest_junction_box2{nullptr};
    bool is_closest_junction_box_pair_set{false};
    for (auto i{junction_boxes.begin()}; i != junction_boxes.end() - 1; i++) {
      for (auto j{i + 1}; j != junction_boxes.end(); j++) {
        const threed_pos *junction_box_i{&*i};
        const threed_pos *junction_box_j{&*j};
        if (does_connection_exist_between_junction_boxes(junction_box_i,
                                                         junction_box_j))
          continue;

        if (!is_closest_junction_box_pair_set ||
            distance(junction_box_i, junction_box_j) <
                distance(closest_junction_box1, closest_junction_box2)) {
          closest_junction_box1 = junction_box_i;
          closest_junction_box2 = junction_box_j;
          is_closest_junction_box_pair_set = true;
        }
      }
    }
    if (!is_closest_junction_box_pair_set)
      break;

    connections.push_back({closest_junction_box1, closest_junction_box2});
    num_connections++;

    auto get_existing_circuit_containing_junction_box{
        [&circuits](const threed_pos *junction_box) -> circuit * {
          auto circuit_iter{find_if(circuits.begin(), circuits.end(),
                                    [junction_box](const circuit &c) {
                                      return find(c.begin(), c.end(),
                                                  junction_box) != c.end();
                                    })};
          if (circuit_iter == circuits.end())
            return nullptr;

          return &*circuit_iter;
        }};
    auto circuit_containing_closest_junction_box1{
        get_existing_circuit_containing_junction_box(closest_junction_box1)};
    if (!circuit_containing_closest_junction_box1) {
      std::cerr << "error: junction box is not in any circuit" << endl;
      return 1;
    }
    auto circuit_containing_closest_junction_box2{
        get_existing_circuit_containing_junction_box(closest_junction_box2)};
    if (!circuit_containing_closest_junction_box2) {
      std::cerr << "error: junction box is not in any circuit" << endl;
      return 1;
    }
    if (circuit_containing_closest_junction_box1 ==
        circuit_containing_closest_junction_box2)
      continue;

    auto move_junction_box_between_two_circuits{[](circuit &from, circuit &to) {
      if (to.size() >= from.size()) {
        to.insert(to.end(), from.begin(), from.end());
        from.clear();
      }
    }};
    move_junction_box_between_two_circuits(
        *circuit_containing_closest_junction_box2,
        *circuit_containing_closest_junction_box1);
    move_junction_box_between_two_circuits(
        *circuit_containing_closest_junction_box1,
        *circuit_containing_closest_junction_box2);
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