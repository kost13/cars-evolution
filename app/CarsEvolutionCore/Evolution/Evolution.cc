#include "Evolution.h"

#include <cstdlib>
#include <ctime>
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
    cars.push_back(static_cast<double>((rand() % 25 + 15) / 100.));
    cars.push_back(static_cast<double>((rand() % 25 + 15) / 100.));

    std::vector<double> points =
        std::vector<double>{0.40, 0.40, 1.20, 0.40, 1.30, 0.60, 1.10, 0.80,
                            1.00, 1.00, 0.80, 1.10, 0.70, 1.05, 0.60, 1.00};
    for (auto &p : points) {
      cars.push_back(p + (rand() % 20 - 10) / 100.);
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

  population_fitness_ = std::vector<double>(population_->cars().carsNum());
  std::iota(population_fitness_.begin(), population_fitness_.end(), 1);

  auto params = population_->cars();

  math::RandomGenerator rg;
  rg.setStd(0.04);

  std::vector<double> children;
  children.reserve(params.size());

  for (size_t i = 0; i < params.carsNum(); ++i) {
    auto parents = math::tournamentSelection(population_fitness_, 2);
    auto child =
        math::crossover(params.begin(parents.at(0)), params.end(parents.at(0)),
                        params.begin(parents.at(1)), params.end(parents.at(1)));
    math::mutate(child.begin(), child.end(), rg);
    children.insert(children.end(), child.begin(), child.end());
  }

  population_->setCars(ParametersMatrix(children));
}