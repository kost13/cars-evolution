#ifndef SIMULATIONDATA_H
#define SIMULATIONDATA_H

#include <deque>
#include <mutex>
#include <vector>

namespace cer {

struct Position {
  float x, y, theta;
};

class SimulationData {
 public:
  SimulationData() = default;
  Position popPosition(size_t ind);
  void pushPosition(size_t ind, const Position &pos);
  size_t carsNum() const;
  void reset(size_t cars_num);

 private:
  std::vector<std::deque<Position>> positions_;
  mutable std::mutex mutex_;
};
}  // namespace cer

#endif  // SIMULATIONDATA_H
