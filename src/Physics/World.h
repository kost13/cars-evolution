#ifndef WORLD_H
#define WORLD_H

#include <vector>

#include "box2d/box2d.h"

#include "Car.h"
#include "ParametersMatrix.h"

struct Settings;

namespace cer {
class CarsPopulationData;
class SimulationData;

namespace physics {

//#########################################################

class World {
 public:
  explicit World(const CarsPopulationData &population,
                 SimulationData *simulation_data);

  bool runSimulation();

  // do testow animacji
  bool runDummySimulation();

  std::vector<std::pair<double, double>> getRoute() const;

  std::vector<float> maxDistanceReached() const;

 private:
  void generateCars();

  const CarsPopulationData &population_;
  SimulationData *simulation_data_;
  std::vector<float> max_distance_reached_;
  std::vector<Car *> cars_;
  std::unique_ptr<b2World> world_;
  std::vector<std::pair<double, double>> route_;
};

}  // namespace physics
}  // namespace cer

#endif  // WORLD_H
