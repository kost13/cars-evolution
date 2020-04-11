#ifndef APPINTERFACE_H
#define APPINTERFACE_H

#include <cpputils/worker.h>

#include <QObject>
#include <QVariant>

namespace cer {
class CarsEvolutionRoot;
}

class AppInterface : public QObject {
  Q_OBJECT
 public:
  explicit AppInterface(cer::CarsEvolutionRoot *root);
  Q_INVOKABLE void startSimulation();
  Q_INVOKABLE QVariantList getPosition(int car_num);

 signals:
  void simulationEnded();
  void simulationStarted();

 private:
  cer::CarsEvolutionRoot *root_;
  cpputils::Worker worker_;
};

#endif  // APPINTERFACE_H
