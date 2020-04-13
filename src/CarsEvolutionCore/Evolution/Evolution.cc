#include "Evolution.h"

#include <cstdlib>
#include <vector>

#include "CarsPopulationData.h"

namespace {

std::vector<cer::CarParameters> dummyPopulation() {
  using cer::CarParameters;

  std::vector<CarParameters> cars_;
  cars_.reserve(10);

  for (int i = 0; i < 10; ++i) {
    auto front = static_cast<double>(rand() % 25 + 15);
    auto rear = static_cast<double>(rand() % 25 + 15);

    std::vector<double> points = std::vector<double>{
        40, 40, 120, 40, 130, 60, 110, 80, 100, 100, 80, 110, 70, 105, 60, 100};
    for (auto &p : points) {
      p += rand() % 20 - 10;
    }

    cars_.push_back(CarParameters{front, rear, points});
  }

  return cars_;
}
}  // namespace

cer::evolution::Evolution::Evolution(cer::CarsPopulationData *population)
    : population_(population) {
  srand(time(nullptr));
}

void cer::evolution::Evolution::generatePopulation() {
  population_->setCars(dummyPopulation());
}
