#ifndef EVOLUTION_H
#define EVOLUTION_H

#include <vector>
#include "CarsPopulationData.h"
#include "MathUtils.h"

namespace cer {

class CarsPopulationData;

namespace evolution {
class Evolution {
 public:
  explicit Evolution(CarsPopulationData *population, bool time_seed = true);
  void setPopulationFitness(const std::vector<double> &fitness);
  void generatePopulation();

 private:
  CarsPopulationData *population_;
  std::vector<double> population_fitness_;
  math::RandomGenerator random_generator_{};
};

}  // namespace evolution
}  // namespace cer

#endif  // EVOLUTION_H
