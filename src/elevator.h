#ifndef ELEVATOR_H
#define ELEVATOR_H

#include <iostream>
#include <string>
#include <unordered_map>

class IElevator {
 public:
  virtual void call_elevator(size_t floor) = 0;
  virtual void get_floor(size_t floor) = 0;
  virtual void moving() = 0;
  virtual ~IElevator(){}
  virtual void continue_moving() = 0;

};

class PassengerElevator : public IElevator {
 private:
  const std::string enter_command_msg = " <Press \'Enter\' key to enter new command>\n";
  size_t max_floor;
  size_t current_floor;
  std::unordered_map<size_t, bool> floors;
  bool keep_moving;

  enum class State {
    STOP = 0,
    UP = 1,
    DOWN = 2
  };
  State current_state;

  bool found_next_destination();

 public:
  explicit PassengerElevator(size_t _max_floor);
  ~PassengerElevator() {}

  void get_floor(size_t floor) override;
  void call_elevator(size_t floor) override;
  void moving() override;
  void continue_moving() override;
};

#endif // ELEVATOR_H
