#ifndef WORLD_H
#define WORLD_H

#include <stdio.h>
#include <stdlib.h>

#include <algorithm>
#include <chrono>
#include <thread>
#include <vector>

#include "box2d/box2d.h"

#if defined(_WIN32)
#include <crtdbg.h>
#endif

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
  //~World();

  bool runSimulation();

  // do testow animacji
  bool runDummySimulation();

  std::vector<std::pair<double, double>> getRoute();
  std::vector<float> max_distance_reached();

 private:
  const CarsPopulationData &population_;
  SimulationData *simulation_data_;
  std::vector<float> max_distance_reached_;
  std::vector<Car *> cars;

  std::unique_ptr<b2World> m_world;
  std::vector<std::pair<double, double>> route;
  std::vector<Car *> generateCars();
};

}  // namespace physics
}  // namespace cer

#endif  // WORLD_H
