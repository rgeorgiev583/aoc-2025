#include <iostream>
#include <queue>
#include <set>
#include <sstream>
#include <vector>

int main() {
  using namespace std;

  struct machine {
    vector<bool> indicator_light_diagram;
    vector<vector<unsigned int>> button_wiring_schematics;
  };
  vector<machine> machines;
  while (cin.good()) {
    string machine_str;
    getline(cin, machine_str);
    if (cin.bad()) {
      cerr << "error: could not read machine from standard input" << endl;
      return 1;
    }
    if (machine_str.empty())
      continue;

    istringstream machine_stream{machine_str};

    machine &current_machine{machines.emplace_back()};

    const auto left_square_bracket{machine_stream.get()};
    if (!machine_stream) {
      cerr << "error: could not parse the `[` in the beginning of the "
              "indicator light diagram for machine `"
           << machine_str << "`" << endl;
      return 1;
    }
    if (left_square_bracket != '[') {
      cerr << "error: the indicator light diagram for machine `" << machine_str
           << " ` does not begin with `[`" << endl;
      return 1;
    }

    string indicator_light_diagram_str;
    getline(machine_stream, indicator_light_diagram_str, ']');
    if (!machine_stream) {
      cerr << "error: could not parse the indicator light diagram for machine "
           << machine_str << "`" << endl;
      return 1;
    }

    current_machine.indicator_light_diagram.reserve(
        indicator_light_diagram_str.length());
    for (auto indicator_light : indicator_light_diagram_str)
      current_machine.indicator_light_diagram.push_back(indicator_light == '#');

    while (machine_stream.good()) {
      string button_wiring_schematic_str;
      machine_stream >> button_wiring_schematic_str;
      if (!machine_stream) {
        cerr << "error: could not parse button wiring schematic for machine `"
             << machine_str << "`" << endl;
        return 1;
      }

      istringstream button_wiring_schematic_stream{button_wiring_schematic_str};

      const auto left_parenthesis{button_wiring_schematic_stream.get()};
      if (!machine_stream) {
        cerr << "error: could not parse the `(` in the beginning of the "
                "button wiring schematic `"
             << button_wiring_schematic_str << "` for machine `" << machine_str
             << "`" << endl;
        return 1;
      }
      if (left_parenthesis == '{')
        break;
      else if (left_parenthesis != '(') {
        cerr << "error: the button wiring schematic `"
             << button_wiring_schematic_str << "` for machine `" << machine_str
             << "` does not begin with `(`: it begins with `"
             << left_parenthesis << "` instead" << endl;
        return 1;
      }

      vector<unsigned int> &button_wiring_schematic{
          current_machine.button_wiring_schematics.emplace_back()};
      while (button_wiring_schematic_stream.good()) {
        string button_indicator_light_str;
        getline(button_wiring_schematic_stream, button_indicator_light_str,
                ',');
        if (!button_wiring_schematic_stream) {
          cerr << "error: could not read indicator light index from button "
                  "wiring schematic `"
               << button_wiring_schematic_str << "` for machine `"
               << machine_str << "`" << endl;
          return 1;
        }

        istringstream button_indicator_light_stream{button_indicator_light_str};
        int button_indicator_light_index;
        button_indicator_light_stream >> button_indicator_light_index;
        if (!button_indicator_light_stream) {
          cerr << "error: could not parse indicator light index in button "
                  "wiring schematic `"
               << button_wiring_schematic_str << "` for machine `"
               << machine_str << "`" << endl;
          return 1;
        }
        if (button_indicator_light_index < 0) {
          cerr << "error: button indicator light index is negative: "
               << button_indicator_light_index << endl;
          return 1;
        }

        button_wiring_schematic.push_back(button_indicator_light_index);
      }
    }
  }

  if (machines.empty()) {
    cerr << "error: empty standard input" << endl;
    return 1;
  }

  unsigned int total_fewest_button_presses{0};
  struct machine_state {
    vector<bool> indicator_light_diagram;
    unsigned int num_button_presses;
  };
  for (const machine &current_machine : machines) {
    queue<machine_state> states;
    machine_state &init_state{states.emplace()};
    init_state.indicator_light_diagram.resize(
        current_machine.indicator_light_diagram.size());
    set<vector<bool>> visited_states;
    visited_states.insert(init_state.indicator_light_diagram);
    machine_state current_state;
    while (!states.empty()) {
      current_state = std::move(states.front());
      states.pop();
      if (current_state.indicator_light_diagram ==
          current_machine.indicator_light_diagram)
        break;

      for (const vector<unsigned int> &button :
           current_machine.button_wiring_schematics) {
        machine_state new_state{current_state};
        for (unsigned int indicator_light : button)
          new_state.indicator_light_diagram[indicator_light] =
              !new_state.indicator_light_diagram[indicator_light];

        if (visited_states.count(new_state.indicator_light_diagram))
          continue;

        visited_states.insert(new_state.indicator_light_diagram);
        new_state.num_button_presses++;
        states.push(std::move(new_state));
      }
    }
    total_fewest_button_presses += current_state.num_button_presses;
  }

  cout << total_fewest_button_presses << endl;

  return 0;
}