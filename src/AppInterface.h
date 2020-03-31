#ifndef APPINTERFACE_H
#define APPINTERFACE_H

#include <cpputils/worker.h>

#include <QObject>
#include <QVariant>

class CarsEvolutionRoot;

class AppInterface : public QObject {
  Q_OBJECT
 public:
  AppInterface(CarsEvolutionRoot *root);
  Q_INVOKABLE void startSimulation();
  Q_INVOKABLE QVariantList getPosition(int car_num);
  Q_INVOKABLE QVariantList getCars();

 private:
  CarsEvolutionRoot *root_;
  cpputils::Worker worker_;
};

#endif  // APPINTERFACE_H
