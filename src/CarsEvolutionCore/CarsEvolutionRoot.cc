#include "CarsEvolutionRoot.h"

#include <mutex>
#include <queue>

#include "cpputils/logger.hpp"

// temp
#include <chrono>
#include <cstdlib>
#include <thread>

#include "CarsPopulationData.h"
#include "World.h"

namespace logger = cpputils::log;

struct cer::CarsEvolutionRoot::Opaque {
  explicit Opaque(CarsEvolutionRoot *parent) : p_(parent) {
    positions_ = std::vector<std::queue<Position>>(10);
    srand(time(nullptr));
  }

  physics::World world_;
  std::vector<std::queue<Position>> positions_;
  CarsPopulationData cars_population_;
  std::mutex queue_mutex_;
  std::mutex car_mutex_;
  CarsEvolutionRoot *p_;
};

cer::CarsEvolutionRoot::CarsEvolutionRoot()
    : o_(std::make_unique<Opaque>(this)) {}

cer::CarsEvolutionRoot::~CarsEvolutionRoot() = default;

void cer::CarsEvolutionRoot::runSimulation() {
  std::vector<float> speeds(10);
  for (auto &s : speeds) {
    s = float(rand() % 10);
  }
  for (int j = 0; j < 100; ++j) {
    // simulate computations
    logger::info() << "runSimulation" << j;

    //    std::this_thread::sleep_for(std::chrono::milliseconds{50});
    {
      std::lock_guard<std::mutex> lock(o_->queue_mutex_);
      for (size_t i = 0; i < 10; ++i) {
        o_->positions_[i].emplace((1.5f * speeds[i] + 1.0) * j,
                                  (1 + 0.5f * speeds[i]) * j,
                                  speeds[i] * 0.1f * j);
      }
    }
  }
}

cer::Position cer::CarsEvolutionRoot::getPosition(int car_num) {
  std::lock_guard<std::mutex> lock(o_->queue_mutex_);

  if (o_->positions_.size() < car_num + 1) {
    return {-1, -1, {}};
  }
  if (o_->positions_[car_num].empty()) {
    return {-1, -1, {}};
  }
  auto position = o_->positions_[car_num].front();
  o_->positions_[car_num].pop();
  cpputils::log::info() << "car " << car_num << " left "
                        << o_->positions_[car_num].size();
  return position;
}

cer::CarsPopulationData *cer::CarsEvolutionRoot::carsPopulation() const {
  return &o_->cars_population_;
}
