#include "AppInterface.h"

#include "CarsEvolutionCore/CarsEvolutionRoot.h"
#include "cpputils/worker.h"

AppInterface::AppInterface(CarsEvolutionRoot *root) : root_(root) {
  worker_.start();
}

void AppInterface::startSimulation() {
  worker_.async([r = root_] { r->runSimulation(); });
}

QVariantList AppInterface::getPosition() {
  auto pos = root_->getPosition();
  return {pos.x, pos.y, pos.theta};
}
