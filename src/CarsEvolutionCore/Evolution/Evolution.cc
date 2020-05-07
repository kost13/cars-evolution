#include "Evolution.h"

#include <cstdlib>
#include <random>
#include <vector>

#include "CarsPopulationData.h"

namespace {

cer::ParametersMatrix dummyPopulation() {
  using cer::ParametersMatrix;

  size_t n{10};

  std::vector<double> cars;
  cars.reserve(n * ParametersMatrix::parametersNum());

  for (size_t i = 0; i < n; ++i) {
    cars.push_back(static_cast<double>(rand() % 25 + 15));
    cars.push_back(static_cast<double>(rand() % 25 + 15));

    std::vector<double> points = std::vector<double>{
        40, 40, 120, 40, 130, 60, 110, 80, 100, 100, 80, 110, 70, 105, 60, 100};
    for (auto &p : points) {
      cars.push_back(p + rand() % 20 - 10);
    }
  }

  return ParametersMatrix{cars};
}
}  // namespace

cer::evolution::Evolution::Evolution(cer::CarsPopulationData *population,
                                     bool time_seed)
    : population_(population) {
  if (time_seed) {
    std::srand(std::time(nullptr));
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
