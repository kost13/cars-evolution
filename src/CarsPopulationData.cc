#include "CarsPopulationData.h"

#include <iterator>

cer::CarsPopulationData::CarsPopulationData() = default;

cer::ParametersMatrix cer::CarsPopulationData::cars() const {
  std::unique_lock<std::mutex> lock(mutex_);
  return cars_;
}

void cer::CarsPopulationData::setCars(const cer::ParametersMatrix& cars) {
  std::unique_lock<std::mutex> lock(mutex_);
  cars_ = cars;
}

bool cer::CarsPopulationData::empty() const {
  std::unique_lock<std::mutex> lock(mutex_);
  return cars_.empty();
}
