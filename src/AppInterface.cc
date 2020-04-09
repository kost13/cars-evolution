#include "AppInterface.h"

#include "CarsEvolutionCore/CarsEvolutionRoot.h"
#include "cpputils/worker.h"

#include <QDebug>

AppInterface::AppInterface(CarsEvolutionRoot *root) : root_(root) {
  worker_.start();
}

void AppInterface::startSimulation() {
  emit simulationStarted();
  worker_.async([=] {
    root_->runSimulation();
    if (worker_.empty()) {
      emit simulationEnded();
    }
  });
}

QVariantList AppInterface::getPosition(int car_num) {
  auto pos = root_->getPosition(car_num);
  return {pos.x, pos.y, pos.theta};
}

QVariantList AppInterface::getCars() {
  auto root_cars = root_->getCars();
  QVariantList cars;
  cars.reserve(root_cars.size());

  for (const auto &c : root_cars) {
    QVariantList car;
    car.append(c.front_wheel);
    car.append(c.rear_wheel);

    auto polygon = c.body_points;
    for (auto p : polygon) {
      car.append(p);
    }

    cars.push_back(car);
  }

  return cars;
}
