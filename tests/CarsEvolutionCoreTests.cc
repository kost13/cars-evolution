#include <boost/test/unit_test.hpp>

#include <map>
#include <stdexcept>
#include <vector>

#include "CarsEvolutionRoot.h"
#include "CarsPopulationData.h"
#include "Evolution/Evolution.h"
#include "SimulationData.h"

BOOST_AUTO_TEST_CASE(population_parameters_test) {
  using cer::CarsPopulationData;

  BOOST_CHECK_THROW(cer::ParametersMatrix(std::vector<double>{1, 2, 3}),
                    std::runtime_error);

  cer::ParametersMatrix mat;
  int cars_num = 6;
  std::vector<double> params(cer::ParametersMatrix::parametersNum() * cars_num,
                             1.0);
  BOOST_CHECK_NO_THROW(mat.setParameters(params));

  cer::CarsPopulationData d;
  BOOST_CHECK(d.empty());
  d.setCars(mat);
  BOOST_CHECK(!d.empty());
  BOOST_CHECK(!d.cars().empty());

  BOOST_CHECK_EQUAL(d.cars().carsNum(), cars_num);
  BOOST_CHECK_EQUAL(d.cars().size(),
                    cer::ParametersMatrix::parametersNum() * cars_num);

  BOOST_CHECK_EQUAL(std::distance(mat.begin(0), mat.end(0)),
                    cer::ParametersMatrix::parametersNum());

  BOOST_CHECK_EQUAL(std::distance(mat.cbegin(1), mat.cend(1)),
                    cer::ParametersMatrix::parametersNum());

  for (auto it = mat.begin(0); it != mat.end(0); ++it) {
    BOOST_CHECK_EQUAL(*it, 1.0);
  }

  std::vector<double> new_params(
      cer::ParametersMatrix::parametersNum() * cars_num, 2.0);
  BOOST_CHECK_NO_THROW(mat.setParameters(new_params));

  for (auto it = mat.begin(1); it != mat.end(1); ++it) {
    BOOST_CHECK_EQUAL(*it, 2.0);
  }

  BOOST_CHECK_NO_THROW(mat.setCarParameters(
      2, std::vector<double>(cer::ParametersMatrix::parametersNum(), 3.0)));

  for (auto it = mat.begin(3); it != mat.end(3); ++it) {
    BOOST_CHECK_EQUAL(*it, 2.0);
  }

  for (auto it = mat.begin(2); it != mat.end(2); ++it) {
    BOOST_CHECK_EQUAL(*it, 3.0);
  }
}

BOOST_AUTO_TEST_CASE(parameters_matrix_exceptions) {
  cer::ParametersMatrix m;
  std::vector<double> params(cer::ParametersMatrix::parametersNum() + 2, 1.0);
  BOOST_CHECK_THROW(m.setParameters(params), std::runtime_error);
  std::vector<double> good_params(cer::ParametersMatrix::parametersNum(), 1.0);
  BOOST_CHECK_THROW(m.setCarParameters(1, good_params), std::out_of_range);
  m.setParameters(good_params);
  BOOST_CHECK_THROW(m.setCarParameters(0, params), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(simulation_data_test) {
  cer::SimulationData sd;
  BOOST_CHECK_EQUAL(sd.carsNum(), 0);
  sd.reset(2);
  BOOST_CHECK_EQUAL(sd.carsNum(), 2);
  BOOST_CHECK_THROW(sd.pushPosition(2, cer::Position{}), std::out_of_range);
  cer::Position pos1{1.0, 2.0, 3.0};
  cer::Position pos2{4.0, 5.0, 6.0};
  cer::Position pos3{7.0, 8.0, 9.0};
  BOOST_CHECK_NO_THROW(sd.pushPosition(0, pos1));
  BOOST_CHECK_NO_THROW(sd.pushPosition(0, pos2));
  cer::Position pos{};
  auto compare = [](const cer::Position &p1, const cer::Position &p2) {
    return p1.x == p2.x && p1.y == p2.y && p1.theta == p2.theta;
  };

  BOOST_CHECK_NO_THROW(pos = sd.popPosition(0));
  BOOST_CHECK(compare(pos, pos1));
  BOOST_CHECK_THROW(sd.popPosition(3), std::out_of_range);
  BOOST_CHECK_NO_THROW(sd.pushPosition(1, pos3));
  BOOST_CHECK(compare(sd.popPosition(1), pos3));
  BOOST_CHECK(compare(sd.popPosition(0), pos2));
  BOOST_CHECK(compare(sd.popPosition(0), cer::Position{-1, -1, -1}));
  BOOST_CHECK(compare(sd.popPosition(1), cer::Position{-1, -1, -1}));
}

BOOST_AUTO_TEST_CASE(root_test) {
  cer::CarsEvolutionRoot r;
  auto route = r.getRoute();
  BOOST_CHECK(!route.empty());
  BOOST_CHECK(r.cars().empty());
  BOOST_CHECK(!r.parameters().empty());
  BOOST_CHECK(r.parameters().find("std") != r.parameters().end());
  r.setParameterValue("std", 0.5);
  BOOST_CHECK_EQUAL(r.parameters()["std"].value, 0.5);
  r.generatePopulation();
  BOOST_CHECK(!r.cars().empty());
  r.runSimulation();
  BOOST_CHECK(r.getBestCar().second > 0.0);
  auto pos = r.popPosition(0);
  BOOST_CHECK(pos.x != -1 && pos.y != -1 && pos.theta != -1);
  BOOST_CHECK_THROW(r.popPosition(r.cars().carsNum()), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(root_loaded_population_test) {
  cer::CarsEvolutionRoot r;
  BOOST_CHECK(r.cars().empty());

  std::vector<double> population{
      0.5,  0.5,  0.4,  0.4,  0.80, 0.50, 1.10, 0.60, 1.20, 0.40, 1.21, 0.81,
      1.00, 0.90, 0.70, 0.90, 0.50, 0.60, 0.6,  0.6,  0.4,  0.4,  0.80, 0.50,
      1.10, 0.60, 1.20, 0.40, 1.21, 0.81, 1.00, 0.90, 0.70, 0.90, 0.50, 0.60};

  BOOST_CHECK_NO_THROW(r.setCars(cer::ParametersMatrix(population)));
  BOOST_CHECK(!r.cars().empty());
  BOOST_CHECK_EQUAL(r.cars().carsNum(), 2);

  r.generatePopulation();
  r.runSimulation();
  BOOST_CHECK(r.getBestCar().second > 0.0);
  auto pos = r.popPosition(0);
  BOOST_CHECK(pos.x != -1 && pos.y != -1 && pos.theta != -1);

  r.generatePopulation();
  r.runSimulation();
  BOOST_CHECK(r.getBestCar().second > 0.0);
  pos = r.popPosition(1);
  BOOST_CHECK(pos.x != -1 && pos.y != -1 && pos.theta != -1);
}
