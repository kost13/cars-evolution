// module: App
// author: Lukasz Kostrzewa

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

///
/// \brief The AppInterface class
/// It creates an interface between the GUI and the logic model of the app. It
/// uses worker object to run computations in separate thread.
///
class AppInterface : public QObject {
  Q_OBJECT
 public:
  explicit AppInterface(cer::CarsEvolutionRoot *root);

  ///
  /// \brief starts cars movement simulation in worker thread
  Q_INVOKABLE void startSimulation();

  ///
  /// \brief get position of the given car
  /// \param car_num
  /// \return list with: x, y, theta of the car
  Q_INVOKABLE QVariantList getPosition(int car_num);

  ///
  /// \brief save curent population parameters to the file
  /// \param file
  /// \return true if succeeded
  Q_INVOKABLE bool savePopulation(const QUrl &file);

  ///
  /// \brief load population parameters from the file
  /// \param file
  /// \return true if succeeded
  Q_INVOKABLE bool loadPopulation(const QUrl &file);

  ///
  /// \brief get route from the simulation
  /// \return list with x and y positions of the next route points
  Q_INVOKABLE QVariantList getRoute();

 signals:
  ///
  /// \brief simulationEnded signals the end of the cars movement simulation
  ///
  void simulationEnded();

  ///
  /// \brief simulationStarted signals the start of the cars movement simulation
  ///
  void simulationStarted();

  ///
  /// \brief newPopulationGenerated signals new populion parameters
  ///
  void newPopulationGenerated();

  ///
  /// \brief newBestCar signals a new result of the simulation
  /// \param car_index
  /// \param distance
  ///
  void newBestCar(int car_index, double distance);

 private:
  cer::CarsEvolutionRoot *root_;
  cpputils::Worker worker_;
};

#endif  // APPINTERFACE_H
