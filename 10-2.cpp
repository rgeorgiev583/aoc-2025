#include <iostream>
#include <queue>
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

    while (machine_stream.good()) {
      string button_wiring_schematic_str;
      machine_stream >> button_wiring_schematic_str;
      if (!machine_stream) {
        cerr << "error: could not parse button wiring schematic" << endl;
        return 1;
      }

      istringstream button_wiring_schematic_stream{button_wiring_schematic_str};

      const auto left_parenthesis{button_wiring_schematic_stream.get()};
      if (!machine_stream) {
        cerr << "error: could not parse the `(` in the beginning of the "
                "button wiring schematic"
             << endl;
        return 1;
      }
      if (left_parenthesis == '{') {
        string joltage_requirements_str;
        getline(button_wiring_schematic_stream, joltage_requirements_str, '}');
        if (!button_wiring_schematic_stream) {
          cerr << "error: could not parse the joltage requirements" << endl;
          return 1;
        }

        istringstream joltage_requirements_stream{joltage_requirements_str};

        while (joltage_requirements_stream.good()) {
          string joltage_requirement_str;
          getline(joltage_requirements_stream, joltage_requirement_str, ',');
          if (!joltage_requirements_stream) {
            cerr << "error: could not parse joltage requirement" << endl;
            return 1;
          }

          istringstream joltage_requirement_stream{joltage_requirement_str};
          int joltage_requirement;
          joltage_requirement_stream >> joltage_requirement;
          m.joltage_requirements.push_back(joltage_requirement);
        }

        break;
      } else if (left_parenthesis != '(') {
        cerr << "error: the button wiring schematic does not begin with `(`"
             << endl;
        return 1;
      }

      vector<int> &button_wiring_schematic{
          m.button_wiring_schematics.emplace_back()};
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
    vector<int> joltage_counters;
    int num_button_presses;
  };
  for (const machine &m : machines) {
    queue<machine_state> states;
    machine_state &init_state{states.emplace()};
    init_state.joltage_counters.resize(m.joltage_requirements.size());
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
        new_state.num_button_presses++;
        states.push(std::move(new_state));
      }
    }
    total_fewest_button_presses += current_state.num_button_presses;
  }

  cout << total_fewest_button_presses << endl;

  return 0;
}