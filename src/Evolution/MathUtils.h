#ifndef MATHUTILS_H
#define MATHUTILS_H

#include <random>
#include <vector>

namespace cer {
namespace evolution {
namespace math {

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

std::vector<size_t> tournamentSelection(const std::vector<double> &fitness,
                                        size_t n, size_t tournament_size = 3);

}  // namespace math
}  // namespace evolution
}  // namespace cer

#endif  // MATHUTILS_H
