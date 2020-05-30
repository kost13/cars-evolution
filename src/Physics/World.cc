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
  gravity.Set(0, -10.0);
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
    fd.density = 0.0f;
    fd.friction = 0.6f;
    fd.filter.maskBits = 0x0002;
    fd.filter.categoryBits = 0x0001;

    const int size = 100;  // settings.number_of_stages;
    const float dx = 2;

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
  }
}

std::vector<std::pair<double, double>> cer::physics::World::getRoute() {
  return route;
}

std::vector<cer::physics::Car*> cer::physics::World::generateCars() {
  // number of cars in simulation
  static const size_t car_No = population_.cars().carsNum();

  // vector of cars
  std::vector<Car*> cars;

  // loading parametersMatrix from evoultion module
  auto parametersMatrix = population_.cars();

  // for each car
  for (size_t i = 0; i < car_No; i++) {
    std::vector<double> parametersVector(parametersMatrix.begin(i),
                                         parametersMatrix.end(i));
    Car* car = new Car(m_world.get(), parametersVector, i);
    cars.push_back(car);
  }

  return cars;
}

bool cer::physics::World::runSimulation() {
  auto cars_num = population_.cars().carsNum();
  simulation_data_->reset(cars_num);

  float timeStep = 1.0f / 60;

  // creating physical car objects
  //'population_' was assigned in constructor
  cars = generateCars();

  bool stop = 0;  // flag for simulation control
  int iter = 0;
  int flag_stop = 0;  // 1 if one of cars reached ending

  std::vector<Car*>::iterator it;

  //  for (b2Body* b = m_world->GetBodyList(); b; b = b->GetNext()) {
  //    std::cout << b->IsEnabled() << std::endl;
  //  }

  // used for simualtion control
  b2Vec2 last_position;
  b2Vec2 diff_position;  // for b2vec2 only -= operator is defined
  last_position.SetZero();

  while (!stop && iter < 30000) {
    m_world->Step(timeStep, 8, 3);

    // for each car

    for (it = cars.begin(); it != cars.end(); ++it) {
      b2Vec2 position = ((*it)->getCar())->GetPosition();
      float angle = ((*it)->getCar())->GetAngle();

      auto dx = (*it)->getRwheelPos().x;
      auto dy = (*it)->getRwheelPos().y;

      float dx_r = dx * cos(-angle) - dy * sin(-angle);
      float dy_r = dx * sin(-angle) + dy * cos(-angle);

      Position position_ = {dx /*position.x + dx_r*/,
                            dy /*position.y /* + dy_r*/, -angle};
      simulation_data_->pushPosition((*it)->getCarNum(), position_);

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

      int pom = (*it)->GetIterStopped();
      if (diff_position.Length() < 5)
        pom++;
      else
        pom = 0;
      (*it)->setIterStopped(pom);

      position = last_position;

      if ((*it)->GetIterStopped() > 10000)  // flag up
        (*it)->setStopped(1);

      iter++;

    }  // for each car

    if (flag_stop) break;

    // Checks if any car is moving.
    // If not, 'stop' will be left with value 1 and simualtion stopped.
    stop = 1;
    for (it = cars.begin(); it != /*cars.begin() + 1*/ cars.end(); ++it)
      if (!(*it)->getStopped()) {
        stop = 0;
        break;
      }

  }  // simulation loop 'while'

  return true;
}

bool cer::physics::World::runDummySimulation() {
  auto cars_num = population_.cars().carsNum();
  simulation_data_->reset(cars_num);

  const int size = 100;  // settings.number_of_stages;
  const float dx = 2;

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

  for (unsigned int i = 0; i < size; ++i) {
    float y2 = hs[i];
    auto p = Position{x, y2, float(x * b2_pi) / 180.0f};
    simulation_data_->pushPosition(1, p);

    x += dx;
  }

  return true;
}

std::vector<float> cer::physics::World::max_distance_reached() {
  std::vector<float> distance_reached;
  for (auto car = cars.begin(); car != cars.end(); ++car)
    distance_reached.push_back((*car)->getMaximalDistanceReached());

  return distance_reached;
}
