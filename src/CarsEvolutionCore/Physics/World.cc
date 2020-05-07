#include "World.h"

#include <cpputils/logger.hpp>

#include "CarsPopulationData.h"
#include "SimulationData.h"

namespace logger = cpputils::log;

cer::physics::World::World(const cer::CarsPopulationData &population,
                           cer::SimulationData *simulation_data)
    : population_(population), simulation_data_(simulation_data), vec_{3, 4} {}

bool cer::physics::World::runSimulation() {
  auto cars_num = population_.cars().carsNum();
  simulation_data_->reset(cars_num);

  // dummy simulation
  std::vector<float> speeds(cars_num);
  for (auto &s : speeds) {
    s = float(rand() % 10);
  }
  for (int j = 0; j < 100; ++j) {
    // simulate computations
    //    logger::info() << "runSimulation" << j;
    {
      for (size_t i = 0; i < cars_num; ++i) {
        simulation_data_->pushPosition(
            i, Position{(1.5f * speeds[i] + 1.0f) * j,
                        (1.0f + 0.5f * speeds[i]) * j, speeds[i] * 0.1f * j});
      }
    }
  }
  return true;
}

float cer::physics::World::vecNorm() const { return vec_.Length(); }
