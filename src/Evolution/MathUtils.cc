#include "MathUtils.h"

#include <algorithm>
#include <random>

namespace {
std::random_device rd{};
std::mt19937 gen{rd()};
}

using cer::evolution::math::iterator;

cer::evolution::math::RandomGenerator::RandomGenerator(double std)
    : std_(std) {}

void cer::evolution::math::RandomGenerator::setStd(double std) { std_ = std; }

double cer::evolution::math::RandomGenerator::operator()(double v) const {
  return std::normal_distribution<double>{v, std_}(gen);
}

std::vector<double> cer::evolution::math::crossover(iterator p1_first,
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

void cer::evolution::math::mutate(iterator first, iterator last,
                                  const RandomGenerator &rg) {
  for (; first != last; ++first) {
    *first = rg(*first);
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
