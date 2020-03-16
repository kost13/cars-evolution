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
  explicit Opaque(CarsEvolutionRoot *parent) : p_(parent) {}

  physics::World world_;
  std::queue<Position> positions_;
  std::mutex queue_mutex_;
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
      o_->positions_.push(Position(0.0f, 2.5f * i, 0.2f * i));
    }
  }
}

Position CarsEvolutionRoot::getPosition() {
  std::lock_guard<std::mutex> lock(o_->queue_mutex_);
  logger::info() << "size: " << o_->positions_.size();
  if (o_->positions_.empty()) {
    return {-1, -1, {}};
  }
  auto position = o_->positions_.front();
  o_->positions_.pop();
  return position;
}
