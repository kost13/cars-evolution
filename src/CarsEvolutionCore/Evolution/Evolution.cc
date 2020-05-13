#include "Evolution.h"

#include <cstdlib>
#include <random>
#include <vector>
#include <ctime>

#include "CarsPopulationData.h"

namespace {

cer::ParametersMatrix dummyPopulation() {
  using cer::ParametersMatrix;

  size_t n{1};

  std::vector<double> cars;
  cars.reserve(n * ParametersMatrix::parametersNum());

  for (size_t i = 0; i < n; ++i) {
    cars.push_back(static_cast<double>((rand() % 25 + 15)/100.));
    cars.push_back(static_cast<double>((rand() % 25 + 15)/100.));

    std::vector<double> points = std::vector<double>{
        0.40, 0.40, 1.20, 0.40, 1.30, 0.60, 1.10, 0.80, 1.00, 1.00, 0.80, 1.10, 0.70, 1.05, 0.60, 1.00};
    for (auto &p : points) {
      cars.push_back(p + (rand() % 20 - 10)/100.);
    }
  }

  return ParametersMatrix{cars};
}
}  // namespace

cer::evolution::Evolution::Evolution(cer::CarsPopulationData *population,
                                     bool time_seed)
    : population_(population) {
  if (time_seed) {
    std::srand(time(nullptr));
  }
}

void cer::evolution::Evolution::setPopulationFitness(
    const std::vector<double> &fitness) {
  population_fitness_ = fitness;
}

void cer::evolution::Evolution::generatePopulation() {
  if (population_->empty()) {
    population_->setCars(dummyPopulation());
    return;
  }

  auto params = population_->cars();

  RandomGenerator rg;

  for (size_t i = 0; i < params.carsNum(); ++i) {
    cer::evolution::mutate(params.begin(i), params.end(i), rg);
  }
  population_->setCars(params);
}
