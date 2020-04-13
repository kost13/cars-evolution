#ifndef CARSPOPULATIONMODEL_H
#define CARSPOPULATIONMODEL_H

#include <vector>

#include <QAbstractListModel>
#include <QColor>

#include "CarsEvolutionCore/CarsPopulationData.h"

class CarsPopulationModel : public QAbstractListModel {
  Q_OBJECT

 public:
  enum RoleNames { NumberRole = Qt::UserRole + 1, ColorRole, ParametersRole };

  explicit CarsPopulationModel(cer::CarsPopulationData *population,
                               QObject *parent = nullptr);

  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  int columnCount(const QModelIndex &parent = QModelIndex()) const override;

  QVariant data(const QModelIndex &index,
                int role = Qt::DisplayRole) const override;

  Q_INVOKABLE QVariantList parameters(int row) const;
  Q_INVOKABLE QString color(int row) const;

  QHash<int, QByteArray> roleNames() const override;

  void updatePoplation();

 private:
  void colorCars();
  QVariantList structureToVariantList(const cer::CarParameters &car) const;
  const cer::CarsPopulationData *const population_;
  std::vector<cer::CarParameters> cars_;
  std::vector<QColor> cars_colors_;
};

#endif  // CARSPOPULATIONMODEL_H
