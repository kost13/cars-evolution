#ifndef CARSEVOLUTIONROOT_H
#define CARSEVOLUTIONROOT_H

#include <memory>
#include <vector>

//#include "CarStructure.h"

namespace cer {

class CarsPopulationData;

struct Position {
  Position(float xx, float yy, float thetaa) : x(xx), y(yy), theta(thetaa) {}
  float x, y, theta;
};

class CarsEvolutionRoot {
 public:
  explicit CarsEvolutionRoot();

  CarsEvolutionRoot(const CarsEvolutionRoot &) = delete;
  CarsEvolutionRoot &operator=(const CarsEvolutionRoot &) = delete;

  CarsEvolutionRoot(const CarsEvolutionRoot &&) = delete;
  CarsEvolutionRoot &operator=(const CarsEvolutionRoot &&) = delete;

  ~CarsEvolutionRoot();

  void runSimulation();
  Position getPosition(int car_num);

  //  std::vector<Car::Parameters> getCars() const;
  CarsPopulationData *carsPopulation() const;

 private:
  struct Opaque;
  std::unique_ptr<Opaque> o_;
};
}  // namespace cer

#endif  // CARSEVOLUTIONROOT_H
