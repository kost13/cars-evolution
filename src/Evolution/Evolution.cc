// module: Core.Evolution
// author: Lukasz Kostrzewa

#include "Evolution.h"

#include <chrono>
#include <cstdlib>
#include <ctime>
#include <numeric>
#include <random>
#include <vector>

#include <cpputils/logger.hpp>

#include "CarsPopulationData.h"

namespace {

cer::ParametersMatrix dummyPopulation(size_t n) {
  using cer::ParametersMatrix;

  std::vector<double> cars;
  cars.reserve(n * ParametersMatrix::parametersNum());

  for (size_t i = 0; i < n; ++i) {
    cars.push_back(static_cast<double>(2 * ((rand() % 25 + 15) / 250.)));
    cars.push_back(static_cast<double>(2 * ((rand() % 25 + 15) / 250.)));

    std::vector<double> points =
        std::vector<double>{0.4,  0.4,  0.80, 0.50, 1.10, 0.60, 1.20, 0.40,
                            1.21, 0.81, 1.00, 0.90, 0.70, 0.90, 0.50, 0.60};
    for (auto &p : points) {
      cars.push_back(2 * (p + (rand() % 25 - 15) / 1000.));
    }
  }

  return ParametersMatrix{cars};
}
}  // namespace

cer::evolution::Evolution::Evolution(cer::CarsPopulationData *population,
                                     int seed)
    : population_(population), seed_(seed) {
  if (seed != -1) {
    std::srand(seed_);
  } else {
    std::srand(time(nullptr));
  }
  initializeEvolutionParameters();
}

void cer::evolution::Evolution::setPopulationFitness(
    const std::vector<double> &fitness) {
  population_fitness_ = fitness;
}

void cer::evolution::Evolution::generatePopulation() {
  if (population_->empty() && first_run_) {
    population_->setCars(initialPopulation(10));
    first_run_ = false;
    return;
  }

  // population not empty so must have been loaded
  if (first_run_) {
    first_run_ = false;
    return;
  }

  if (population_fitness_.empty()) {
    population_fitness_ =
        std::vector<double>(population_->cars().carsNum(), 1.0);
  }

  auto params = population_->cars();

  math::RandomGenerator rg(seed_);
  {
    std::lock_guard<std::mutex> locker(parameters_mutex_);
    rg.setStd(parameters_["std"].value);
  }

  std::vector<double> children;
  children.reserve(params.size());

  for (size_t i = 0; i < params.carsNum(); ++i) {
    auto parents = math::tournamentSelection(population_fitness_, 2);
    std::vector<double> child;
    try {
      child = math::crossover(
          params.begin(parents.at(0)), params.end(parents.at(0)),
          params.begin(parents.at(1)), params.end(parents.at(1)));
    } catch (std::invalid_argument &e) {
      cpputils::log::critical() << "Evolution error. " << e.what();
      return;
    }

    math::mutate(child.begin(), child.end(), rg);
    children.insert(children.end(), child.begin(), child.end());
  }

  population_->setCars(ParametersMatrix(children));
}

auto cer::evolution::Evolution::parameters() const
    -> std::map<std::string, Parameter> {
  std::lock_guard<std::mutex> locker(parameters_mutex_);
  return parameters_;
}

void cer::evolution::Evolution::setParameterValue(const std::string &name,
                                                  double val) {
  std::lock_guard<std::mutex> locker(parameters_mutex_);
  if (parameters_.find(name) != parameters_.end()) {
    parameters_[name].value = val;
  }
}

void cer::evolution::Evolution::initializeEvolutionParameters() {
  std::lock_guard<std::mutex> locker(parameters_mutex_);
  parameters_ = {
      {"std", Parameter{"Odchylenie standardowe mutacji", 0.05, ""}}};
}

cer::ParametersMatrix cer::evolution::Evolution::initialPopulation(
    size_t cars_num) const {
  using cer::ParametersMatrix;

  std::vector<double> cars;
  cars.reserve(cars_num * ParametersMatrix::parametersNum());

  auto l = math::lowerLimits(ParametersMatrix::parametersNum());
  auto u = math::upperLimits(ParametersMatrix::parametersNum());

  for (size_t i = 0; i < cars_num; ++i) {
    cars.push_back(static_cast<double>((rand() % 50 + 4) / 100.));
    cars.push_back(static_cast<double>((rand() % 50 + 4) / 100.));

    int k{2};
    std::vector<double> points =
        std::vector<double>{0.41, 0.41, 0.80, 0.50, 1.10, 0.60, 1.50, 0.60,
                            1.30, 0.80, 1.00, 0.90, 0.70, 0.90, 0.50, 0.60};
    for (auto &p : points) {
      cars.push_back(
          std::fmax(l[k], std::fmin(u[k], p + (rand() % 80 - 40) / 100.)));
      ++k;
    }
  }

  return ParametersMatrix{cars};
}
