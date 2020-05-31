// module: Core.Evolution
// author: Lukasz Kostrzewa

#ifndef MATHUTILS_H
#define MATHUTILS_H

#include <random>
#include <vector>

namespace cer {
namespace evolution {

///
/// Math functions for evolution algorithm
///
namespace math {

///
/// \brief The RandomGenerator class
/// It generates random numbers from gaussian distribution
///
class RandomGenerator {
 public:
  RandomGenerator() = default;
  explicit RandomGenerator(double std);
  ///
  /// \brief setStd
  /// \param std - standard deviation of the gaussian distribution
  ///
  void setStd(double std);

  ///
  /// \brief operator ()
  /// \param v - mean value of the distribution
  /// \return random number from gaussian distribution
  ///
  double operator()(double v) const;

 private:
  double std_{1.0};
};

using iterator = std::vector<double>::iterator;

///
/// \brief crossover generates a child from the given vectors
/// \param p1_first - first parent begin iterator
/// \param p1_last - first parent end iterator
/// \param p2_first - second parent begin iterator
/// \param p2_last - second parent end iterator
/// \return a child vector
/// For the two given vectors it computes a new vector of the same size. The
/// formula: new_vector[i] = w* vec1[i] + (1-w)*vec2[i] where w comes from
/// uniform distribution U(0,1). If input vectors sizes don't match it throws
/// std::invalid_arguments exception.
///
std::vector<double> crossover(iterator p1_first, iterator p1_last,
                              iterator p2_first, iterator p2_last);

///
/// \brief mutate modifies the given vector
/// \param first - vector to modify begin iterator
/// \param last - vector to modify end iterator
/// \param rg - RandomGenerator used to modify the vector
/// Mutation function uses lowerLimits and upperLimits functions to generate a
/// new vector within that range.
///
void mutate(iterator first, iterator last, const RandomGenerator &rg);

///
/// \brief tournamentSelection
/// \param fitness - fitness function results
/// \param n - number of generated elements
/// \param tournament_size
/// \return vector with indices of the chosen elements.
std::vector<size_t> tournamentSelection(const std::vector<double> &fitness,
                                        size_t n, size_t tournament_size = 3);

///
/// \brief lowerLimits of car parameters
/// \param params_count
/// \return vector with lower limits for cars parameters
std::vector<double> lowerLimits(std::size_t params_count);

///
/// \brief upperLimits of car parameters
/// \param params_count
/// \return vector with upper limits for cars parameters
///
std::vector<double> upperLimits(std::size_t params_count);

}  // namespace math
}  // namespace evolution
}  // namespace cer

#endif  // MATHUTILS_H
