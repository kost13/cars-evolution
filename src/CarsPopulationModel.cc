#include "CarsPopulationModel.h"

#include <cstdlib>

#include "CarsEvolutionCore/CarsPopulationData.h"

#include "cpputils/logger.hpp"

// temp
#include <QDebug>

namespace {
const int MIN_COLOR = 26;
const int COLOR_RANGE = 200;
}

CarsPopulationModel::CarsPopulationModel(cer::CarsPopulationData *population,
                                         QObject *parent)
    : QAbstractListModel(parent), population_(population) {
  srand(time(nullptr));
}

int CarsPopulationModel::rowCount(const QModelIndex &parent) const {
  if (parent.isValid()) {
    return 0;
  }
  return cars_.size();
}

int CarsPopulationModel::columnCount(const QModelIndex &parent) const {
  if (parent.isValid()) {
    return 0;
  }
  return 1;
}

QVariant CarsPopulationModel::data(const QModelIndex &index, int role) const {
  if (!index.isValid() || index.row() >= cars_colors_.size() ||
      index.row() >= cars_.size()) {
    return {};
  }

  switch (role) {
    case NumberRole:
      // cars have no names, just numbers
      return QString::number(index.row() + 1);
    case ColorRole:
      return cars_colors_[index.row()];
    case ParametersRole:
      return structureToVariantList(cars_[index.row()]);
    default:
      return {};
  }
}

QHash<int, QByteArray> CarsPopulationModel::roleNames() const {
  QHash<int, QByteArray> roles;
  roles[NumberRole] = "number";
  roles[ColorRole] = "color";
  roles[ParametersRole] = "parameters";
  return roles;
}

void CarsPopulationModel::updatePoplation() {
  cars_ = population_->cars();
  colorCars();
}

void CarsPopulationModel::colorCars() {
  cars_colors_.clear();
  cars_colors_.reserve(cars_.size());

  int color[3];
  for (size_t i = 0; i < cars_.size(); ++i) {
    for (auto &c : color) {
      c = rand() % COLOR_RANGE + MIN_COLOR;
    }
    cars_colors_.emplace_back(color[0], color[1], color[2]);
  }
}

QVariantList CarsPopulationModel::structureToVariantList(
    const cer::CarParameters &car) const {
  QVariantList params;

  params.reserve(cer::CarParameters::BODY_POINTS_NUM * 2 + 2);
  params.push_back(car.front_wheel);
  params.push_back(car.rear_wheel);
  for (auto p : car.body_points) {
    params.push_back(p);
  }
  return params;
}
