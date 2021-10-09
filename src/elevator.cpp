#include <iostream>
#include <algorithm>
#include <thread>
#include <string>
#include <time.h>
#include "elevator.h"

PassengerElevator::PassengerElevator(size_t _max_floor) :
    max_floor(_max_floor),
    current_state(State::STOP),
    keep_moving(false) {
  srand(static_cast<size_t>(time(NULL)));
  current_floor = 1 + static_cast<size_t>(rand()) % max_floor;

  for (size_t i = 1; i <= max_floor; ++i)
    floors[i] = false;

  std::cout << "Elevator stays on " << current_floor << " floor." << enter_command_msg;
}

void PassengerElevator::get_floor(size_t floor) {
  if (current_floor == floor) {
    std::cout << "Elevator stays on " << floor << " floor. Try another floor." << enter_command_msg;
    return;
  }

  floors[floor] = true;

  if (current_state == State::STOP)
    current_state = current_floor > floor ? State::DOWN : State::UP;
  else
    found_next_destination();

  keep_moving = true;
}

bool PassengerElevator::found_next_destination() {
  auto found_target = [this]() {
    auto result = std::find_if(floors.begin(), floors.end(), [this](std::pair<size_t, bool> floor) {
      bool moving_up = false;
      if (current_state == State::UP)
        moving_up = floor.first > current_floor;
      else if (current_state == State::DOWN)
        moving_up = floor.first < current_floor;

      return moving_up && floor.second;
    });

    return result != floors.end();
  };

  auto change_state = [this]() {
    if (current_state == State::UP)
      current_state = State::DOWN;
    else if (current_state == State::DOWN)
      current_state = State::UP;
  };

  if (found_target())
    return true;

  change_state();

  return found_target();
}

void PassengerElevator::moving() {
  if (!keep_moving)
    return;

  std::string cabin_status = "Cabin on floor " + std::to_string(current_floor) + ". Going ";
  if (current_state == State::DOWN) {
    --current_floor;
    cabin_status += "down. ";
  } else if (current_state == State::UP) {
    ++current_floor;
    cabin_status += "up. ";
  }
  std::cout << cabin_status << enter_command_msg;
  std::this_thread::sleep_for(std::chrono::seconds(1));

  if (floors[current_floor]) {
    floors[current_floor] = false;
    std::cout << "Cabin on floor " << current_floor << ". Stopped. Doors opened." << enter_command_msg;
    keep_moving = false;
  }
}

void PassengerElevator::call_elevator(size_t floor) {
  if (current_floor == floor) {
    std::cout << "Cabin on floor " << current_floor << ". Doors opened." << enter_command_msg;
    keep_moving = false;
    return;
  }
  get_floor(floor);
}

void PassengerElevator::continue_moving() {
  if (!found_next_destination()) {
    current_state = State::STOP;
    std::cout << "Elevator stays on " << current_floor << " floor." << enter_command_msg;
    keep_moving = false;
    return;
  }
  keep_moving = true;
}
