#ifndef APPINTERFACE_H
#define APPINTERFACE_H

#include <cpputils/worker.h>

#include <QObject>
#include <QVariant>

class QUrl;

namespace cer {
class CarsEvolutionRoot;
}

class QUrl;

class AppInterface : public QObject {
  Q_OBJECT
 public:
  explicit AppInterface(cer::CarsEvolutionRoot *root);
  Q_INVOKABLE void startSimulation();
  Q_INVOKABLE QVariantList getPosition(int car_num);
  Q_INVOKABLE bool savePopulation(const QUrl &file);
  Q_INVOKABLE bool loadPopulation(const QUrl &file);
  Q_INVOKABLE QVariantList getRoute();

 signals:
  void simulationEnded();
  void simulationStarted();
  void newPopulationGenerated();

 private:
  cer::CarsEvolutionRoot *root_;
  cpputils::Worker worker_;
};

#endif  // APPINTERFACE_H
