#include "CarsPopulationModel.h"

#include <cstdlib>

#include "CarsEvolutionCore/CarsEvolutionRoot.h"
#include "CarsEvolutionCore/CarsPopulationData.h"

#include "cpputils/logger.hpp"

#include <qdebug.h>

namespace {
const int MIN_COLOR = 20;
const int COLOR_RANGE = 210;
}  // namespace

CarsPopulationModel::CarsPopulationModel(const cer::CarsEvolutionRoot &root,
                                         QObject *parent)
    : QAbstractListModel(parent), root_(root) {
  srand(time(nullptr));
}

int CarsPopulationModel::rowCount(const QModelIndex &parent) const {
  if (parent.isValid()) {
    return 0;
  }
  return static_cast<int>(cars_.carsNum());
}

int CarsPopulationModel::columnCount(const QModelIndex &parent) const {
  if (parent.isValid()) {
    return 0;
  }
  return 1;
}

QVariant CarsPopulationModel::data(const QModelIndex &index, int role) const {
  if (!index.isValid() ||
      static_cast<size_t>(index.row()) >= cars_colors_.size() ||
      static_cast<size_t>(index.row()) >= cars_.carsNum()) {
    return {};
  }

  switch (role) {
    case NumberRole:
      // cars have no names, just numbers
      return QString::number(index.row() + 1);
    case ColorRole:
      return cars_colors_[index.row()];
    case ParametersRole: {
      auto row = index.row();
      return toVariantList(cars_.cbegin(row), cars_.cend(row));
    }
    default:
      return {};
  }
}

QVariantList CarsPopulationModel::parameters(int row) const {
  if (row < 0 || static_cast<size_t>(row) > cars_.carsNum()) {
    return {};
  }
  return toVariantList(cars_.cbegin(row), cars_.cend(row));
}

QString CarsPopulationModel::color(int row) const {
  if (row < 0 || static_cast<size_t>(row) > cars_colors_.size()) {
    return {};
  }
  return cars_colors_[row].name();
}

QHash<int, QByteArray> CarsPopulationModel::roleNames() const {
  QHash<int, QByteArray> roles;
  roles[NumberRole] = "number";
  roles[ColorRole] = "color";
  roles[ParametersRole] = "parameters";
  return roles;
}

void CarsPopulationModel::updatePoplation() {
  beginResetModel();
  cars_ = root_.cars();
  colorCars();
  endResetModel();
}

void CarsPopulationModel::colorCars() {
  cars_colors_.clear();
  cars_colors_.reserve(cars_.carsNum());

  int color[3];
  for (size_t i = 0; i < cars_.carsNum(); ++i) {
    for (auto &c : color) {
      c = rand() % COLOR_RANGE + MIN_COLOR;
    }
    cars_colors_.emplace_back(color[0], color[1], color[2]);
  }
}

QVariantList CarsPopulationModel::toVariantList(
    cer::ParametersMatrix::citer begin,
    cer::ParametersMatrix::citer end) const {
  QVariantList l;
  for (; begin != end; ++begin) {
    l.push_back(*begin);
  }
  return l;
}
