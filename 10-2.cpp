#include <iostream>
#include <queue>
#include <set>
#include <sstream>
#include <vector>

int main() {
  using namespace std;

  struct machine {
    vector<vector<int>> button_wiring_schematics;
    vector<int> joltage_requirements;
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

    machine &m{machines.emplace_back()};

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
      if (left_parenthesis == '{') {
        string joltage_requirements_str;
        getline(button_wiring_schematic_stream, joltage_requirements_str, '}');
        if (!button_wiring_schematic_stream) {
          cerr
              << "error: could not parse the joltage requirements for machine `"
              << machine_str << "`" << endl;
          return 1;
        }

        istringstream joltage_requirements_stream{joltage_requirements_str};

        while (joltage_requirements_stream.good()) {
          string joltage_requirement_str;
          getline(joltage_requirements_stream, joltage_requirement_str, ',');
          if (!joltage_requirements_stream) {
            cerr << "error: could not parse joltage requirement for machine `"
                 << machine_str << "`" << endl;
            return 1;
          }

          istringstream joltage_requirement_stream{joltage_requirement_str};
          int joltage_requirement;
          joltage_requirement_stream >> joltage_requirement;
          m.joltage_requirements.push_back(joltage_requirement);
        }

        break;
      } else if (left_parenthesis != '(') {
        cerr << "error: the button wiring schematic `"
             << button_wiring_schematic_str << "` for machine `" << machine_str
             << "` does not begin with `(`: it begins with `"
             << left_parenthesis << "` instead" << endl;
        return 1;
      }

      vector<int> &button_wiring_schematic{
          m.button_wiring_schematics.emplace_back()};
      while (button_wiring_schematic_stream.good()) {
        string button_indicator_light_str;
        getline(button_wiring_schematic_stream, button_indicator_light_str,
                ',');
        if (!button_wiring_schematic_stream) {
          cerr << "error: could not parse indicator light index in button "
                  "wiring schematic `"
               << button_wiring_schematic_str << "` for machine `"
               << machine_str << "`" << endl;
          return 1;
        }

        istringstream button_indicator_light_stream{button_indicator_light_str};
        int button_indicator_light_index;
        button_indicator_light_stream >> button_indicator_light_index;
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
    vector<int> joltage_counters;
    unsigned int num_button_presses;
  };
  for (const machine &m : machines) {
    queue<machine_state> states;
    machine_state &init_state{states.emplace()};
    init_state.joltage_counters.resize(m.joltage_requirements.size());
    set<vector<int>> visited_states;
    visited_states.insert(init_state.joltage_counters);
    machine_state current_state;
    while (!states.empty()) {
      current_state = std::move(states.front());
      states.pop();
      if (current_state.joltage_counters == m.joltage_requirements)
        break;

      for (const vector<int> &button : m.button_wiring_schematics) {
        machine_state new_state{current_state};
        for (int joltage_counter : button)
          new_state.joltage_counters[joltage_counter]++;

        if (visited_states.count(new_state.joltage_counters))
          continue;

        visited_states.insert(new_state.joltage_counters);
        new_state.num_button_presses++;
        states.push(std::move(new_state));
      }
    }
    total_fewest_button_presses += current_state.num_button_presses;
  }

  cout << total_fewest_button_presses << endl;

  return 0;
}