#define BOOST_TEST_MODULE
#include <boost/test/unit_test.hpp>

#include <iostream>
#include <stdexcept>
#include <vector>

#include "CarsPopulationData.h"
#include "Evolution/Evolution.h"
#include "Evolution/MathUtils.h"

BOOST_AUTO_TEST_CASE(evolution_test) {
  cer::CarsPopulationData pd;
  BOOST_CHECK(pd.cars().empty());

  cer::evolution::Evolution ev(&pd, 12345);
  ev.generatePopulation();
  BOOST_CHECK(!pd.cars().empty());

  auto population = pd.cars();

  ev.generatePopulation();
  BOOST_CHECK_EQUAL(pd.cars().size(), population.size());

  for (size_t i = 0; i < population.carsNum(); ++i) {
    auto p1_b = population.begin(i);
    auto p1_e = population.end(i);

    auto p2_b = pd.cars().begin(i);
    auto p2_e = pd.cars().end(i);

    for (; p1_b != p1_e && p2_b != p2_e; ++p1_b, ++p2_b) {
      BOOST_CHECK((*p1_b) != (*p2_b));
    }
  }
}

BOOST_AUTO_TEST_CASE(evolution_parameters_test) {
  cer::CarsPopulationData pd;
  cer::evolution::Evolution ev(&pd);

  auto params = ev.parameters();
  BOOST_CHECK(!params.empty());

  for (const auto &p : params) {
    ev.setParameterValue(p.first, p.second.value + 1);
  }

  auto params_new = ev.parameters();

  for (const auto &p : params) {
    BOOST_CHECK(params_new.find(p.first) != params_new.end());
    BOOST_CHECK_EQUAL(p.second.value + 1, params_new[p.first].value);
  }
}

BOOST_AUTO_TEST_CASE(evolution_math_test) {
  namespace math = cer::evolution::math;

  std::vector<double> parent1(40, 1);
  std::vector<double> parent2(40, 2);
  std::vector<double> child;

  BOOST_CHECK_THROW(math::crossover(parent1.begin(), parent1.end(),
                                    parent2.begin(), parent2.end() - 1),
                    std::invalid_argument);

  BOOST_CHECK_NO_THROW(child = math::crossover(parent1.begin(), parent1.end(),
                                               parent2.begin(), parent2.end()));

  BOOST_CHECK_EQUAL(child.size(), parent1.size());
  for (auto p : child) {
    BOOST_CHECK((p >= 1. && p <= 2.));
  }

  std::vector<double> fitness(10);
  std::iota(fitness.begin(), fitness.end(), 1);

  for (size_t i = 2; i < 7; ++i) {
    auto selected = math::tournamentSelection(fitness, i, i / 2 + 3);
    BOOST_CHECK_EQUAL(selected.size(), i);
    std::cout << "inidices selected in tournament, there should be mostly "
                 "numbers close to 10: "
              << std::endl;
    for (auto s : selected) {
      std::cout << s << " ";
      BOOST_CHECK(s < 10);
    }
    std::cout << std::endl;
  }

  size_t n = 10;
  std::vector<double> parameters(n, 1.0);
  math::RandomGenerator rg(6);
  math::mutate(parameters.begin(), parameters.end(), rg);
  auto l = math::lowerLimits(n);
  auto u = math::upperLimits(n);
  for (size_t i = 0; i < n; ++i) {
    BOOST_CHECK(u[i] >= parameters[i] && l[i] <= parameters[i]);
  }
}
