#ifndef MATHUTILS_H
#define MATHUTILS_H

#include <random>
#include <vector>

namespace cer {
namespace evolution {
class RandomGenerator {
 public:
  RandomGenerator() = default;
  explicit RandomGenerator(double std);
  void setStd(double std);
  double operator()(double v) const;

 private:
  double std_{1.0};
};

using iterator = std::vector<double>::iterator;

std::vector<double> crossover(iterator p1_first, iterator p1_last,
                              iterator p2_first, iterator p2_last);

void mutate(iterator first, iterator last, const RandomGenerator &rg);

}  // namespace evolution
}  // namespace cer

#endif  // MATHUTILS_H
