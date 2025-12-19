#include <iostream>
#include <queue>
#include <sstream>
#include <vector>

int main() {
  using namespace std;

  struct machine {
    vector<bool> indicator_light_diagram;
    vector<vector<int>> button_wiring_schematics;
  };
  vector<machine> machines;
  while (cin.good()) {
    string line;
    getline(cin, line);
    if (cin.bad()) {
      cerr << "error: could not read line from standard input" << endl;
      return 1;
    }
    if (line.empty())
      continue;

    istringstream machine_stream{line};

    machine &m{machines.emplace_back()};

    const auto left_square_bracket{machine_stream.get()};
    if (!machine_stream) {
      cerr << "error: could not parse the `[` in the beginning of the "
              "indicator light diagram"
           << endl;
      return 1;
    }
    if (left_square_bracket != '[') {
      cerr << "error: the indicator light diagram does not begin with `[`"
           << endl;
      return 1;
    }

    string indicator_light_diagram_str;
    getline(machine_stream, indicator_light_diagram_str, ']');
    if (!machine_stream) {
      cerr << "error: could not parse the indicator light diagram" << endl;
      return 1;
    }

    m.indicator_light_diagram.reserve(indicator_light_diagram_str.length());
    for (auto indicator_light : indicator_light_diagram_str)
      m.indicator_light_diagram.push_back(indicator_light == '#');

    while (machine_stream.good()) {
      string button_wiring_schematic_str;
      machine_stream >> button_wiring_schematic_str;
      if (!machine_stream) {
        cerr << "error: could not parse button wiring schematic" << endl;
        return 1;
      }

      vector<int> &button_wiring_schematic{
          m.button_wiring_schematics.emplace_back()};
      istringstream button_wiring_schematic_stream{button_wiring_schematic_str};

      const auto left_parenthesis{button_wiring_schematic_stream.get()};
      if (!machine_stream) {
        cerr << "error: could not parse the `(` in the beginning of the "
                "button wiring schematic"
             << endl;
        return 1;
      }
      if (left_parenthesis == '{')
        break;
      else if (left_parenthesis != '(') {
        cerr << "error: the button wiring schematic does not begin with `(`"
             << endl;
        return 1;
      }

      while (button_wiring_schematic_stream.good()) {
        string indicator_light_str;
        getline(button_wiring_schematic_stream, indicator_light_str, ',');
        if (!button_wiring_schematic_stream) {
          cerr << "error: could not parse indicator light" << endl;
          return 1;
        }

        istringstream indicator_light_stream{indicator_light_str};
        int indicator_light_index;
        indicator_light_stream >> indicator_light_index;
        button_wiring_schematic.push_back(indicator_light_index);
      }
    }
  }

  if (machines.empty()) {
    cerr << "error: empty standard input" << endl;
    return 1;
  }

  int total_fewest_button_presses{0};
  struct machine_state {
    vector<bool> indicator_light_diagram;
    int num_button_presses;
  };
  for (const machine &m : machines) {
    queue<machine_state> states;
    machine_state &init_state{states.emplace()};
    init_state.indicator_light_diagram.resize(m.indicator_light_diagram.size());
    machine_state current_state;
    while (!states.empty()) {
      current_state = std::move(states.front());
      states.pop();
      if (current_state.indicator_light_diagram == m.indicator_light_diagram)
        break;

      for (const vector<int> &button : m.button_wiring_schematics) {
        machine_state new_state{current_state};
        for (int indicator_light : button) {
          new_state.indicator_light_diagram[indicator_light] =
              !new_state.indicator_light_diagram[indicator_light];
        }
        new_state.num_button_presses++;
        states.push(std::move(new_state));
      }
    }
    total_fewest_button_presses += current_state.num_button_presses;
  }

  cout << total_fewest_button_presses << endl;

  return 0;
}