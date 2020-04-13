#define BOOST_TEST_MODULE evolution_test

#include <boost/test/unit_test.hpp>

#include <stdexcept>
#include <vector>

#include "CarsPopulationData.h"
#include "Evolution/Evolution.h"

BOOST_AUTO_TEST_CASE(evolution_test) {
  cer::CarsPopulationData pd;
  BOOST_TEST(pd.cars().size() == 0);

  cer::evolution::Evolution ev(&pd);
  ev.generatePopulation();
  BOOST_TEST(pd.cars().size() > 0);
}
