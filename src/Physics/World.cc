#include "World.h"

#include <cpputils/logger.hpp>

#include "CarsPopulationData.h"
#include "SimulationData.h"

namespace logger = cpputils::log;  // not used so far

// constructor, here is initialized physics engine and created track.
cer::physics::World::World(const cer::CarsPopulationData& population,
                           cer::SimulationData* simulation_data)
    : population_(population), simulation_data_(simulation_data) {
  // the scene is initiated with gravity vector
  b2Vec2 gravity;
  gravity.Set(settings.gravity_x, settings.gravity_y);
  m_world = std::make_unique<b2World>(gravity);

  // m_world = new b2World(gravity);
  // b2BodyDef bodyDef;
  // m_groundBody = m_world->CreateBody(&bodyDef);

  b2Body* ground = nullptr;
  {
    // track is created
    b2BodyDef bd;
    bd.type = b2_staticBody;
    ground = m_world->CreateBody(&bd);

    b2EdgeShape shape;

    b2FixtureDef fd;
    fd.shape = &shape;
    fd.density = settings.ground_density;
    fd.friction = settings.ground_friction;

    const int size = 100;  // settings.number_of_stages;
    const float dx = settings.stage_width_x;

    // start is at flat plate, size 10*dx, on the ground level
    shape.Set(b2Vec2(-5 * dx, 0.0f), b2Vec2(5 * dx, 0.0f));
    ground->CreateFixture(&fd);

    // create high wall at the left end of the track
    shape.Set(b2Vec2(-5 * dx, 0.0f), b2Vec2(-5 * dx, 100 * dx));
    ground->CreateFixture(&fd);

    // on the right end of the starting plate
    // a bumpy track is created, starting from (5dx,0) point
    float x = 5 * dx;
    float y1 = 0.0f;

    // vector holding y positions of next points of the track
    // x position change is constant step defined in the settings file
    float hs[size] = {
        1.0f, 0.3f,  0.4f, -1.0f, -0.2f, 0.5f,  0.4f,  0.5f,  -0.25f, 0.0f,
        0.1f, -0.1f, 0.3f, 0.0f,  0.0f,  -0.1f, -0.2f, -0.5f, -0.25f, 0.0f,
        1.0f, 0.3f,  0.4f, -1.0f, -0.2f, 0.5f,  0.4f,  0.5f,  -0.25f, 0.0f,
        0.1f, -0.1f, 0.3f, 0.0f,  0.0f,  -0.1f, -0.2f, -0.5f, -0.25f, 0.0f,
        1.0f, 0.3f,  0.4f, -1.0f, -0.2f, 0.5f,  0.4f,  0.5f,  -0.25f, 0.0f,
        0.1f, -0.1f, 0.3f, 0.0f,  0.0f,  -0.1f, -0.2f, -0.5f, -0.25f, 0.0f,
        1.0f, 0.3f,  0.4f, -1.0f, -0.2f, 0.5f,  0.4f,  0.5f,  -0.25f, 0.0f,
        0.1f, -0.1f, 0.3f, 0.0f,  0.0f,  -0.1f, -0.2f, -0.5f, -0.25f, 0.0f,
        1.0f, 0.3f,  0.4f, -1.0f, -0.2f, 0.5f,  0.4f,  0.5f,  -0.25f, 0.0f,
        0.1f, -0.1f, 0.3f, 0.0f,  0.0f,  -0.1f, -0.2f, -0.5f, -0.25f, 0.0f};

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
    shape.Set(b2Vec2(x - dx, 0.0f), b2Vec2(x - dx, 100 * dx));
    ground->CreateFixture(&fd);

    // loading points of track to send it to the visualisation module
    route.emplace_back(-5 * dx, 5 * dx);  // wall at the left end
    route.emplace_back(-5 * dx, 0);       // starting plate left end
    route.emplace_back(5 * dx, 0);        // starting plate right end

    double x_ = 5 * dx;
    double y_;
    // loading road
    for (int k = 0; k < size /*settings.number_of_stages*/; k++) {
      x_ += dx;
      y_ = hs[k];
      route.emplace_back(x_, y_);
    }

    x_ += dx;
    route.emplace_back(x_, 0);
    route.emplace_back(x_, 100 * dx);

    for (int k = 0; k < size + 5; k++) {
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
  static const size_t car_No = population_.cars().carsNum();

  // vertices need constant parameter and has problem with parameter taken form
  // the other file
  // static const int BODY_POINTS_NUM =
  // (population_.cars().parametersNum()-2)/2;

  static const int BODY_POINTS_NUM = 8;

  std::cout << "Body points:" << BODY_POINTS_NUM << std::endl;

  // vector of cars
  std::vector<b2Body*> cars;

  // loading parametersMatrix from evoultion module
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

    // // // // inserting parameters

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
    float wheel1_x = 0;
    float wheel1_y = 0;

    float wheel2_x = 0;
    float wheel2_y = 0;

    // shape of a car

    int j;
    for (it = parametersMatrix.begin(i) + 2, j = 0;
         it != parametersMatrix.end(i) - 1 && j < BODY_POINTS_NUM; ++it, ++j) {
      x_ = *it;
      ++it;
      y_ = *it;

      logger::info() << x_ << " " << y_;
      vertices[j].Set(x_, y_);

      /*there is no method for getting coordinates from vertices
      so it is better to save needed ones in later stages
      (wheels coordinates)*/

      if (j == settings.wheel1_point_no) {
        wheel1_x = static_cast<float>(x_);
        wheel1_y = static_cast<float>(y_);
      }

      if (j == settings.wheel2_point_no) {
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

    bd.position.Set(settings.starting_position_x, settings.starting_position_y);
    m_car = m_world->CreateBody(&bd);

    m_car->CreateFixture(&chassis, 1.0f);

    /* filetring collisions allows to create cars which collide with
     * track but not with each other. They all have categoryBits equal
     * -belong to same category
     */
    fdc.filter.categoryBits = 0x0002;
    // fdc.filter.maskBits = 0x0001; //not needed, they collide with everything
    // else

    b2Vec2 axis(settings.starting_position_x, settings.starting_position_y);

    // parameters for springs, then take to settings
    // it was added last time, as before the springs were working other way
    float hertz = 4.0f;
    float dampingRatio = 0.7f;
    float omega = 2.0f * b2_pi * hertz;

    // // //wheels creation

    // wheel 1
    fdc.shape = &circle_front;
    fdc.density = settings.wheel1_density;
    fdc.friction = settings.wheel1_friction;

    bd.position.Set(wheel1_x, wheel1_y);  // wheel position
    m_wheel1 = m_world->CreateBody(&bd);
    fdc.filter.categoryBits = 0x0002;
    // fdc.filter.maskBits = 0x0001;
    m_wheel1->CreateFixture(&fdc);

    jd.Initialize(m_car, m_wheel1, m_wheel1->GetPosition(), axis);
    float mass1 = m_wheel1->GetMass();
    jd.motorSpeed = settings.motor1_speed;
    jd.maxMotorTorque = settings.motor1_maxTorque;
    jd.enableMotor = settings.motor1_enable;
    //       car_t.jd.frequencyHz = settings.motor1_frequencyHz;
    jd.damping = settings.wheel1_dampingRatio;
    jd.stiffness = mass1 * omega * omega;
    jd.damping = 2.0f * mass1 * dampingRatio * omega;
    jd.lowerTranslation = -0.25f;
    jd.upperTranslation = 0.25f;
    jd.enableLimit = true;

    m_spring1 = (b2WheelJoint*)m_world->CreateJoint(&jd);

    // wheel 2
    // m_car->CreateFixture(&car_t.chassis, 1.0f);
    fdc.shape = &circle_rear;
    fdc.density = settings.wheel2_density;
    fdc.friction = settings.wheel2_friction;

    fdc.filter.categoryBits = 0x0002;
    // fdc.filter.maskBits = 0x0001;

    bd.position.Set(wheel2_x, wheel2_y);  // wheel position
    m_wheel2 = m_world->CreateBody(&bd);
    m_wheel2->CreateFixture(&fdc);

    jd.Initialize(m_car, m_wheel2, m_wheel2->GetPosition(), axis);
    float mass2 = m_wheel2->GetMass();
    jd.motorSpeed = settings.motor2_speed;
    jd.maxMotorTorque = settings.motor2_maxTorque;
    jd.enableMotor = settings.motor2_enable;
    //        car_t.jd.frequencyHz = settings.motor2_frequencyHz;
    jd.damping = settings.wheel2_dampingRatio;
    jd.stiffness = mass2 * omega * omega;
    jd.damping = 2.0f * mass2 * dampingRatio * omega;
    jd.lowerTranslation = -0.25f;
    jd.upperTranslation = 0.25f;
    jd.enableLimit = true;

    m_spring2 = (b2WheelJoint*)m_world->CreateJoint(&jd);

    m_spring1->SetMotorSpeed(50);
    // m_spring2->SetMotorSpeed(m_speed);

    car_t.iter_stopped = 0;  // simualtion management default parameter
    car_t.stopped = 0;       // simualtion management default parameter

    // printf("%d \n", car_t.car_num);

    // calculating Center of mass of each car relatively to rear wheel
    b2Vec2 mass_center = (m_car->GetLocalCenter());
    car_t.CoM_position.x = 0;
    car_t.CoM_position.y = 0;
    std::cout << -mass_center.x << std::endl;
    std::cout << -mass_center.y << std::endl;
    car_t.CoM_position.x = -mass_center.x - wheel1_x;
    car_t.CoM_position.y = -mass_center.y - wheel1_y;

    cars_struct.push_back(car_t);
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
  int flag_stop = 0;  // 1 if one of cars reached ending

  std::vector<b2Body*>::iterator it;

  b2Vec2 position;
  b2Vec2 last_position;
  b2Vec2 diff_position;  // for b2vec2 only -= operator is defined

  last_position.SetZero();
  float angle;

  for (size_t i = 0; i < cars.size(); i++) {
    Car car(i);
    cars_struct.push_back(car);
  }

  std::vector<Car>::iterator it2;

  b2Vec2 axis(settings.starting_position_x, settings.starting_position_y);
  for (it = cars.begin(), it2 = cars_struct.begin();
       it != /*cars.begin() + 1*/ cars.end(); ++it, ++it2) {
    /*printf("Nr: %d    (%4.2f, %4.2f)= (%4.2f, %4.2f)-(%4.2f, %4.2f)\n",
           it2->car_num, (it2->mass_center).x, (it2->mass_center).y,
           ((*it)->GetLocalCenter()).x, ((*it)->GetLocalCenter()).y, axis.x,
           axis.y);*/
  }

  while (!stop && iter < settings.sim_max_iter) {
    // simulate computations
    //    logger::info() << "runSimulation" << j;

    m_world->Step(timeStep, settings.m_velocityIterations,
                  settings.m_positionIterations);

    /*for each car
     *jeżeli tak zdecydujemy, to można nie liczyć dla samochodów
     *które mają status zatrzymanych*/
    for (it = cars.begin(), it2 = cars_struct.begin();
         it != /*cars.begin() + 1*/ cars.end(); ++it, ++it2) {
      position = (*it)->GetPosition();
      angle = (*it)->GetAngle();
      std::cout << it2->CoM_position.x << std::endl;
      std::cout << it2->CoM_position.y << std::endl;
      Position position_ = {position.x + it2->CoM_position.x,
                            position.y + it2->CoM_position.y, angle};

      // printf("%d\n",iter/10);
      // if(iter==0){
      // printf("%d %4.2f %4.2f %4.2f\n", it2->car_num, position_.x,
      // position_.y,
      //             position_.theta);

      // printf("%d %4.2f %4.2f %4.2f\n", it2->car_num, position_.x + mass_x,
      //      position_.y + mass_y, position_.theta);
      //}

      simulation_data_->pushPosition(it2->car_num, position_);

      // maximal reached distance

      if (position.x > it2->maximal_distance_reached)
        it2->maximal_distance_reached = position.x;

      // check if any car reached ending
      // later parameters can be taken to the settings struct
      if (position_.x > 200 && position_.y > -5) {
        std::cout << "Car " << it2->car_num
                  << "has reached the end of the track" << std::endl;
        flag_stop = 1;
        break;
      }

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

      if (it2->iter_stopped > settings.max_car_iter)  // flag up
        it2->stopped = 1;

      iter++;

    }  // for each car

    if (flag_stop) break;

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

// bool cer::physics::World::runDummySimulation() {
//  auto cars_num = population_.cars().carsNum();
//  simulation_data_->reset(cars_num);

//  std::vector<float> speeds(cars_num);
//  for (auto& s : speeds) {
//    s = float(rand() % 10);
//  }
//  for (int j = 0; j < 200; ++j) {
//    {
//      for (size_t i = 0; i < cars_num; ++i) {
//        auto p =
//            Position{(0.05f * speeds[i] + 1.0f) * j, 0.5, speeds[i] * 0.1f *
//            j};
//        if (i == 1) {
//          std::cout << p.x << " " << p.y << std::endl;
//        }
//        simulation_data_->pushPosition(i, p);
//      }
//    }
//  }
//  return true;
//}

bool cer::physics::World::runDummySimulation() {
  auto cars_num = population_.cars().carsNum();
  simulation_data_->reset(cars_num);

  const int size = 100;  // settings.number_of_stages;
  const float dx = settings.stage_width_x;

  float x = 5 * dx;
  float y1 = 0.0f;

  float hs[size] = {
      1.0f, 0.3f,  0.4f, -1.0f, -0.2f, 0.5f,  0.4f,  0.5f,  -0.25f, 0.0f,
      0.1f, -0.1f, 0.3f, 0.0f,  0.0f,  -0.1f, -0.2f, -0.5f, -0.25f, 0.0f,
      1.0f, 0.3f,  0.4f, -1.0f, -0.2f, 0.5f,  0.4f,  0.5f,  -0.25f, 0.0f,
      0.1f, -0.1f, 0.3f, 0.0f,  0.0f,  -0.1f, -0.2f, -0.5f, -0.25f, 0.0f,
      1.0f, 0.3f,  0.4f, -1.0f, -0.2f, 0.5f,  0.4f,  0.5f,  -0.25f, 0.0f,
      0.1f, -0.1f, 0.3f, 0.0f,  0.0f,  -0.1f, -0.2f, -0.5f, -0.25f, 0.0f,
      1.0f, 0.3f,  0.4f, -1.0f, -0.2f, 0.5f,  0.4f,  0.5f,  -0.25f, 0.0f,
      0.1f, -0.1f, 0.3f, 0.0f,  0.0f,  -0.1f, -0.2f, -0.5f, -0.25f, 0.0f,
      1.0f, 0.3f,  0.4f, -1.0f, -0.2f, 0.5f,  0.4f,  0.5f,  -0.25f, 0.0f,
      0.1f, -0.1f, 0.3f, 0.0f,  0.0f,  -0.1f, -0.2f, -0.5f, -0.25f, 0.0f};

  double M_PI = 3.14159265358;  // sorka, ale u mnie na zaden sposób nie dziala
  for (unsigned int i = 0; i < size; ++i) {
    float y2 = hs[i];
    auto p = Position{x, y2, float(x * M_PI) / 180.0f};
    simulation_data_->pushPosition(1, p);

    x += dx;
  }

  return true;
}

/*
cer::physics::World::~World() {
  // By deleting the world, we delete the bomb, mouse joint, etc.
  delete m_world;
  m_world = NULL;
}*/
