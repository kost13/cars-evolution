#ifndef WORLD_H
#define WORLD_H

//#include <box2d/b2_body.h>


//dodałem biblioteczki, sprawdzic co to crt
#include <algorithm>
#include <stdio.h>
#include <thread>
#include <chrono>
#include "box2d/box2d.h"
#include <stdlib.h>
#include <vector>


#if defined(_WIN32)
#include <crtdbg.h>
#endif

struct Settings;


namespace cer {
class CarsPopulationData;
class SimulationData;

namespace physics {



const int RAND_LIMIT=32767;

/// Random floating point number in range [lo, hi]
inline float randomFloat(float lo, float hi)
{
    auto r = static_cast<float>(rand() & RAND_LIMIT);
    r /= RAND_LIMIT;
    r = (hi - lo) * r + lo;
    return r;
}



const int32 k_maxContactPoints = 2048;
static const int BODY_POINTS_NUM = 8;


//      #########################
struct ContactPoint
{
    b2Fixture* fixtureA;
    b2Fixture* fixtureB;
    b2Vec2 normal;
    b2Vec2 position;
    b2PointState state;
    float normalImpulse;
    float tangentImpulse;
    float separation;
};


//##################



//      #######################################
struct Car {
  Car() = default;

  b2PolygonShape chassis;
  b2Vec2 vertices[BODY_POINTS_NUM];
  b2CircleShape circle;
  b2CircleShape circle2;
  b2BodyDef bd;
  b2FixtureDef fd;
  b2WheelJointDef jd;


};


//##################################

class World {
 public:
  explicit World(const CarsPopulationData &population,
                 SimulationData *simulation_data);
  ~World()=default;

  bool runSimulation();

  void Step(/*Settings& settings*/);
  void ShiftOrigin(const b2Vec2& newOrigin);
  void generateCars(const cer::CarsPopulationData &population,
                                         cer::SimulationData *simulation_data);



  float vecNorm() const;  // temp




 private:
  const CarsPopulationData &population_;
  SimulationData *simulation_data_;
  b2Vec2 vec_;

  //        ###################################
  b2Body* m_groundBody;
  b2AABB m_worldAABB;
  ContactPoint m_points[k_maxContactPoints];
  int32 m_pointCount;
  std::unique_ptr<b2World> m_world;
  int32 m_stepCount;
  b2Profile m_maxProfile;
  b2Profile m_totalProfile;

  b2Body* m_car;
  b2Body* m_wheel1;
  b2Body* m_wheel2;

  float m_hz;
  float m_zeta;
  float m_speed;
  b2WheelJoint* m_spring1;
  b2WheelJoint* m_spring2;

  //###################################


};
}  // namespace physics
}  // namespace cer

#endif  // WORLD_H
