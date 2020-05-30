#include "Car.h"

#include <cpputils/logger.hpp>

namespace logger = cpputils::log;  // not used so far

cer::physics::Car::Car(std::unique_ptr<b2World>& m_world,
                       std::vector<double> car_parameters, int car_num)
    : car_parameters_(car_parameters), car_num_(car_num) {
  static const int BODY_POINTS_NUM = 8;

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
  double FrontWheelRadius = *it;
  it++;
  circle_rear.m_radius = *it;
  double RearWheelRadius_ = *it;
  it++;

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

    if (j == 0) {
      wheel1_x = static_cast<float>(x_);
      wheel1_y = static_cast<float>(y_);
    }

    if (j == 3) {
      wheel2_x = static_cast<float>(x_);
      wheel2_y = static_cast<float>(y_);
    }
  }

  chassis.Set(vertices, BODY_POINTS_NUM);

  bd.type = b2_dynamicBody;
  bd.angle = 0;

  /*position where car is created
   * - origin of a local coordinates system
   * in which car is created - body points positions
   */

  b2Vec2 p_car = {2.0f, wheel1_y + circle_rear.m_radius + 0.5f};
  bd.position.Set(p_car.x, p_car.y);
  m_car = m_world->CreateBody(&bd);

  fdc.shape = &chassis;
  fdc.density = 1.0f;

  /* filetring collisions allows to create cars which collide with
   * track but not with each other. They all have categoryBits equal
   * -belong to same category
   */

  fdc.filter.categoryBits = 0x0002;
  fdc.filter.maskBits = 0x0001;
  m_car->CreateFixture(&fdc);

  // // //wheels creation

  // wheel 1
  b2FixtureDef fd;
  fd.shape = &circle_rear;
  fd.density = 1.0f;
  fd.friction = 0.9f;

  bd.position.Set(p_car.x + wheel1_x, p_car.y + wheel1_y);
  auto m_wheel1 = m_world->CreateBody(&bd);
  fd.filter.categoryBits = 0x0002;
  fd.filter.maskBits = 0x0001;
  m_wheel1->CreateFixture(&fd);

  b2WheelJointDef jd;
  b2Vec2 axis(0.0f, 1.0f);

  // parameters for springs
  float hertz = 4.0f;
  float dampingRatio = 0.7f;
  float omega = 2.0f * b2_pi * hertz;

  jd.Initialize(m_car, m_wheel1, m_wheel1->GetPosition(), axis);
  float mass1 = m_wheel1->GetMass();
  jd.motorSpeed = -50.0f;
  jd.maxMotorTorque = -50.0f;
  jd.enableMotor = true;
  jd.stiffness = mass1 * omega * omega;
  jd.damping = 2.0f * mass1 * dampingRatio * omega;
  jd.lowerTranslation = -0.25f;
  jd.upperTranslation = 0.25f;
  jd.enableLimit = true;

  auto m_spring1 = (b2WheelJoint*)m_world->CreateJoint(&jd);

  // wheel 2
  fd.shape = &circle_rear;
  fd.density = 1.0f;
  fd.friction = 0.0f;

  fd.filter.categoryBits = 0x0002;
  fd.filter.maskBits = 0x0001;

  bd.position.Set(p_car.x + wheel2_x, p_car.y + wheel2_y);
  auto m_wheel2 = m_world->CreateBody(&bd);
  m_wheel2->CreateFixture(&fd);

  jd.Initialize(m_car, m_wheel2, m_wheel2->GetPosition(), axis);
  float mass2 = m_wheel2->GetMass();
  jd.motorSpeed = 0.0f;
  jd.maxMotorTorque = 0.0f;
  jd.enableMotor = false;
  jd.stiffness = mass2 * omega * omega;
  jd.damping = 2.0f * mass2 * dampingRatio * omega;
  jd.lowerTranslation = -0.25f;
  jd.upperTranslation = 0.25f;
  jd.enableLimit = true;

  auto m_spring2 = (b2WheelJoint*)m_world->CreateJoint(&jd);

  iter_stopped_ = 0;  // simualtion management default parameter
  stopped_ = 0;       // simualtion management default parameter

  correctionSection_ = RearWheelRadius_ + p_car.y;
  correctionAngle_ =
      tan((RearWheelRadius_ - FrontWheelRadius) / (wheel2_x - wheel1_x));

  wheel1_pos_.x = -wheel1_x;
  wheel1_pos_.y = -wheel1_y;

  // auto p = m_car->GetPosition();
  // std::cout << "initial pos " << p.x << " " << p.y << std::endl;

  m_spring1->SetMotorSpeed(-50);
}

b2Body* cer::physics::Car::getCar() const { return m_car; }
b2Vec2 cer::physics::Car::getRwheelPos() const { return wheel1_pos_; }

int cer::physics::Car::getCarNum() const { return car_num_; }
bool cer::physics::Car::getStopped() const { return stopped_; }
int cer::physics::Car::GetIterStopped() const { return iter_stopped_; }
int cer::physics::Car::getMaximalDistanceReached() const {
  return maximal_distance_reached_;
}

double cer::physics::Car::getCorrectionSection() const {
  return correctionSection_;
}
void cer::physics::Car::setStopped(bool stop) { stopped_ = stop; }
void cer::physics::Car::setIterStopped(int iter_stopped) {
  iter_stopped_ = iter_stopped;
}

void cer::physics::Car::setMaximalDistanceReached(double distance) {
  maximal_distance_reached_ = distance;
}

double cer::physics::Car::getCorrectionAngle() const {
  return correctionAngle_;
}
