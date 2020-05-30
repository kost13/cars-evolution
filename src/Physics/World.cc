#include "World.h"

#include <cpputils/logger.hpp>

#include "Car.h"
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
    /*
        for (int k = 0; k < size + 5; k++) {
          std::cout << "x: " << route[k].first << std::endl
                    << "y: " << route[k].second << std::endl;
        }*/
  }
}

std::vector<std::pair<double, double>> cer::physics::World::getRoute() {
  return route;
}

std::vector<cer::physics::Car*> cer::physics::World::generateCars() {
  // number of cars in simulation
  static const size_t car_No = population_.cars().carsNum();

  // vertices need constant parameter and has problem with parameter taken form
  // the other file
  // static const int BODY_POINTS_NUM =
  // (population_.cars().parametersNum()-2)/2;

  static const int BODY_POINTS_NUM = 8;

  std::cout << "Body points:" << BODY_POINTS_NUM << std::endl;

  // vector of cars
  std::vector<Car*> cars;

  // loading parametersMatrix from evoultion module
  auto parametersMatrix = population_.cars();

  std::vector<double>::iterator it;

  // for each car
  for (size_t i = 0; i < car_No; i++) {
    std::vector<double> parametersVector(parametersMatrix.begin(i),
                                         parametersMatrix.end(i));
    Car* car = new Car(m_world, parametersVector, i);
    cars.push_back(car);
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
  std::vector<Car*> cars = generateCars();

  bool stop = 0;  // flag for simulation control
  int iter = 0;
  int flag_stop = 0;  // 1 if one of cars reached ending

  std::vector<Car*>::iterator it;

  b2Vec2 position;
  b2Vec2 last_position;
  b2Vec2 diff_position;  // for b2vec2 only -= operator is defined

  last_position.SetZero();
  float angle;

  for (size_t i = 0; i < cars.size(); i++) {
    Car_parameters car(i);
    cars_struct.push_back(car);
  }

  std::vector<Car_parameters>::iterator it2;

  b2Vec2 axis(0.0f, 1.0f);
  for (it = cars.begin(); it != /*cars.begin() + 1*/ cars.end(); ++it) {
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
      b2Body* car_t = ((*it)->getCar());
      position = (car_t)->GetPosition();
      angle = (car_t)->GetAngle();
      /*
      // calculating Center of mass of each car relatively to rear wheel
      b2Vec2 mass_center = ((*it)->GetLocalCenter());
      b2Vec2 LocCS_position;  // position of local CS in global CS
      LocCS_position.x =
          position.x - ((mass_center.x + it2->wheel2_pos_.x) * cos(angle) -
                        (mass_center.y + it2->wheel2_pos_.y) * sin(angle));
      LocCS_position.y =
          position.y - ((mass_center.x + it2->wheel2_pos_.x) * sin(angle) +
                        (mass_center.y + it2->wheel2_pos_.y) * cos(angle));
      b2Vec2 rwheel_pos_GCS;  // rear wheel position in global coordiate system
      rwheel_pos_GCS.x = LocCS_position.x + ((it2->wheel2_pos_.x) * cos(angle) -
                                             (it2->wheel2_pos_.y) * sin(angle));
      rwheel_pos_GCS.y = LocCS_position.y + ((it2->wheel2_pos_.x) * sin(angle) +
                                             (it2->wheel2_pos_.y) * cos(angle));

*/
      // std::cout << -mass_center.x << std::endl;
      // std::cout << -mass_center.y << std::endl;
      // it2->CoM_position.x = -mass_center.x - it2->wheel2_pos.x;
      // it2->CoM_position.y = -mass_center.y - it2->wheel2_pos.y;

      // std::cout << rwheel_pos_GCS.x << std::endl;
      // std::cout << rwheel_pos_GCS.y << std::endl;

      auto dx = (*it)->getRwheelPos().x;
      auto dy = (*it)->getRwheelPos().y;

      float dx_r = dx * cos(-angle) - dy * sin(-angle);
      float dy_r = dx * sin(-angle) + dy * cos(-angle);
      //      std::cout << angle << " " << dx_r << " " << dy_r << " " <<
      //      position.x
      //                << " " << position.y << " " << position.x + dx_r << " "
      //                << position.y + dy_r << std::endl;
      Position position_ = {position.x + dx_r, position.y + dy_r, -angle};
      simulation_data_->pushPosition((*it)->getCarNum(), position_);

      // Position position_ = {rwheel_pos_GCS.x, rwheel_pos_GCS.y, angle};

      // printf("%d\n",iter/10);
      // if(iter==0){
      // printf("%d %4.2f %4.2f %4.2f\n", it2->car_num, position_.x,
      // position_.y,
      //             position_.theta);

      printf("%d %4.2f %4.2f %4.2f\n", (*it)->getCarNum(), position_.x,
             position_.y, position_.theta);
      //}

      // simulation_data_->pushPosition(it2->car_num_, position_);

      // maximal reached distance

      if (position_.x > (*it)->getMaximalDistanceReached())
        (*it)->setMaximalDistanceReached(position_.x);

      // check if any car reached ending
      // later parameters can be taken to the settings struct
      if (position_.x > 200 && position_.y > -5) {
        std::cout << "Car " << (*it)->getCarNum()
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
        (it2->iter_stopped_)++;
      else
        it2->iter_stopped_ = 0;

      position = last_position;

      if (it2->iter_stopped_ > settings.max_car_iter)  // flag up
        it2->stopped_ = 1;

      iter++;

    }  // for each car

    if (flag_stop) break;

    // Checks if any car is moving.
    // If not, 'stop' will be left with value 1 and simualtion stopped.
    stop = 1;
    for (it2 = cars_struct.begin(); it2 != cars_struct.end(); it2++)
      if (!it2->stopped_) {
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
