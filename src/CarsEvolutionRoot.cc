#include "CarsEvolutionRoot.h"

#include <cpputils/logger.hpp>

#include "Evolution/Evolution.h"
#include "Physics/World.h"

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
  physics::World world_;
  evolution::Evolution evolution_;
};

cer::CarsEvolutionRoot::CarsEvolutionRoot()
    : o_(std::make_unique<Opaque>(this)) {}

void cer::CarsEvolutionRoot::generatePopulation() {
  o_->evolution_.generatePopulation();
}

cer::CarsEvolutionRoot::~CarsEvolutionRoot() = default;

void cer::CarsEvolutionRoot::runSimulation() { o_->world_.runSimulation(); /*o_->world_.runDummySimulation();*/ }

cer::ParametersMatrix cer::CarsEvolutionRoot::cars() const {
  return o_->cars_population_.cars();
}

void cer::CarsEvolutionRoot::setCars(const cer::ParametersMatrix &cars) {
  o_->cars_population_.setCars(cars);
}

cer::Position cer::CarsEvolutionRoot::popPosition(size_t car_index) {
  return o_->simulation_data_.popPosition(car_index);
}

std::vector<std::pair<double, double> > cer::CarsEvolutionRoot::getRoute() {
  return o_->world_.getRoute();
}
