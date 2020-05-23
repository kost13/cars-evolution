#include "World.h"

#include <cpputils/logger.hpp>

#include "CarsPopulationData.h"
#include "SimulationData.h"

namespace logger = cpputils::log;  // not used so far

// constructor
cer::physics::World::World(const cer::CarsPopulationData& population,
                           cer::SimulationData* simulation_data)
    : population_(population), simulation_data_(simulation_data) {
  // the scene is initiated
  b2Vec2 gravity;
  gravity.Set(settings.gravity_x, settings.gravity_y);
  m_world = std::make_unique<b2World>(gravity);

  // b2BodyDef bodyDef;
  // m_groundBody = m_world->CreateBody(&bodyDef);

  b2Body* ground = nullptr;
  {
    b2BodyDef bd;
    bd.type = b2_staticBody;
    ground = m_world->CreateBody(&bd);

    b2EdgeShape shape;

    b2FixtureDef fd;
    fd.shape = &shape;
    fd.density = settings.ground_density;
    fd.friction = settings.ground_friction;
    /*fd.filter.categoryBits = 0x0001;
    fd.filter.maskBits = 0x0002;
    */

    const int size = 20;  // settings.number_of_stages;
    const float dx = settings.stage_width_x;

    // start is at flat plate, size 4*dx, on the ground level
    shape.Set(b2Vec2(-2 * dx, 0.0f), b2Vec2(2 * dx, 0.0f));
    ground->CreateFixture(&fd);

    float x = 2 * dx;
    float y1 = 0.0f;

    // można potem zamienić na wektor, jak zdecydujemy
    // gdzie to przechowujemy

    float hs[size] = {1.0f, 3.0f,   4.0f,  -1.0f, -2.0f,  1.0f, 2.0f,
                      5.0f, -1.25f, 0.0f,  1.0f,  1.0f,   4.0f, 0.0f,
                      0.0f, -1.0f,  -2.0f, -5.0f, -5.25f, 0.0f};

    // float hs[size];

    for (unsigned int i = 0; i < size; ++i) {
      // h[i]=RandomFloat(-5.0f,5.0f)
      float y2 = hs[i];
      shape.Set(b2Vec2(x, y1), b2Vec2(x + dx, y2));
      ground->CreateFixture(&fd);
      y1 = y2;
      x += dx;
    }

    // wall at the end
    // remove if track could be extended
    x += dx;
    shape.Set(b2Vec2(x, 0.0f), b2Vec2(x, 2 * dx));
    ground->CreateFixture(&fd);

    route.emplace_back(-2 * dx, 0);
    route.emplace_back(2 * dx, 0);

    double x_ = 2 * dx;
    double y_;
    // saving road
    for (int k = 0; k < settings.number_of_stages; k++) {
      x_ += dx;
      y_ = hs[k];
      route.emplace_back(x_, y_);
    }

    x_ += dx;
    route.emplace_back(x_, 2 * dx);

    for (int k = 0; k < 23; k++) {
      std::cout << "x: " << route[k].first << std::endl
                << "y: " << route[k].second << std::endl;
    }
  }
}

std::vector<std::pair<double, double>> cer::physics::World::getRoute() {
  return route;
}

