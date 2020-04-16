#include <utility>

#ifndef CARSPOPULATIONDATA_H
#define CARSPOPULATIONDATA_H

#include <mutex>
#include <vector>

namespace cer {

struct CarParameters {
  CarParameters() = default;
  CarParameters(double fw, double rw, std::vector<double> bp)
      : front_wheel(fw), rear_wheel(rw), body_points(std::move(bp)) {}
  double front_wheel{};             // front wheel diameter
  double rear_wheel{};              // rear wheel diameter
  std::vector<double> body_points;  // x and y coordiantes of the body polygon
                                    // order: x_1, y_1, ..., x_n, y_n
  static const int BODY_POINTS_NUM = 8;
};

class CarsPopulationData {
 public:
  CarsPopulationData();

  std::vector<CarParameters> cars() const;

  std::vector<double> carsVector() const;

  void setCars(const std::vector<CarParameters> &cars);

  CarParameters car(size_t ind);

 private:
  std::vector<CarParameters> cars_;
  mutable std::mutex mutex_;
};

}  // namespace cer

#endif  // CARSPOPULATIONDATA_H
