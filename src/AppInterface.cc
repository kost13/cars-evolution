#include "AppInterface.h"

#include <QUrl>

#include <cpputils/worker.h>
#include <cpputils/logger.hpp>

#include "CarsEvolutionCore/CarsEvolutionRoot.h"
#include "CarsEvolutionCore/CarsPopulationData.h"
#include "CarsEvolutionCore/SimulationData.h"

#include "JsonParser.h"

AppInterface::AppInterface(cer::CarsEvolutionRoot *root) : root_(root) {
  worker_.start();
}

void AppInterface::startSimulation() {
  emit simulationStarted();
  worker_.async([=] {
    root_->generatePopulation();
    emit newPopulationGenerated();
  });

  worker_.async([=] {
    root_->runSimulation();
    if (worker_.empty()) {
      emit simulationEnded();
    }
  });
}

QVariantList AppInterface::getPosition(int car_num) {
  try {
    auto pos = root_->popPosition(car_num);
    return {pos.x, pos.y, pos.theta};
  } catch (const std::out_of_range &e) {
    cpputils::log::critical() << "Cannot get position for car " << car_num
                              << " " << e.what();
    return {-1, -1, -1};
  }
}

bool AppInterface::savePopulation(const QUrl &file) {
  return json_parser::writeParameters(root_->cars(), file.toLocalFile());
}

bool AppInterface::loadPopulation(const QUrl &file) {
  std::vector<cer::CarParameters> cars;
  bool status;
  std::tie(cars, status) = json_parser::readParameters(file.toLocalFile());
  if (status) {
    root_->setCars(cars);
    emit newPopulationGenerated();
    return true;
  }
  return false;
}
