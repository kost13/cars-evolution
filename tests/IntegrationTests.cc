#define BOOST_TEST_MODULE integration_test

#include <boost/test/unit_test.hpp>

#include <iostream>

#include "CarsEvolutionRoot.h"

BOOST_AUTO_TEST_CASE(integration_test) {
  cer::CarsEvolutionRoot root;

  BOOST_CHECK_NO_THROW(root.generatePopulation());

  // simulation 1
  {
    root.runSimulation();
    auto best_distance = root.getBestCar().second;
    std::cout << "best distance after 1st run: " << best_distance << std::endl;
  }

  BOOST_CHECK_NO_THROW(root.generatePopulation());

  // simulation 2
  {
    root.runSimulation();
    auto best_distance = root.getBestCar().second;
    std::cout << "best distance after 2nd run: " << best_distance << std::endl;
  }

  BOOST_CHECK_NO_THROW(root.generatePopulation());

  // simulation 3
  {
    root.runSimulation();
    auto best_distance = root.getBestCar().second;
    std::cout << "best distance after 3rd run: " << best_distance << std::endl;
  }

  BOOST_CHECK_NO_THROW(root.generatePopulation());

  // simulation 4
  {
    root.runSimulation();
    auto best_distance = root.getBestCar().second;
    std::cout << "best distance after 4th run: " << best_distance << std::endl;
  }

  BOOST_CHECK_NO_THROW(root.generatePopulation());

  // simulation 5
  {
    root.runSimulation();
    auto best_distance = root.getBestCar().second;
    std::cout << "best distance after 5th run: " << best_distance << std::endl;
  }

  BOOST_CHECK_NO_THROW(root.generatePopulation());

  // simulation 6
  {
    root.runSimulation();
    auto best_distance = root.getBestCar().second;
    std::cout << "best distance after 6th run: " << best_distance << std::endl;
  }
}
