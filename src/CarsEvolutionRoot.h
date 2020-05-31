// module: Core
// author: Lukasz Kostrzewa, Marcin Gajewski

#ifndef CARSEVOLUTIONROOT_H
#define CARSEVOLUTIONROOT_H

#include <map>
#include <memory>
#include <vector>

namespace cer {

struct ParametersMatrix;
struct Position;

namespace evolution {
struct Parameter;
}

///
/// \brief The CarsEvolutionRoot
/// CarsEvolutionRoot owns other classes of the cars evolution library and
/// provides interface to access them.
///
class CarsEvolutionRoot {
 public:
  ///
  /// \brief CarsEvolutionRoot
  /// \param seed - seed for random numbers generation, if -1 time is used
  ///
  explicit CarsEvolutionRoot(int seed = -1);

  CarsEvolutionRoot(const CarsEvolutionRoot &) = delete;
  CarsEvolutionRoot &operator=(const CarsEvolutionRoot &) = delete;

  CarsEvolutionRoot(const CarsEvolutionRoot &&) = delete;
  CarsEvolutionRoot &operator=(const CarsEvolutionRoot &&) = delete;

  ~CarsEvolutionRoot();

  // population

  ///
  /// \brief generatePopulation creates a new population of the cars
  /// If it is called for the first time a new population is generated randomly.
  /// Otherwise evolutionary algorithm is used. The algorithm uses the vector of
  /// distances reached by each car during the last simulation as a fitness
  /// function.
  ///
  void generatePopulation();

  ///
  /// \brief cars
  /// \return matrix with cars parameters
  ///
  ParametersMatrix cars() const;

  ///
  /// \brief setCars
  /// \param cars - matrix with cars parameters
  ///
  void setCars(const ParametersMatrix &cars);

  // evolution parameters
  ///
  /// \brief parameters
  /// \return map of the parameters used in evolutionary algorihm
  ///
  std::map<std::string, evolution::Parameter> parameters() const;

  ///
  /// \brief setParameterValue
  /// \param name
  /// \param val
  /// Sets parameters used in evolutionary algorihm
  ///
  void setParameterValue(const std::string &name, double val);

  // simulation

  ///
  /// \brief runSimulation of the cars movement
  /// Creates dynamic simulation of the gars generated from the current
  /// parameters.
  ///
  void runSimulation();

  ///
  /// \brief getBestDistance
  /// \return the best distance from the last simulation
  ///
  double getBestDistance();

  ///
  /// \brief popPosition
  /// \param car_index
  /// \return the first position of the given car from positions queue.
  /// Simulation adds new positions to the queue. This method returns the first
  /// position in the queue and deletes it.
  ///
  Position popPosition(size_t car_index);

  ///
  /// \brief getRoute
  /// \return route used in the simulation
  ///
  std::vector<std::pair<double, double>> getRoute();

 private:
  struct Opaque;
  std::unique_ptr<Opaque> o_;
};
}  // namespace cer

#endif  // CARSEVOLUTIONROOT_H
