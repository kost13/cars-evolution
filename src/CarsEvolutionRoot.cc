// module: Core
// author: Lukasz Kostrzewa, Marcin Gajewski

#include "CarsEvolutionRoot.h"

#include "CarsPopulationData.h"
#include "Evolution/Evolution.h"
#include "Physics/World.h"
#include "SimulationData.h"

#include <algorithm>

struct cer::CarsEvolutionRoot::Opaque {
  explicit Opaque(CarsEvolutionRoot *parent, int seed)
      : p_(parent),
        world_(cars_population_, &simulation_data_),
        evolution_(&cars_population_, seed) {}

  CarsEvolutionRoot *p_;
  CarsPopulationData cars_population_;
  SimulationData simulation_data_;
  physics::World world_;
  evolution::Evolution evolution_;
};

cer::CarsEvolutionRoot::CarsEvolutionRoot(int seed)
    : o_(std::make_unique<Opaque>(this, seed)) {}

void cer::CarsEvolutionRoot::generatePopulation() {
  auto fitness = o_->world_.maxDistanceReached();

  if (!fitness.empty()) {
    o_->evolution_.setPopulationFitness(fitness);
  }
  o_->evolution_.generatePopulation();
}

cer::CarsEvolutionRoot::~CarsEvolutionRoot() = default;

void cer::CarsEvolutionRoot::runSimulation() { o_->world_.runSimulation(); }

std::pair<int, double> cer::CarsEvolutionRoot::getBestCar() {
  auto distances = o_->world_.maxDistanceReached();
  auto max_iter = std::max_element(distances.begin(), distances.end());
  return {std::distance(distances.begin(), max_iter), *max_iter};
}

cer::ParametersMatrix cer::CarsEvolutionRoot::cars() const {
  return o_->cars_population_.cars();
}

void cer::CarsEvolutionRoot::setCars(const cer::ParametersMatrix &cars) {
  o_->cars_population_.setCars(cars);
}

std::map<std::string, cer::evolution::Parameter>
cer::CarsEvolutionRoot::parameters() const {
  return o_->evolution_.parameters();
}

void cer::CarsEvolutionRoot::setParameterValue(const std::string &name,
                                               double val) {
  o_->evolution_.setParameterValue(name, val);
}

cer::Position cer::CarsEvolutionRoot::popPosition(size_t car_index) {
  return o_->simulation_data_.popPosition(car_index);
}

std::vector<std::pair<double, double> > cer::CarsEvolutionRoot::getRoute() {
  return o_->world_.getRoute();
}
