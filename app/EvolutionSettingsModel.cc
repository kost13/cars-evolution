#include "EvolutionSettingsModel.h"

#include "CarsEvolutionRoot.h"

// temp
#include <iostream>

EvolutionSettingsModel::EvolutionSettingsModel(cer::CarsEvolutionRoot *root,
                                               QObject *parent)
    : QAbstractListModel(parent), root_(root) {
  auto parameters_map = root->parameters();
  int k{0};
  for (const auto &p : parameters_map) {
    parameters_.insert({k, p.second});
    parameters_keys_.insert({k, p.first});
    ++k;
  }
}

int EvolutionSettingsModel::rowCount(const QModelIndex & /*parent*/) const {
  return parameters_.size();
}

int EvolutionSettingsModel::columnCount(const QModelIndex & /*parent*/) const {
  return 3;
}

QVariant EvolutionSettingsModel::data(const QModelIndex &index,
                                      int role) const {
  if (!index.isValid() || size_t(index.row()) >= parameters_.size()) {
    return {};
  }

  switch (role) {
    case RoleNames::NameRole:
      return QString::fromStdString(parameters_.at(index.row()).accesible_name);
    case RoleNames::ValueRole:
      return parameters_.at(index.row()).value;
    case RoleNames::DescriptionRole:
      return QString::fromStdString(parameters_.at(index.row()).description);
  }

  return {};
}

bool EvolutionSettingsModel::setData(const QModelIndex &index,
                                     const QVariant &value, int role) {
  if (data(index, role) != value && size_t(index.row()) < parameters_.size()) {
    auto key = parameters_keys_[index.row()];
    parameters_[index.row()].value = value.toDouble();
    root_->setParameterValue(key, value.toDouble());

    emit dataChanged(index, index, QVector<int>() << role);
    return true;
  }
  return false;
}

QHash<int, QByteArray> EvolutionSettingsModel::roleNames() const {
  QHash<int, QByteArray> roles;
  roles[NameRole] = "name";
  roles[ValueRole] = "value";
  roles[DescriptionRole] = "description";
  return roles;
}
