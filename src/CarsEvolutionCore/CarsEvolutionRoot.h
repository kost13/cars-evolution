#ifndef CARSEVOLUTIONROOT_H
#define CARSEVOLUTIONROOT_H

#include <memory>
#include <vector>

namespace cer {

class CarsPopulationData;
class SimulationData;

class CarsEvolutionRoot {
 public:
  explicit CarsEvolutionRoot();

  CarsEvolutionRoot(const CarsEvolutionRoot &) = delete;
  CarsEvolutionRoot &operator=(const CarsEvolutionRoot &) = delete;

  CarsEvolutionRoot(const CarsEvolutionRoot &&) = delete;
  CarsEvolutionRoot &operator=(const CarsEvolutionRoot &&) = delete;

  ~CarsEvolutionRoot();

  void generatePopulation();

  void runSimulation();

  SimulationData *simulationData() const;
  CarsPopulationData *carsPopulation() const;

 private:
  struct Opaque;
  std::unique_ptr<Opaque> o_;
};
}  // namespace cer

#endif  // CARSEVOLUTIONROOT_H
