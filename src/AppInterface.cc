#include "AppInterface.h"

#include "Worker.h"
#include "core/CarsEvolutionRoot.h"

AppInterface::AppInterface(CarsEvolutionRoot *root, concurrent::Worker *worker)
    : root_(root), worker_(worker) {}

void AppInterface::startSimulation() {
  worker_->async([r = root_] { r->runSimulation(); });
}

QVariantList AppInterface::getPosition() {
  auto pos = root_->getPosition();
  return {pos.x, pos.y, pos.theta};
}
