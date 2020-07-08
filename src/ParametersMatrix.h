// module: Core
// author: Lukasz Kostrzewa

#ifndef PARAMETERSMATRIX_H
#define PARAMETERSMATRIX_H

#include <vector>

namespace cer {

///
/// \brief The ParametersMatrix class stores cars parameters.
///
class ParametersMatrix {
 public:
  ParametersMatrix() = default;
  explicit ParametersMatrix(const std::vector<double> &params);

  using iter = std::vector<double>::iterator;
  using citer = std::vector<double>::const_iterator;

  ///
  /// \brief setParameters
  /// \param v - vector with parameters of the size params_num_ * number of cars
  ///
  void setParameters(const std::vector<double> &v);

  ///
  /// \brief setCarParameters
  /// \param car_num
  /// \param v - vector with parameters of the size params_num_
  ///
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
