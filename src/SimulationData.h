#ifndef SIMULATIONDATA_H
#define SIMULATIONDATA_H

#include <deque>
#include <mutex>
#include <vector>

namespace cer {

///
/// \brief The Position struct stores position in 2D
///
struct Position {
  float x, y, theta;
};

///
/// \brief The SimulationData class stores cars positions.
/// The positions are stored in a queues. One for each car. All methods
/// accessing the queues are thread safe.
///
class SimulationData {
 public:
  SimulationData() = default;

  ///
  /// \brief popPosition
  /// \param ind - index of the car
  /// \return Position of the specified car
  /// It deletes the returned position from the positions queue.
  ///
  Position popPosition(size_t ind);

  ///
  /// \brief pushPosition adds a new position the the queue
  /// \param ind - index of the car
  /// \param pos - position of the given car
  ///
  void pushPosition(size_t ind, const Position &pos);

  size_t carsNum() const;

  ///
  /// \brief reset clears the positions queues
  /// \param cars_num
  /// Reset method clears current positions queues and prepares the object to
  /// store positions of cars_num cars.
  ///
  void reset(size_t cars_num);

 private:
  std::vector<std::deque<Position>> positions_;
  mutable std::mutex mutex_;
};
}  // namespace cer

#endif  // SIMULATIONDATA_H
