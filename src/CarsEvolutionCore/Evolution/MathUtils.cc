#include "MathUtils.h"

#include <random>

namespace {
std::random_device rd{};
std::mt19937 gen{rd()};
}

using cer::evolution::iterator;

cer::evolution::RandomGenerator::RandomGenerator(double std) : std_(std) {}

void cer::evolution::RandomGenerator::setStd(double std) { std_ = std; }

double cer::evolution::RandomGenerator::operator()(double v) const {
  return std::normal_distribution<double>{v, std_}(gen);
}

std::vector<double> cer::evolution::crossover(iterator p1_first,
                                              iterator p1_last,
                                              iterator p2_first,
                                              iterator p2_last) {
  std::vector<double> child;
  child.reserve(std::distance(p1_first, p1_last));
  for (; p1_first != p1_last && p2_first != p2_last; ++p1_first, ++p2_first) {
    auto w = static_cast<double>(std::rand()) / RAND_MAX;
    child.push_back(w * (*p1_first) + (1. - w) * (*p2_first));
  }
  return child;
}

void cer::evolution::mutate(iterator first, iterator last,
                            const RandomGenerator &rg) {
  for (; first != last; ++first) {
    *first = rg(*first);
  }
}
