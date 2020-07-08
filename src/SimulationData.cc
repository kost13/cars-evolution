#include "SimulationData.h"

#include <exception>

cer::Position cer::SimulationData::popPosition(size_t ind) {
  std::lock_guard<std::mutex> locker(mutex_);
  if (ind < positions_.size()) {
    if (!positions_[ind].empty()) {
      auto val = positions_[ind].front();
      positions_[ind].pop_front();
      return val;
    }
    return {-1, -1, -1};
  }
  throw std::out_of_range("poping position error");
}

void cer::SimulationData::pushPosition(size_t ind, const cer::Position &pos) {
  std::lock_guard<std::mutex> locker(mutex_);
  if (ind < positions_.size()) {
    positions_[ind].push_back(pos);
  } else {
    throw std::out_of_range("pushing position error");
  }
}

size_t cer::SimulationData::carsNum() const {
  std::lock_guard<std::mutex> locker(mutex_);
  return positions_.size();
}

void cer::SimulationData::reset(size_t cars_num) {
  std::lock_guard<std::mutex> locker(mutex_);
  positions_ = std::vector<std::deque<Position>>(cars_num);
}
