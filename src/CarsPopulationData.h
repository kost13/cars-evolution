// module: Core
// author: Lukasz Kostrzewa

#ifndef CARSPOPULATIONDATA_H
#define CARSPOPULATIONDATA_H

#include <mutex>
#include <utility>
#include <vector>

#include "ParametersMatrix.h"

namespace cer {

///
/// \brief The CarsPopulationData class is a wrapper for the ParametersMatrix
/// The CarsPopulationData class allows to use ParametersMatrix in the thread
/// safe way. All methods are thread safe.
///
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
