#include <iostream>
#include <thread>
#include <atomic>
#include <memory>
#include <regex>
#include <stdio.h>
#include "elevator.h"

class ElevatorController {
 private:
  const size_t floors_count_max;
  std::unique_ptr<IElevator> elevator;

 public:
  explicit ElevatorController(size_t floors_count) : floors_count_max(floors_count) {
    elevator.reset(new PassengerElevator(floors_count));
  }

  void work_iteration() {
    elevator->moving();
  }

  std::string enter_command() const {
    std::cout << "Call elevator >> FXX; get floor >> CXX; get out >> out; exit application >> exit " << std::endl;
    std::string command;
    std::cin >> command;
    std::cin.get();
    return command;
  }

  void process_command(const std::string& command) const {
    if (command.empty()) {
      std::cout << "Empty command!";
      return;
    }

    std::regex exp("([F|C])(\\d+)|out");
    std::smatch command_match;
    if (std::regex_search(command, command_match, exp)) {
      if (command_match[0] == "out") {
        elevator->continue_moving();
        return;
      }

      size_t floor_number = std::stoi(command_match[2].str());
      if (floor_number > floors_count_max) {
        std::cout << "Invalid floor number! Enter from 0 to " << floors_count_max;
        return;
      }
      if (command_match[1] == "F")
        elevator->call_elevator(floor_number);
      else if (command_match[1] == "C")
        elevator->get_floor(floor_number);
    } else {
      std::cout << "Invalid command!";
    }
  }
};

int main() {
  std::atomic<bool> run_app {true};

  size_t building_floors_count = 0;
  std::cout << "Enter Building max floors: ";
  std::cin >> building_floors_count; std::cin.get();

  ElevatorController elevator_controller(building_floors_count);

  std::atomic<bool> ready_to_read {true};
  std::atomic<char> key {'\0'};
  std::thread th([&key, &ready_to_read, &run_app]() {
    while(run_app.load()) {
      if (ready_to_read.load()) {
        key.store(static_cast<char>(getchar()));
        ready_to_read.store(false);
      }
    }
  });
  th.detach();

  std::string command;
  while(true) {
    elevator_controller.work_iteration();
    if (key.load() != '\0') {
      key.store('\0');

      command = elevator_controller.enter_command();
      if (command == "exit")
        break;

      elevator_controller.process_command(command);

      ready_to_read.store(true);
    }
  }
  run_app.store(false);
  return 0;
}
