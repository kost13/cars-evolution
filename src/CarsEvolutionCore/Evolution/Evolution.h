#ifndef EVOLUTION_H
#define EVOLUTION_H

namespace cer {

class CarsPopulationData;

namespace evolution {
class Evolution {
 public:
  explicit Evolution(CarsPopulationData *population);
  void generatePopulation();

 private:
  CarsPopulationData *population_;
};

}  // namespace evolution
}  // namespace cer

#endif  // EVOLUTION_H
