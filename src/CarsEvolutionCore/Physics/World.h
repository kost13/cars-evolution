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

#include "ParametersMatrix.h"
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
  b2CircleShape circle_front;
  b2CircleShape circle_rear;
  b2BodyDef bd;
  b2FixtureDef fd;
  b2WheelJointDef jd;


};


//##################################



struct Settings{

    //simualation and world
    float m_hertz= 60.0f;
    int m_velocityIterations=8;
    int m_positionIterations=3;
    bool m_enableWarmStarting=true;
    bool m_enableContinuous=true;
    bool m_enableSubStepping=false;


    //scene
    float ground_friction=0.6f;
    float ground_density=0.0f; //recommended!
    int number_of_stages=20;   /* jak zdecydujemy się na jakiś sposób
                                przechowywania to zmienić w cpp przypisanie
                                chwilowo jest stała macierz    */
    float stage_width_x=10.0f;



    //cars


        //wheel1
        int wheel1_point_no=1; //to which point of body, wheel1 is attached
        float wheel1_density=1.0f;
        float wheel1_friction=0.9f;
        float motor1_speed=1.0f;
        float motor1_maxTorque=20.0f;
        bool  motor1_enable=true;
        float motor1_frequencyHz=4.0f;
        float wheel1_dampingRatio=0.7f;



        //wheel2
        int wheel2_point_no=4; //to which point of body, wheel2 is attached
        float wheel2_density=1.0f;
        float wheel2_friction=0.9f;
        float motor2_speed=1.0f;
        float motor2_maxTorque=20.0f;
        bool  motor2_enable=true;
        float motor2_frequencyHz=4.0f;
        float wheel2_dampingRatio=0.7f;







};

class World {
 public:
  explicit World(const CarsPopulationData &population,
                 SimulationData *simulation_data);
  ~World()=default;

  bool runSimulation(std::vector<cer::physics::Car> cars);

  void Step(/*Settings& settings*/);
  void ShiftOrigin(const b2Vec2& newOrigin);
  std::vector<cer::physics::Car> generateCars(const cer::CarsPopulationData &population,
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


  b2WheelJoint* m_spring1;
  b2WheelJoint* m_spring2;

  Settings settings;
  //###################################


};
}  // namespace physics
}  // namespace cer

#endif  // WORLD_H
