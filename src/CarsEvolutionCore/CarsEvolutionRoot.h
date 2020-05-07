#ifndef CARSEVOLUTIONROOT_H
#define CARSEVOLUTIONROOT_H

#include <memory>
#include <vector>

namespace cer {

struct ParametersMatrix;
struct Position;

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

  ParametersMatrix cars() const;

  void setCars(const ParametersMatrix &cars);

  Position popPosition(size_t car_index);

 private:
  struct Opaque;
  std::unique_ptr<Opaque> o_;
};
}  // namespace cer

#endif  // CARSEVOLUTIONROOT_H
