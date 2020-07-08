#ifndef EVOLUTIONSETTINGSMODEL_H
#define EVOLUTIONSETTINGSMODEL_H

#include <QAbstractListModel>
#include "Evolution/Evolution.h"

namespace cer {
class CarsEvolutionRoot;
}

class EvolutionSettingsModel : public QAbstractListModel {
  Q_OBJECT

 public:
  enum RoleNames { NameRole = Qt::UserRole + 1, ValueRole, DescriptionRole };

  explicit EvolutionSettingsModel(cer::CarsEvolutionRoot *root,
                                  QObject *parent = nullptr);

  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  int columnCount(const QModelIndex &parent = QModelIndex()) const override;

  QVariant data(const QModelIndex &index,
                int role = Qt::DisplayRole) const override;

  bool setData(const QModelIndex &index, const QVariant &value,
               int role = Qt::EditRole) override;

  QHash<int, QByteArray> roleNames() const override;

 private:
  std::map<int, cer::evolution::Parameter> parameters_;
  std::map<int, std::string> parameters_keys_;
  cer::CarsEvolutionRoot *root_;
};

#endif  // EVOLUTIONSETTINGSMODEL_H
