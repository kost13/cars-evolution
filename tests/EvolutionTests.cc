#define BOOST_TEST_MODULE
#include <boost/test/unit_test.hpp>

#include <stdexcept>
#include <vector>

#include "CarsPopulationData.h"
#include "Evolution/Evolution.h"
#include "Evolution/MathUtils.h"

BOOST_AUTO_TEST_CASE(evolution_test) {
  cer::CarsPopulationData pd;
  BOOST_TEST(pd.cars().size() == 0);

  cer::evolution::Evolution ev(&pd, 12345);
  ev.generatePopulation();
  BOOST_TEST(pd.cars().size() > 0);

  auto population = pd.cars();

  ev.generatePopulation();
  BOOST_TEST(pd.cars().size() == population.size());

  for (size_t i = 0; i < population.carsNum(); ++i) {
    auto p1_b = population.begin(i);
    auto p1_e = population.end(i);

    auto p2_b = pd.cars().begin(i);
    auto p2_e = pd.cars().end(i);

    for (; p1_b != p1_e && p2_b != p2_e; ++p1_b, ++p2_b) {
      BOOST_TEST((*p1_b) != (*p2_b));
    }
  }
}

BOOST_AUTO_TEST_CASE(evolution_math_test) {
  namespace math = cer::evolution::math;

  std::vector<double> parent1(40, 1);
  std::vector<double> parent2(40, 2);

  auto child = math::crossover(parent1.begin(), parent1.end(), parent2.begin(),
                               parent2.end());

  BOOST_TEST(child.size() == parent1.size());
  for (auto p : child) {
    BOOST_TEST((p >= 1. && p <= 2.));
  }

  std::vector<double> fitness(10);
  std::iota(fitness.begin(), fitness.end(), 1);

  for (int i = 2; i < 7; ++i) {
    auto selected = math::tournamentSelection(fitness, i, i / 2 + 3);
    BOOST_TEST(selected.size() == i);
    for (auto s : selected) {
      BOOST_TEST(s < 10);
    }
  }
}