std::vector<b2Body*> cer::physics::World::generateCars() {
  // number of cars in simulation
  static const size_t car_No = 1;  // population_.cars().carsNum();
  // static const int BODY_POINTS_NUM =
  // (population_.cars().parametersNum()-2)/2;
  static const int BODY_POINTS_NUM = 8;

  std::cout << "Body points:" << BODY_POINTS_NUM << std::endl;
  // vector of cars
  std::vector<b2Body*> cars;
  auto parametersMatrix = population_.cars();

  std::vector<double>::iterator it;

  // for each car
  for (size_t i = 0; i < car_No; i++) {
    b2Body* m_car;
    b2Body* m_wheel1;
    b2Body* m_wheel2;
    b2WheelJoint* m_spring1;
    b2WheelJoint* m_spring2;

    b2PolygonShape chassis;
    b2Vec2 vertices[BODY_POINTS_NUM];
    b2CircleShape circle_front;
    b2CircleShape circle_rear;
    b2BodyDef bd;
    b2FixtureDef fdc;
    b2WheelJointDef jd;

    Car car_t;  // temporary object

    car_t.car_num = i;

    // radiuses of wheels are saved
    it = parametersMatrix.begin(i);
    circle_front.m_radius = *it;
    it++;
    circle_rear.m_radius = *it;
    it++;

    std::cout << "r1: " << circle_front.m_radius
              << "r2: " << circle_rear.m_radius << std::endl;

    // body point coordinates
    double x_;
    double y_;

    // coordinates of wheels are set later
    double wheel1_x = 0;
    double wheel1_y = 0;

    double wheel2_x = 0;
    double wheel2_y = 0;

    // shape of a car

    int j;
    for (it = parametersMatrix.begin(i) + 2, j = 0;
         it != parametersMatrix.end(i) - 1 && j < BODY_POINTS_NUM; ++it, ++j) {
      x_ = *it;  //+10.0f;
      ++it;
      y_ = *it;  //+25.0f;
      logger::info() << x_ << " " << y_;
      std::cout << "x: " << x_ << "y: " << y_ << std::endl;
      vertices[j].Set(x_, y_);

      /*there is no method for getting coordinates from vertices
      so it is better to save them*/
      if (j == settings.wheel1_point_no) {
        wheel1_x = x_;
        wheel1_y = y_;
      }

      if (j == settings.wheel2_point_no) {
        wheel2_x = x_;
        wheel2_y = y_;
      }
    }

    chassis.Set(vertices, BODY_POINTS_NUM);

    // jezeli koło nie bedzie sie krecic to ten blok skopiowac
    // do wheel 2

    bd.type = b2_dynamicBody;
    bd.position.Set(settings.starting_position_x, settings.starting_position_y);
    m_car = m_world->CreateBody(&bd);

    m_car->CreateFixture(&chassis, 1.0f);
    fdc.filter.categoryBits = 0x0002;  // jak przypisać maski itd.
    fdc.filter.maskBits = 0x0001;

    b2Vec2 axis(settings.starting_position_x, settings.starting_position_y);

    // wheel 1

    fdc.shape = &circle_front;
    fdc.density = settings.wheel1_density;
    fdc.friction = settings.wheel1_friction;

    bd.position.Set(wheel1_x, wheel1_y);  // wheel position
    m_wheel1 = m_world->CreateBody(&bd);
    fdc.filter.categoryBits = 0x0002;
    fdc.filter.maskBits = 0x0001;
    m_wheel1->CreateFixture(&fdc);

    jd.Initialize(m_car, m_wheel1, m_wheel1->GetPosition(), axis);
    jd.motorSpeed = settings.motor1_speed;
    jd.maxMotorTorque = settings.motor1_maxTorque;
    jd.enableMotor = settings.motor1_enable;
    //       car_t.jd.frequencyHz = settings.motor1_frequencyHz;
    jd.damping = settings.wheel1_dampingRatio;
    m_spring1 = (b2WheelJoint*)m_world->CreateJoint(&jd);

    // wheel 2
    // m_car->CreateFixture(&car_t.chassis, 1.0f);
    fdc.shape = &circle_rear;
    fdc.density = settings.wheel2_density;
    fdc.friction = settings.wheel2_friction;

    fdc.filter.categoryBits = 0x0002;
    fdc.filter.maskBits = 0x0001;

    bd.position.Set(wheel2_x, wheel2_y);  // wheel position
    m_wheel2 = m_world->CreateBody(&bd);
    m_wheel2->CreateFixture(&fdc);

    jd.Initialize(m_car, m_wheel2, m_wheel2->GetPosition(), axis);
    jd.motorSpeed = settings.motor2_speed;
    jd.maxMotorTorque = settings.motor2_maxTorque;
    jd.enableMotor = settings.motor2_enable;
    //        car_t.jd.frequencyHz = settings.motor2_frequencyHz;
    jd.damping = settings.wheel2_dampingRatio;
    m_spring2 = (b2WheelJoint*)m_world->CreateJoint(&jd);

    car_t.iter_stopped = 0;  // simualtion management default parameter
    car_t.stopped = 0;       // simualtion management default parameter
    // car_t.mass_center=

    printf("%d \n", car_t.car_num);

    cars.push_back(m_car);
  }

  return cars;
}

bool cer::physics::World::runSimulation() {
  auto cars_num = population_.cars().carsNum();
  simulation_data_->reset(cars_num);
  /* for dummy simulation
  auto cars_num = population_.cars().carsNum();
  simulation_data_->reset(cars_num);
  */

  /*
  // dummy simulation
  std::vector<float> speeds(cars_num);
  for (auto &s : speeds) {
    s = float(rand() % 10);
  }
  */

#if defined(_WIN32)
  // Enable memory-leak reports
  _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG));
