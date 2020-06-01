// module: Core.Evolution
// author: Lukasz Kostrzewa

#include "MathUtils.h"

#include <algorithm>
#include <random>
#include <stdexcept>

using cer::evolution::math::iterator;

cer::evolution::math::RandomGenerator::RandomGenerator(int seed)
    : seed_(seed) {}

cer::evolution::math::RandomGenerator::RandomGenerator(double std, int seed)
    : std_(std), seed_(seed) {}

void cer::evolution::math::RandomGenerator::setStd(double std) { std_ = std; }

double cer::evolution::math::RandomGenerator::operator()(double v) const {
  std::mt19937 generator(seed_);
  return std::normal_distribution<double>{v, std_}(generator);
}

std::vector<double> cer::evolution::math::crossover(iterator p1_first,
                                                    iterator p1_last,
                                                    iterator p2_first,
                                                    iterator p2_last) {
  if (std::distance(p1_first, p1_last) != std::distance(p2_first, p2_last)) {
    throw std::invalid_argument("Crossover vectors sizes don't match");
  }

  std::vector<double> child;
  child.reserve(std::distance(p1_first, p1_last));
  for (; p1_first != p1_last && p2_first != p2_last; ++p1_first, ++p2_first) {
    auto w = static_cast<double>(std::rand()) / RAND_MAX;
    child.push_back(w * (*p1_first) + (1. - w) * (*p2_first));
  }
  return child;
}

void cer::evolution::math::mutate(iterator first, iterator last,
                                  const RandomGenerator &rg) {
  auto l = lowerLimits(std::distance(first, last));
  auto u = upperLimits(std::distance(first, last));
  int k{0};
  for (; first != last; ++first) {
    *first = std::min(u[k], std::max(rg(*first), l[k]));
    ++k;
  }
}

std::vector<size_t> cer::evolution::math::tournamentSelection(
    const std::vector<double> &fitness, size_t n, size_t tournament_size) {
  std::vector<size_t> res;
  std::vector<size_t> tournament(tournament_size);
  auto pop_size = fitness.size();
  for (; n > 0; --n) {
    for (size_t i = 0; i < tournament_size; ++i) {
      tournament[i] = std::rand() % pop_size;
    }
    auto selected = std::max_element(
        tournament.begin(), tournament.end(),
        [&](size_t a, size_t b) { return fitness.at(a) < fitness.at(b); });
    res.push_back(*selected);
  }
  return res;
}

std::vector<double> cer::evolution::math::lowerLimits(
    std::size_t params_count) {
  auto v = std::vector<double>(params_count, 0.0);
  if (params_count > 1) {
    v[0] = v[1] = 0.01;
  }
  return v;
}

std::vector<double> cer::evolution::math::upperLimits(
    std::size_t params_count) {
  return std::vector<double>(params_count, 3.0);
}
