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
  explicit Car(b2World* m_world, const std::vector<double>& car_parameters,
               int car_num);
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
  double getCorrectionSection() const;
  double getCorrectionAngle() const;

 private:
  b2Body* m_car;
  b2Body* m_wheel1;
  std::vector<double> car_parameters_;
  int car_num_;
  bool stopped_ = 0;      // parameter for simulation management
  int iter_stopped_ = 0;  // parameter for simulation management
  float maximal_distance_reached_ = 0;
  b2Vec2 wheel1_pos_;
  double correctionSection_;
  double correctionAngle_;
};

}  // namespace physics
}  // namespace cer

#endif  // CAR_H
