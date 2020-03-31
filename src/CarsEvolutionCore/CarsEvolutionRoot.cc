#include "CarsEvolutionRoot.h"

#include <mutex>
#include <queue>

#include "cpputils/logger.hpp"

// temp
#include <chrono>
#include <thread>

#include "World.h"

namespace logger = cpputils::log;

struct CarsEvolutionRoot::Opaque {
  explicit Opaque(CarsEvolutionRoot *parent) : p_(parent) {
    cars_.emplace_back(20, 20, std::vector<double>{40, 40, 120, 40, 100, 100,
                                                   80, 60, 60, 100});
    cars_.emplace_back(20, 30, std::vector<double>{40, 40, 120, 40, 100, 100,
                                                   80, 140, 60, 100});

    positions_ = std::vector<std::queue<Position>>(cars_.size());
  }

  physics::World world_;
  std::vector<std::queue<Position>> positions_;
  std::vector<Car::Parameters> cars_;
  std::mutex queue_mutex_;
  std::mutex car_mutex_;
  CarsEvolutionRoot *p_;
};

CarsEvolutionRoot::CarsEvolutionRoot() : o_(std::make_unique<Opaque>(this)) {}

CarsEvolutionRoot::~CarsEvolutionRoot() = default;

void CarsEvolutionRoot::runSimulation() {
  for (int i = 0; i < 100; ++i) {
    // simulate computations
    logger::info() << "runSimulation" << i;

    std::this_thread::sleep_for(std::chrono::milliseconds{40});
    {
      std::lock_guard<std::mutex> lock(o_->queue_mutex_);
      o_->positions_[0].emplace(0.0f, 2.5f * i, 0.2f * i);
      o_->positions_[1].emplace(0.5f * i, 2.0f * i, 0.6f * i);
    }
  }
}

Position CarsEvolutionRoot::getPosition(int car_num) {
  std::lock_guard<std::mutex> lock(o_->queue_mutex_);
  logger::info() << "size: " << o_->positions_.size();
  if (o_->positions_.size() < car_num + 1) {
    return {-1, -1, {}};
  }
  if (o_->positions_[car_num].empty()) {
    return {-1, -1, {}};
  }
  auto position = o_->positions_[car_num].front();
  o_->positions_[car_num].pop();
  return position;
}

std::vector<Car::Parameters> CarsEvolutionRoot::getCars() const {
  std::lock_guard<std::mutex> lock(o_->car_mutex_);
  return o_->cars_;
}
