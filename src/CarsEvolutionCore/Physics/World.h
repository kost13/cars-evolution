#ifndef WORLD_H
#define WORLD_H


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


//##############################################################


//structure storing parameters for physics simulation
struct Settings{

    //simualation and world
    float m_hertz= 60.0f;       //simualtion time step every 1/60s
    int sim_max_iter= 3600*10;  //1s = 60 iterations, 60s = 3600 iterations, 10 cars
    int m_velocityIterations=8;
    int m_positionIterations=3;
    float minimumLength_of_vector=5;    /*if length of car position shift
                                        vector is smaller than this
                                        value, car is counted as
                                        not moving*/

    int max_car_iter=1000;              /*if car is not moving in
                                        'max_car_iter' it is counted
                                        as stopped -> flag 'stopped'
                                        in 'Car' struture is changed to 1.
                                        */

    bool m_enableWarmStarting=true;
    bool m_enableContinuous=true;
    bool m_enableSubStepping=false;

    //######################################################
    //scene


    float gravity_x=0.0f;
    float gravity_y=-10.0f;

    float ground_friction=0.6f;
    float ground_density=0.0f; //recommended!
    int number_of_stages=20;   /* jak zdecydujemy się na jakiś sposób
                                przechowywania to zmienić w cpp przypisanie
                                chwilowo jest stała macierz    */
    float stage_width_x=10.0f;


    //######################################################
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

//jeżeli chcemy statycznie 'vertices', to nie ma jak tego inaczej zrobić
//fajnie by to było mieć w parametrach
static const int BODY_POINTS_NUM = 8;

//#########################################################

struct Car {
  Car() = default;

  int car_num;
  b2PolygonShape chassis;
  b2Vec2 vertices[BODY_POINTS_NUM];
  b2CircleShape circle_front;
  b2CircleShape circle_rear;
  b2BodyDef bd;
  b2FixtureDef fdc;
  b2WheelJointDef jd;
  bool stopped;          //parameter for simulation management
  int iter_stopped;     //parameter for simulation management
};


class World {
  public:
  explicit World(const CarsPopulationData &population,
                 SimulationData *simulation_data); 
  ~World()=default;

  bool runSimulation();

  private:
  const CarsPopulationData &population_;
  SimulationData *simulation_data_;
  Settings settings;

  std::unique_ptr<b2World> m_world;

  b2Body* m_car;
  b2Body* m_wheel1;
  b2Body* m_wheel2;
  b2WheelJoint* m_spring1;
  b2WheelJoint* m_spring2;

  std::vector<cer::physics::Car> generateCars();

};


}  // namespace physics
}  // namespace cer

#endif  // WORLD_H
