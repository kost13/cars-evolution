#define BOOST_TEST_MODULE population_test
#define BOOST_TEST_MODULE simulation_data_test
#include <boost/test/unit_test.hpp>

#include <stdexcept>
#include <vector>

#include "CarsPopulationData.h"
#include "SimulationData.h"

BOOST_AUTO_TEST_CASE(population_test) {
  using cer::CarParameters;
  auto population = std::vector<CarParameters>{
      CarParameters{1., 2., std::vector<double>{3., 4., 5., 6.}},
      CarParameters{7., 8., std::vector<double>{9., 10., 11., 12.}}};
  cer::CarsPopulationData o;
  o.setCars(population);

  BOOST_TEST(o.cars().size() == 2);
  BOOST_TEST(o.carsVector().size() == 2 * 6);

  BOOST_TEST(o.car(0).front_wheel == 1.);
  BOOST_TEST(o.car(0).rear_wheel == 2.);
  BOOST_TEST(o.car(0).body_points.size() == 4);
  BOOST_TEST(o.car(0).body_points[1] == 4.);

  BOOST_TEST(o.car(1).front_wheel == 7.);
  BOOST_TEST(o.car(1).rear_wheel == 8.);
  BOOST_TEST(o.car(1).body_points.size() == 4);
  BOOST_TEST(o.car(1).body_points[3] == 12.);

  o.setCars({});
  BOOST_TEST(o.cars().size() == 0);
  BOOST_TEST(o.carsVector().size() == 2 * 0);
}

BOOST_AUTO_TEST_CASE(simulation_data_test) {
  using cer::Position;
  cer::SimulationData o;
  BOOST_TEST(o.carsNum() == 0);
  o.reset(2);
  BOOST_TEST(o.popPosition(0).x == -1);
  BOOST_TEST(o.carsNum() == 2);
  BOOST_CHECK_THROW(o.popPosition(2), std::out_of_range);

  o.pushPosition(0, {1, 1, 1});
  o.pushPosition(0, {2, 2, 2});
  o.pushPosition(1, {2, 3, 4});
  BOOST_CHECK_THROW(o.pushPosition(3, {3, 3, 3}), std::out_of_range);

  BOOST_TEST(o.popPosition(0).y == 1);
  BOOST_TEST(o.popPosition(0).theta == 2);
  BOOST_TEST(o.popPosition(0).x == -1);

  auto pos = o.popPosition(1);
  BOOST_TEST(pos.x == 2);
  BOOST_TEST(pos.y == 3);
  BOOST_TEST(pos.theta == 4);
}
