#define BOOST_TEST_MODULE world_test

#include <boost/test/unit_test.hpp>

#include <stdexcept>
#include <vector>

#include "CarsPopulationData.h"
#include "Physics/World.h"
#include "SimulationData.h"

BOOST_AUTO_TEST_CASE(world_test) {
  using cer::CarParameters;
  using cer::Position;

  cer::CarsPopulationData pd;
  pd.setCars({cer::CarParameters{1., 1., std::vector<double>{1., 2., 3.}},
              cer::CarParameters{4., 4., std::vector<double>{5., 6., 7.}}});

  cer::SimulationData sd;
  BOOST_TEST(sd.carsNum() == 0);

  cer::physics::World w(pd, &sd);
  w.runSimulation();

  BOOST_TEST(sd.carsNum() == 2);

  BOOST_TEST(sd.popPosition(0).x != -1);
  BOOST_TEST(sd.popPosition(1).y != -1);
}
