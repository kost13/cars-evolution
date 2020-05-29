#ifndef CARSEVOLUTIONROOT_H
#define CARSEVOLUTIONROOT_H

#include <map>
#include <memory>
#include <vector>

namespace cer {

struct ParametersMatrix;
struct Position;

namespace evolution {
struct Parameter;
}

class CarsEvolutionRoot {
 public:
  explicit CarsEvolutionRoot();

  CarsEvolutionRoot(const CarsEvolutionRoot &) = delete;
  CarsEvolutionRoot &operator=(const CarsEvolutionRoot &) = delete;

  CarsEvolutionRoot(const CarsEvolutionRoot &&) = delete;
  CarsEvolutionRoot &operator=(const CarsEvolutionRoot &&) = delete;

  ~CarsEvolutionRoot();

  // population

  void generatePopulation();

  ParametersMatrix cars() const;

  void setCars(const ParametersMatrix &cars);

  // evolution parameters

  std::map<std::string, evolution::Parameter> parameters() const;

  void setParameterValue(const std::string &name, double val);

  // simulation

  void runSimulation();

  Position popPosition(size_t car_index);

  std::vector<std::pair<double, double>> getRoute();

 private:
  struct Opaque;
  std::unique_ptr<Opaque> o_;
};
}  // namespace cer

#endif  // CARSEVOLUTIONROOT_H
