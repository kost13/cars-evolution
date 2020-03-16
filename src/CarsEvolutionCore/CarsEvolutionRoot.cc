#include "CarsEvolutionRoot.h"

#include <mutex>
#include <queue>

// temp
#include <chrono>
#include <iostream>
#include <thread>

#include "World.h"

struct CarsEvolutionRoot::Opaque {
  Opaque(CarsEvolutionRoot *parent) : p_(parent) {}

  physics::World world_;
  std::queue<Position> positions_;
  std::mutex queue_mutex_;
  CarsEvolutionRoot *p_;
};

CarsEvolutionRoot::CarsEvolutionRoot() : o_(std::make_unique<Opaque>(this)) {}

CarsEvolutionRoot::~CarsEvolutionRoot() {}

void CarsEvolutionRoot::runSimulation() {
  for (int i = 0; i < 100; ++i) {
    // simulate computations
    std::cout << "runSimulation" << i << std::endl;

    std::this_thread::sleep_for(std::chrono::milliseconds{40});
    {
      std::lock_guard<std::mutex> lock(o_->queue_mutex_);
      o_->positions_.push(Position(0.0f, 2.5f * i, 0.2f * i));
    }
  }
}

Position CarsEvolutionRoot::getPosition() {
  std::lock_guard<std::mutex> lock(o_->queue_mutex_);
  std::cout << "size: " << o_->positions_.size() << std::endl;
  if (o_->positions_.empty()) {
    return {-1, -1, {}};
  }
  auto position = std::move(o_->positions_.front());
  o_->positions_.pop();
  return position;
}
