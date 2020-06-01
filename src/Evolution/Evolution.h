// module: Core.Evolution
// author: Lukasz Kostrzewa

#ifndef EVOLUTION_H
#define EVOLUTION_H

#include <map>
#include <vector>

#include "CarsPopulationData.h"
#include "MathUtils.h"

namespace cer {

class CarsPopulationData;

namespace evolution {

///
/// \brief The Parameter struct wraps parameter's name, value and description
///
struct Parameter {
  std::string accesible_name;
  double value;
  std::string description;
};

///
/// \brief The Evolution class manages evolution algorithm.
///
class Evolution {
 public:
  ///
  /// \brief Evolution
  /// \param population
  /// \param seed - a seed for random numbers generation, by default equals -1,
  /// which means that it is based on time.
  ///
  explicit Evolution(CarsPopulationData *population, int seed = -1);

  ///
  /// \brief setPopulationFitness
  /// \param fitness - vector with distance reached by each car
  ///
  void setPopulationFitness(const std::vector<double> &fitness);

  ///
  /// \brief generatePopulation generates a new population and adds it to
  /// CarsPopulationData
  /// If there is no previous population, a new one is generated randomly.
  /// Otherwise a new population is generated from the previous one using
  /// evolution algorithm.
  void generatePopulation();

  ///
  /// \brief parameters
  /// \return map with parameters
  /// This method is thrad safe.
  ///
  std::map<std::string, Parameter> parameters() const;

  ///
  /// \brief setParameterValue
  /// \param name - parameter key
  /// \param val - parameter value
  /// This method is thread safe.
  void setParameterValue(const std::string &name, double val);

 private:
  void initializeEvolutionParameters();
  cer::ParametersMatrix initialPopulation(size_t cars_num) const;
  CarsPopulationData *population_;
  std::vector<double> population_fitness_;
  std::map<std::string, Parameter> parameters_;
  bool first_run_{true};
  int seed_{};
  mutable std::mutex parameters_mutex_;
};

}  // namespace evolution
}  // namespace cer

#endif  // EVOLUTION_H
