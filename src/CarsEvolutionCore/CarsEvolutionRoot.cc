#include "CarsEvolutionRoot.h"

#include <mutex>
#include <queue>

#include "cpputils/logger.hpp"

// temp
#include <chrono>
#include <thread>

#include "CarsPopulationData.h"
#include "World.h"

namespace logger = cpputils::log;

struct cer::CarsEvolutionRoot::Opaque {
  explicit Opaque(CarsEvolutionRoot *parent) : p_(parent) {
    //    cars_.emplace_back(20, 20, std::vector<double>{40, 40, 120, 40, 100,
    //    100,
    //                                                   80, 60, 60, 100});
    //    cars_.emplace_back(20, 30, std::vector<double>{40, 40, 120, 40, 100,
    //    100,
    //                                                   80, 140, 60, 100});

    positions_ = std::vector<std::queue<Position>>(10);
  }

  physics::World world_;
  std::vector<std::queue<Position>> positions_;
  //  std::vector<Car::Parameters> cars_;
  CarsPopulationData cars_population_;
  std::mutex queue_mutex_;
  std::mutex car_mutex_;
  CarsEvolutionRoot *p_;
};

cer::CarsEvolutionRoot::CarsEvolutionRoot()
    : o_(std::make_unique<Opaque>(this)) {}

cer::CarsEvolutionRoot::~CarsEvolutionRoot() = default;

void cer::CarsEvolutionRoot::runSimulation() {
  for (int i = 0; i < 100; ++i) {
    // simulate computations
    logger::info() << "runSimulation" << i;

    std::this_thread::sleep_for(std::chrono::milliseconds{10});
    {
      std::lock_guard<std::mutex> lock(o_->queue_mutex_);
      for (size_t i = 0; i < 10; ++i) {
        o_->positions_[i].emplace(0.0f, 2.5f * i, 0.2f * i);
      }
    }
  }
}

cer::Position cer::CarsEvolutionRoot::getPosition(int car_num) {
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

cer::CarsPopulationData *cer::CarsEvolutionRoot::carsPopulation() const {
  return &o_->cars_population_;
}
