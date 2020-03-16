#ifndef APPINTERFACE_H
#define APPINTERFACE_H

#include <QObject>
#include <QVariant>

class CarsEvolutionRoot;

namespace cpputils {
class Worker;
}

class AppInterface : public QObject {
  Q_OBJECT
 public:
  AppInterface(CarsEvolutionRoot *root, cpputils::Worker *worker);
  Q_INVOKABLE void startSimulation();
  Q_INVOKABLE QVariantList getPosition();

 private:
  CarsEvolutionRoot *root_;
  cpputils::Worker *worker_;
};

#endif  // APPINTERFACE_H
