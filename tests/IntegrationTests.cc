#define BOOST_TEST_MODULE integration_test

#include <boost/test/unit_test.hpp>

#include <iostream>
#include <stdexcept>
#include <vector>

#include "CarsEvolutionRoot.h"
#include "ParametersMatrix.h"

BOOST_AUTO_TEST_CASE(integration_test) {
  cer::CarsEvolutionRoot root(123456);

  BOOST_CHECK_NO_THROW(root.generatePopulation());

  // check population 1
  {
    auto cars = root.cars();
    auto cars_params = std::vector<double>(cars.begin(0), cars.end(0));
    auto expected_params = std::vector<double>{
        0.5,  0.24, 0.57, 0.39, 0.52, 0.86, 0.94, 0.47, 1.35,
        0.23, 1.59, 0.96, 0.61, 1.12, 0.71, 0.67, 0.58, 0.79};

    std::cout << "car parameters 1: ";
    for (auto p : cars_params) {
      std::cout << p << ", ";
    }
    std::cout << std::endl;

    for (size_t i = 0; i < expected_params.size(); ++i) {
      BOOST_CHECK_CLOSE(expected_params[i], cars_params[i], 1e-3);
    }
  }

  // simulation 1
  {
    root.runSimulation();
    auto best_distance = root.getBestCar().second;
    auto expected_best_distance = 19.9696;
    std::cout << "best distance after 1st run: " << best_distance << std::endl;
    BOOST_CHECK_CLOSE(best_distance, expected_best_distance, 1e-2);
  }

  BOOST_CHECK_NO_THROW(root.generatePopulation());

  // check population 2
  {
    auto cars = root.cars();
    auto cars_params = std::vector<double>(cars.begin(0), cars.end(0));
    auto expected_params = std::vector<double>{
        0.484455, 0.151404, 0.357902, 0.511513, 1.08164,  0.587781,
        1.22338,  0.738789, 1.14102,  0.486298, 1.61858,  0.800144,
        0.899096, 1.02494,  0.684538, 0.781075, 0.504685, 0.61451};

    std::cout << "car parameters 2: ";
    for (auto p : cars_params) {
      std::cout << p << ", ";
    }
    std::cout << std::endl;

    for (size_t i = 0; i < expected_params.size(); ++i) {
      BOOST_CHECK_CLOSE(expected_params[i], cars_params[i], 1e-3);
    }
  }

  // simulation 2
  {
    root.runSimulation();
    auto best_distance = root.getBestCar().second;
    auto expected_best_distance = 19.9827;
    std::cout << "best distance after 2nd run: " << best_distance << std::endl;
    BOOST_CHECK_CLOSE(best_distance, expected_best_distance, 1e-2);
  }

  BOOST_CHECK_NO_THROW(root.generatePopulation());

  // check population 3
  {
    auto cars = root.cars();
    auto cars_params = std::vector<double>(cars.begin(0), cars.end(0));
    auto expected_params = std::vector<double>{
        0.494383, 0.20683,  0.418559, 0.372205, 0.752759, 0.74014,
        1.22896,  0.570576, 1.26541,  0.93148,  1.2897,   0.830898,
        0.884155, 1.15223,  0.705175, 1.01565,  0.445037, 0.647167};

    std::cout << "car parameters 3: ";
    for (auto p : cars_params) {
      std::cout << p << ", ";
    }
    std::cout << std::endl;

    for (size_t i = 0; i < expected_params.size(); ++i) {
      BOOST_CHECK_CLOSE(expected_params[i], cars_params[i], 1e-3);
    }
  }

  // simulation 3
  {
    root.runSimulation();
    auto best_distance = root.getBestCar().second;
    auto expected_best_distance = 180.54;
    std::cout << "best distance after 3rd run: " << best_distance << std::endl;
    BOOST_CHECK_CLOSE(best_distance, expected_best_distance, 1e-2);
  }
}
