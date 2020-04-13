#ifndef WORLD_H
#define WORLD_H

#include "box2d/b2_body.h"

namespace cer {
class CarsPopulationData;
class SimulationData;

namespace physics {

class World {
 public:
  explicit World(const CarsPopulationData &population,
                 SimulationData *simulation_data);

  bool runSimulation();

  float vecNorm() const;  // temp

 private:
  const CarsPopulationData &population_;
  SimulationData *simulation_data_;
  b2Vec2 vec_;
};
}  // namespace physics
}  // namespace cer

#endif  // WORLD_H
