#ifndef CAR_H
#define CAR_H

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
  explicit Car(b2World* world, const std::vector<double>& car_parameters,
               int car_num);
  Car(const Car&) = delete;
  Car& operator=(const Car&) = delete;
  Car(Car&&) = default;
  Car& operator=(Car&&) = default;
  ~Car() = default;

  void deleteFromWorld();

  float getAngle() const;
  b2Vec2 getRearWheelPos() const;
  int getCarNum() const;
  bool getStopped() const;
  int GetIterStopped() const;
  float getMaximalDistanceReached() const;
  void setStopped(bool stop);
  void setIterStopped(int iter_stopped);
  void setMaximalDistanceReached(double distance);
  double getCorrectionSection() const;
  double getCorrectionAngle() const;

 private:
  b2World* world_;
  b2Body* car_{nullptr};
  b2Body* wheel_rear_{nullptr};
  b2WheelJoint* wheel_rear_joint_{nullptr};
  b2Body* wheel_front_{nullptr};
  b2WheelJoint* wheel_front_joint_{nullptr};
  int car_num_{};
  bool stopped_ = false;  // parameter for simulation management
  int iter_stopped_ = 0;  // parameter for simulation management
  float maximal_distance_reached_ = 0;
  double correctionSection_{};
  double correctionAngle_{};
};

}  // namespace physics
}  // namespace cer

#endif  // CAR_H
