// module: Core
// author: Lukasz Kostrzewa

#include "ParametersMatrix.h"

#include <stdexcept>

cer::ParametersMatrix::ParametersMatrix(const std::vector<double> &params) {
  if (params.size() % params_num_ != 0) {
    throw std::runtime_error("wrong parameters size");
  }
  parameters_ = params;
}

void cer::ParametersMatrix::setParameters(const std::vector<double> &v) {
  if (v.size() % params_num_ != 0) {
    throw std::runtime_error("wrong parameters size");
  }
  parameters_ = v;
}

void cer::ParametersMatrix::setCarParameters(std::size_t car_num,
                                             const std::vector<double> &v) {
  if (car_num >= carsNum()) {
    throw std::out_of_range("car number out of range");
  }
  std::copy(v.begin(), v.end(), begin(car_num));
}

cer::ParametersMatrix::iter cer::ParametersMatrix::begin(std::size_t car_num) {
  return parameters_.begin() + params_num_ * car_num;
}

cer::ParametersMatrix::iter cer::ParametersMatrix::end(std::size_t car_num) {
  return parameters_.begin() + params_num_ * (car_num + 1);
}

cer::ParametersMatrix::citer cer::ParametersMatrix::cbegin(
    std::size_t car_num) const {
  return parameters_.cbegin() + params_num_ * car_num;
}

cer::ParametersMatrix::citer cer::ParametersMatrix::cend(
    std::size_t car_num) const {
  return parameters_.cbegin() + params_num_ * (car_num + 1);
}

std::size_t cer::ParametersMatrix::parametersNum() { return params_num_; }

std::size_t cer::ParametersMatrix::carsNum() const {
  return parameters_.size() / params_num_;
}

bool cer::ParametersMatrix::empty() const { return parameters_.empty(); }

std::size_t cer::ParametersMatrix::size() const { return parameters_.size(); }
