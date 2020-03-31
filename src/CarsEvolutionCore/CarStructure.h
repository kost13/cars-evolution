#include <utility>

#ifndef CARSTRUCTURE_H
#define CARSTRUCTURE_H

#include <vector>

namespace Car {

const int polygon_size = 8;

struct Parameters {
  Parameters(double fw, double rw, std::vector<double> points)
      : front_wheel{fw}, rear_wheel{rw}, body_points{std::move(points)} {}

  Parameters() = default;
  Parameters(const Parameters &) = default;
  Parameters &operator=(const Parameters &) = default;
  Parameters(Parameters &&) = default;
  Parameters &operator=(Parameters &&) = default;
  ~Parameters() = default;

  double front_wheel{};             // front wheel diameter
  double rear_wheel{};              // rear wheel diameter
  std::vector<double> body_points;  // x and y coordiantes of the body polygon
                                    // order: x_1, y_1, ..., x_n, y_n
};
}  // namespace Car

#endif  // CARSTRUCTURE_H