#ifndef CARSPOPULATIONDATA_H
#define CARSPOPULATIONDATA_H

#include <mutex>
#include <utility>
#include <vector>

#include "ParametersMatrix.h"

namespace cer {

class CarsPopulationData {
 public:
  CarsPopulationData();

  ParametersMatrix cars() const;

  void setCars(const ParametersMatrix &cars);

  bool empty() const;

 private:
  ParametersMatrix cars_;
  mutable std::mutex mutex_;
};

}  // namespace cer

#endif  // CARSPOPULATIONDATA_H
