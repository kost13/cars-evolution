#include <boost/test/unit_test.hpp>

#include <stdexcept>
#include <vector>

#include "CarsPopulationData.h"
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

BOOST_AUTO_TEST_CASE(simulation_data_test) {}
