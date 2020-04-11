#ifndef CARSPOPULATIONDATA_H
#define CARSPOPULATIONDATA_H

#include <mutex>
#include <vector>

namespace cer {

struct CarParameters {
  double front_wheel{};             // front wheel diameter
  double rear_wheel{};              // rear wheel diameter
  std::vector<double> body_points;  // x and y coordiantes of the body polygon
                                    // order: x_1, y_1, ..., x_n, y_n

  static const int BODY_POINTS_NUM = 8;
};

class CarsPopulationData {
 public:
  CarsPopulationData();

  std::vector<CarParameters> cars();
  std::vector<double> carsVector();

  void setCars(const std::vector<CarParameters> &cars);
  CarParameters car(size_t ind);

 private:
  std::vector<CarParameters> cars_;
  std::mutex mutex_;
};

}  // namespace cer

#endif  // CARSPOPULATIONDATA_H
