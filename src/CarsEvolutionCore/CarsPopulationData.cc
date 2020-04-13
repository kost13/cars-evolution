#include "CarsPopulationData.h"

cer::CarsPopulationData::CarsPopulationData() = default;

std::vector<cer::CarParameters> cer::CarsPopulationData::cars() const {
  std::unique_lock<std::mutex> lock(mutex_);
  return cars_;
}

std::vector<double> cer::CarsPopulationData::carsVector() const {
  std::vector<double> cars;
  cars.reserve((2 * CarParameters::BODY_POINTS_NUM + 2) * cars_.size());

  std::unique_lock<std::mutex> lock(mutex_);
  for (const auto& c : cars_) {
    cars.push_back(c.front_wheel);
    cars.push_back(c.rear_wheel);
    std::copy(c.body_points.cbegin(), c.body_points.cend(),
              std::back_inserter(cars));
  }

  return cars;
}

void cer::CarsPopulationData::setCars(
    const std::vector<cer::CarParameters>& cars) {
  std::unique_lock<std::mutex> lock(mutex_);
  cars_ = cars;
}

cer::CarParameters cer::CarsPopulationData::car(size_t ind) {
  std::unique_lock<std::mutex> lock(mutex_);
  return cars_[ind];
}
