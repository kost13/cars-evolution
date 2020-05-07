#ifndef PARAMETERSMATRIX_H
#define PARAMETERSMATRIX_H

#include <vector>

namespace cer {

class ParametersMatrix {
 public:
  ParametersMatrix() = default;
  explicit ParametersMatrix(const std::vector<double> &params);

  using iter = std::vector<double>::iterator;
  using citer = std::vector<double>::const_iterator;

  void setParameters(const std::vector<double> &v);
  void setCarParameters(std::size_t car_num, const std::vector<double> &v);
  iter begin(std::size_t car_num);
  iter end(std::size_t car_num);
  citer cbegin(std::size_t car_num) const;
  citer cend(std::size_t car_num) const;
  static std::size_t parametersNum();
  std::size_t carsNum() const;
  bool empty() const;
  std::size_t size() const;

 private:
  std::vector<double> parameters_;
  static const std::size_t params_num_ = 18;
};

}  // namespace cer

#endif  // PARAMETERSMATRIX_H
