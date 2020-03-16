#include "AppInterface.h"

#include "CarsEvolutionCore/CarsEvolutionRoot.h"
#include "cpputils/worker.h"

AppInterface::AppInterface(CarsEvolutionRoot *root, cpputils::Worker *worker)
    : root_(root), worker_(worker) {}

void AppInterface::startSimulation() {
  worker_->async([r = root_] { r->runSimulation(); });
}

QVariantList AppInterface::getPosition() {
  auto pos = root_->getPosition();
  return {pos.x, pos.y, pos.theta};
}
