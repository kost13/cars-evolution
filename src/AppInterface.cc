#include "AppInterface.h"

#include "CarsEvolutionCore/CarsEvolutionRoot.h"
#include "CarsEvolutionCore/SimulationData.h"
#include "cpputils/logger.hpp"
#include "cpputils/worker.h"

#include <QDebug>

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
    auto pos = root_->simulationData()->popPosition(car_num);
    return {pos.x, pos.y, pos.theta};
  } catch (const std::out_of_range &e) {
    cpputils::log::critical() << "Cannot get position for car " << car_num
                              << " " << e.what();
    return {-1, -1, -1};
  }
}
