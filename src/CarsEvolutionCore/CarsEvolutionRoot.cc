#include "CarsEvolutionRoot.h"

#include <mutex>
#include <queue>

#include "cpputils/logger.hpp"

#include "Physics/World.h"

#include "Evolution/Evolution.h"

#include "CarsPopulationData.h"
#include "SimulationData.h"

struct cer::CarsEvolutionRoot::Opaque {
  explicit Opaque(CarsEvolutionRoot *parent)
      : p_(parent),
        world_(cars_population_, &simulation_data_),
        evolution_(&cars_population_) {
    srand(time(nullptr));
  }

  CarsEvolutionRoot *p_;
  CarsPopulationData cars_population_;
  SimulationData simulation_data_;
  evolution::Evolution evolution_;
  physics::World world_;
  std::mutex queue_mutex_;
};

cer::CarsEvolutionRoot::CarsEvolutionRoot()
    : o_(std::make_unique<Opaque>(this)) {}

void cer::CarsEvolutionRoot::generatePopulation() {
  o_->evolution_.generatePopulation();
}

cer::CarsEvolutionRoot::~CarsEvolutionRoot() = default;

void cer::CarsEvolutionRoot::runSimulation() { o_->world_.runSimulation(); }

cer::SimulationData *cer::CarsEvolutionRoot::simulationData() const {
  return &o_->simulation_data_;
}

cer::CarsPopulationData *cer::CarsEvolutionRoot::carsPopulation() const {
  return &o_->cars_population_;
}
