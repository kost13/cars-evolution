#include "Car.h"

#include <cpputils/logger.hpp>

namespace logger = cpputils::log;  // not used so far

cer::physics::Car::Car(std::unique_ptr<b2World>& m_world,
                       std::vector<double> car_parameters, int car_num)
    : car_parameters_(car_parameters), car_num_(car_num) {
  // // // constant parameters for each car
  static const int BODY_POINTS_NUM = 8;

  // front wheel
  int wheel1_point_no = 3;  // to which point of body, wheel1 is attached
  float wheel1_density = 1.0f;
  float wheel1_friction = 0.9f;
  float motor1_speed = 0.0f;      //-1.0f;
  float motor1_maxTorque = 0.0f;  //-200.0f;
  bool motor1_enable = false;
  float wheel1_dampingRatio = 0.7f;

  // rear wheel
  int wheel2_point_no = 0;  // to which point of body, wheel2 is attached
  float wheel2_density = 1.0f;
  float wheel2_friction = 0.9f;
  float motor2_speed = -0.0f;
  float motor2_maxTorque = 0.0f;  //-15.0f;
  bool motor2_enable = false;
  float wheel2_dampingRatio = 0.7f;

  b2PolygonShape chassis;
  b2Vec2 vertices[BODY_POINTS_NUM];
  b2CircleShape circle_front;
  b2CircleShape circle_rear;
  b2BodyDef bd;
  b2FixtureDef fdc;

  // // // // inserting parameters
  auto it = car_parameters_.begin();

  // radiuses of wheels are saved

  circle_front.m_radius = *it;
  it++;
  circle_rear.m_radius = *it;
  it++;

  // std::cout << "r1: " << circle_front.m_radius << "r2: " <<
  // circle_rear.m_radius
  //        << std::endl;

  // body point coordinates
  double x_;
  double y_;

  // coordinates of wheels are set later
  float wheel1_x = 0;
  float wheel1_y = 0;

  float wheel2_x = 0;
  float wheel2_y = 0;

  // shape of a car

  int j;
  for (it = car_parameters_.begin() + 2, j = 0;
       it != car_parameters_.end() - 1 && j < BODY_POINTS_NUM; ++it, ++j) {
    x_ = *it;
    ++it;
    y_ = *it;

    logger::info() << x_ << " " << y_;
    vertices[j].Set(x_, y_);

    /*there is no method for getting coordinates from vertices
  so it is better to save needed ones in later stages
  (wheels coordinates)*/

    if (j == wheel1_point_no) {
      wheel1_x = static_cast<float>(x_);
      wheel1_y = static_cast<float>(y_);
    }

    if (j == wheel2_point_no) {
      wheel2_x = static_cast<float>(x_);
      wheel2_y = static_cast<float>(y_);
    }
  }

  chassis.Set(vertices, BODY_POINTS_NUM);

  bd.type = b2_dynamicBody;
  /*position where car is created
   * - origin of a local coordinates system
   * in which car is created - body points positions
   */

  bd.angle = 0;
  b2Vec2 p_car = {2.0f, wheel1_y + circle_rear.m_radius + 0.5f};
  bd.position.Set(p_car.x, p_car.y);
  m_car = m_world->CreateBody(&bd);

  m_car->CreateFixture(&chassis, 1.0f);

  /* filetring collisions allows to create cars which collide with
   * track but not with each other. They all have categoryBits equal
   * -belong to same category
   */

  //.filter.categoryBits = 0x0002;

  // fdc.filter.maskBits = 0x0001; //not needed, they collide with everything
  // else

  // // //wheels creation

  // wheel 1
  b2FixtureDef fd;
  fd.shape = &circle_front;
  fd.density = wheel1_density;
  fd.friction = wheel1_friction;

  bd.position.Set(p_car.x + wheel1_x, p_car.y + wheel1_y);
  auto m_wheel1 = m_world->CreateBody(&bd);
  fd.filter.categoryBits = 0x0002;
  m_wheel1->CreateFixture(&fd);

  b2WheelJointDef jd;
  b2Vec2 axis(0, 1);

  // parameters for springs, then take to settings
  // it was added last time, as before the springs were working other way
  float hertz = 4.0f;
  float dampingRatio = 0.7f;
  float omega = 2.0f * b2_pi * hertz;

  jd.Initialize(m_car, m_wheel1, m_wheel1->GetPosition(), axis);
  float mass1 = m_wheel1->GetMass();
  jd.motorSpeed = motor1_speed;
  jd.maxMotorTorque = motor1_maxTorque;
  jd.enableMotor = motor1_enable;
  //       car_t.jd.frequencyHz = settings.motor1_frequencyHz;
  // jd.damping = wheel1_dampingRatio;
  jd.stiffness = mass1 * omega * omega;
  jd.damping = 2.0f * mass1 * dampingRatio * omega;
  jd.lowerTranslation = -0.25f;
  jd.upperTranslation = 0.25f;
  jd.enableLimit = true;

  auto m_spring1 = (b2WheelJoint*)m_world->CreateJoint(&jd);

  // wheel 2
  // m_car->CreateFixture(&car_t.chassis, 1.0f);
  fd.shape = &circle_rear;
  fd.density = wheel2_density;
  fd.friction = wheel2_friction;

  fd.filter.categoryBits = 0x0002;
  // fdc.filter.maskBits = 0x0001;

  bd.position.Set(p_car.x + wheel2_x, p_car.y + wheel2_y);
  auto m_wheel2 = m_world->CreateBody(&bd);
  m_wheel2->CreateFixture(&fd);

  jd.Initialize(m_car, m_wheel2, m_wheel2->GetPosition(), axis);
  float mass2 = m_wheel2->GetMass();
  jd.motorSpeed = motor2_speed;
  jd.maxMotorTorque = motor2_maxTorque;
  jd.enableMotor = motor2_enable;
  //        car_t.jd.frequencyHz = settings.motor2_frequencyHz;
  // jd.damping = settings.wheel2_dampingRatio;
  jd.stiffness = mass2 * omega * omega;
  jd.damping = 2.0f * mass2 * dampingRatio * omega;
  jd.lowerTranslation = -0.25f;
  jd.upperTranslation = 0.25f;
  jd.enableLimit = true;

  auto m_spring2 = (b2WheelJoint*)m_world->CreateJoint(&jd);

  // m_spring1->SetMotorSpeed(50);
  // m_spring2->SetMotorSpeed(m_speed);

  iter_stopped_ = 0;  // simualtion management default parameter
  stopped_ = 0;       // simualtion management default parameter

  // printf("%d \n", car_t.car_num);

  // calculating Center of mass of each car relatively to rear wheel
  /*b2Vec2 mass_center = (m_car->GetLocalCenter());
  car_t.CoM_position.x = 0;
  car_t.CoM_position.y = 0;
  std::cout << -mass_center.x << std::endl;
  std::cout << -mass_center.y << std::endl;
  car_t.CoM_position.x = -mass_center.x - wheel2_x;
  car_t.CoM_position.y = -mass_center.y - wheel2_y;

  std::cout << car_t.CoM_position.x << std::endl;
  std::cout << car_t.CoM_position.y << std::endl;
  */

  CoM_position.x = -wheel1_x;
  CoM_position.y = -wheel1_y;

  wheel2_pos_.x = wheel2_x;
  wheel2_pos_.y = wheel2_y;

  // auto p = m_car->GetPosition();
  // std::cout << "initial pos " << p.x << " " << p.y << std::endl;

  m_spring1->SetMotorSpeed(-60);
}

b2Body* cer::physics::Car::getCar() const { return m_car; }
b2Vec2 cer::physics::Car::getRwheelPos() const { return wheel2_pos_; }

int cer::physics::Car::getCarNum() const { return car_num_; }
bool cer::physics::Car::getStopped() const { return stopped_; }
int cer::physics::Car::GetIterStopped() const { return iter_stopped_; }
int cer::physics::Car::getMaximalDistanceReached() const {
  return maximal_distance_reached_;
}

void cer::physics::Car::setStopped(bool stop) { stopped_ = stop; }
void cer::physics::Car::setIterStopped(int iter_stopped) {
  iter_stopped_ = iter_stopped;
}

void cer::physics::Car::setMaximalDistanceReached(double distance) {
  maximal_distance_reached_ = distance;
}
