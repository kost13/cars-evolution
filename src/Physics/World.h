// module: Core.Physics
// author: Marcin Gajewski

#ifndef WORLD_H
#define WORLD_H

#include <mutex>
#include <vector>

#include "Car.h"
#include "ParametersMatrix.h"
#include "box2d/box2d.h"

struct Settings;

namespace cer {
class CarsPopulationData;
class SimulationData;

namespace physics {

///
/// \brief World
/// \param population - parameters of the cars
/// \param simulation_data - calculated cars positions
///
class World {
 public:
  explicit World(const CarsPopulationData &population,
                 SimulationData *simulation_data);
  World(const World &) = delete;
  World &operator=(const World &) = delete;
  World(World &&) = delete;
  World &operator=(World &&) = delete;
  ~World() = default;

  ///
  /// \brief runSimulation
  /// \return execution status
  /// method performing physics simulation
  bool runSimulation();

  ///
  /// \brief getRoute
  /// \return vector of points creating the track
  ///
  std::vector<std::pair<double, double>> getRoute() const;

  ///
  /// \brief maxDistanceReached
  /// \return vector of maximal distances reached by each car
  ///
  std::vector<double> maxDistanceReached() const;

 private:
  void generateCars();

  const CarsPopulationData &population_;
  SimulationData *simulation_data_;
  std::vector<float> max_distance_reached_;
  std::vector<std::unique_ptr<Car>> cars_;
  std::unique_ptr<b2World> world_;
  std::vector<std::pair<double, double>> route_;
  mutable std::mutex route_mutex_;
};

}  // namespace physics
}  // namespace cer

#endif  // WORLD_H