#endif

  float timeStep =
      settings.m_hertz > 0.0f ? 1.0f / settings.m_hertz : float(0.0f);

  // m_world->SetAllowSleeping(settings.m_enableSleep);
  m_world->SetWarmStarting(settings.m_enableWarmStarting);
  m_world->SetContinuousPhysics(settings.m_enableContinuous);
  m_world->SetSubStepping(settings.m_enableSubStepping);

  // creating physical car objects
  //'population_' was assigned in constructor
  std::vector<b2Body*> cars = generateCars();
  bool stop = 0;  // flag for simulation control
  int iter = 0;

  std::vector<b2Body*>::iterator it;

  b2Vec2 position;
  b2Vec2 last_position;
  b2Vec2 diff_position;  // for b2vec2 only -= operator is defined
  last_position.SetZero();
  float angle;
  float mass_x;
  float mass_y;
  // int cars_not_moving;

  std::vector<Car> cars_struct;

  for (size_t i = 0; i < cars.size(); i++) {
    Car car(i);
    cars_struct.push_back(car);
  }
  std::vector<Car>::iterator it2;

  b2Vec2 axis(settings.starting_position_x, settings.starting_position_y);
  for (it = cars.begin(), it2 = cars_struct.begin();
       it != /*cars.begin()+1*/ cars.end(); ++it, ++it2) {
    it2->mass_center = ((*it)->GetLocalCenter());
    printf("Nr: %d    (%4.2f, %4.2f)= (%4.2f, %4.2f)-(%4.2f, %4.2f)\n",
           it2->car_num, (it2->mass_center).x, (it2->mass_center).y,
           ((*it)->GetLocalCenter()).x, ((*it)->GetLocalCenter()).y, axis.x,
           axis.y);
  }

  while (!stop && iter < settings.sim_max_iter) {
    // simulate computations
    //    logger::info() << "runSimulation" << j;

    /*for each car
     *jeżeli tak zdecydujemy, to można nie liczyć dla samochodów
     *które mają status zatrzymanych*/

    // pamietac, poprawic to +1 te powyzej
    for (it = cars.begin(), it2 = cars_struct.begin();
         it != /*cars.begin()+1*/ cars.end(); ++it, ++it2) {
      m_world->Step(timeStep, settings.m_velocityIterations,
                    settings.m_positionIterations);

      position = (*it)->GetPosition();
      angle = (*it)->GetAngle();
      mass_x = (*it2).mass_center.x;
      mass_y = (*it2).mass_center.y;

      Position position_ = {position.x - mass_x, position.y - mass_y, angle};
      // printf("%d\n",iter/10);
      // if(iter==0){
      printf("%d %4.2f %4.2f %4.2f\n", it2->car_num, position_.x, position_.y,
             position_.theta);
      printf("%d %4.2f %4.2f %4.2f\n", it2->car_num, position_.x + mass_x,
             position_.y + mass_y, position_.theta);
      //}

      simulation_data_->pushPosition(it2->car_num, position_);

      /* checks if car is moving in our definition and
       * if can be counted as stopped.
       * Definition: if length of position shift vector for the car
       * is smaller than 'settings.minimumLength_of_vector',
       * the car is not moving. If it is not moving in
       * 'settings.max_car_iter' iterations, it is counted as stopped.
       *
       */
      diff_position = position;
      diff_position -= last_position;  // for b2vec2 only -= operator is defined

      if (diff_position.Length() < settings.minimumLength_of_vector)
        (it2->iter_stopped)++;
      else
        it2->iter_stopped = 0;

      position = last_position;

      if (it2->iter_stopped > settings.max_car_iter)  // flaga up
        it2->stopped = 1;

      iter++;

    }  // for each car

    // Checks if any car is moving.
    // If not, 'stop' will be left with value 1 and simualtion stopped.

    stop = 1;
    for (it2 = cars_struct.begin(); it2 != cars_struct.end(); it2++)
      if (!it2->stopped) {
        stop = 0;
        break;
      }

  }  // simulation loop 'while'

  return true;
}

bool cer::physics::World::runDummySimulation() {
  auto cars_num = population_.cars().carsNum();
  simulation_data_->reset(cars_num);

  std::vector<float> speeds(cars_num);
  for (auto& s : speeds) {
    s = float(rand() % 10);
  }
  for (int j = 0; j < 200; ++j) {
    {
      for (size_t i = 0; i < cars_num; ++i) {
        simulation_data_->pushPosition(
            i, Position{(0.05f * speeds[i] + 1.0f) * j, 0.5,
                        speeds[i] * 0.1f * j});
      }
    }
  }
  return true;
}
