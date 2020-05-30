#ifndef CAR_H
#define CAR_H

#include <stdio.h>
#include <stdlib.h>

#include <memory>
#include <vector>

#include "ParametersMatrix.h"
#include "box2d/box2d.h"

namespace cer {
class CarsPopulationData;
class SimulationData;

namespace physics {

class Car {
 public:
  Car() = default;
  explicit Car(std::unique_ptr<b2World>& m_world,
               std::vector<double> car_parameters, int car_num);
  ~Car();

  b2Body* getCar() const;
  b2Vec2 getRwheelPos() const;
  int getCarNum() const;
  bool getStopped() const;
  int GetIterStopped() const;
  int getMaximalDistanceReached() const;
  void setStopped(bool stop);
  void setIterStopped(int iter_stopped);
  void setMaximalDistanceReached(double distance);

 private:
  b2Body* m_car;
  std::vector<double> car_parameters_;
  int car_num_;
  bool stopped_ = 0;      // parameter for simulation management
  int iter_stopped_ = 0;  // parameter for simulation management
  float maximal_distance_reached_ = 0;
  b2Vec2 CoM_position;  // center of mass of car relatively to rear wheel
  b2Vec2 wheel2_pos_;
};

}  // namespace physics
}  // namespace cer

#endif  // CAR_H
