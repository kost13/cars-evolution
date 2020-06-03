#include <boost/test/unit_test.hpp>
#include <stdexcept>
#include <vector>

#include "CarsPopulationData.h"
#include "Evolution/Evolution.h"
#include "Evolution/MathUtils.h"
#include "Physics/Car.h"
#include "Physics/World.h"
#include "SimulationData.h"

BOOST_AUTO_TEST_CASE(world_test) {
  /// physics tests
  cer::CarsPopulationData pd;
  cer::evolution::Evolution ev(&pd, 12345);
  ev.generatePopulation();
  auto population = pd.cars();
  ev.setPopulationFitness(std::vector<double>(population.carsNum(), 1.0));
  ev.generatePopulation();
  auto params = ev.parameters();
  cer::SimulationData *simulation_data_;
  cer::physics::World world(pd, simulation_data_);
  std::vector<std::pair<double, double>> route;
  float dx = 2.0f;
  route.emplace_back(-5 * dx, 5 * dx);
  route.emplace_back(-5 * dx, 0);
  route.emplace_back(5 * dx, 0);

  float x = 5 * dx;
  float y1 = 0.0f;
  const int size = 100;

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
    y1 = y2;
    x += dx;
    route.emplace_back(x, y1);
  }
  route.emplace_back(x - dx, 0);
  route.emplace_back(x - dx, 100 * dx);

  BOOST_CHECK(world.getRoute() == route);

  BOOST_CHECK(world.runSimulation());

  bool flag = 0;
  std::vector<double> distances = world.maxDistanceReached();
  for (const auto &p : distances)
    if (p > 50) flag = 1;

  BOOST_CHECK(flag);
}
