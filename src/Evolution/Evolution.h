#ifndef EVOLUTION_H
#define EVOLUTION_H

#include <map>
#include <vector>

#include "CarsPopulationData.h"
#include "MathUtils.h"

namespace cer {

class CarsPopulationData;

namespace evolution {

struct Parameter {
  std::string accesible_name;
  double value;
  std::string description;
};

class Evolution {
 public:
  explicit Evolution(CarsPopulationData *population, int time_seed = -1);
  void setPopulationFitness(const std::vector<double> &fitness);
  void generatePopulation();
  std::map<std::string, Parameter> parameters() const;
  void setParameterValue(const std::string &name, double val);

 private:
  void initializeEvolutionParameters();
  cer::ParametersMatrix initialPopulation(size_t cars_num) const;
  CarsPopulationData *population_;
  std::vector<double> population_fitness_;
  math::RandomGenerator random_generator_{};
  std::map<std::string, Parameter> parameters_;
  mutable std::mutex parameters_mutex_;
};

}  // namespace evolution
}  // namespace cer

#endif  // EVOLUTION_H
