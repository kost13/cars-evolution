#ifndef CARSPOPULATIONMODEL_H
#define CARSPOPULATIONMODEL_H

#include <vector>

#include <QAbstractListModel>
#include <QColor>

#include "CarsEvolutionCore/CarsPopulationData.h"

namespace cer {
class CarsEvolutionRoot;
}

class CarsPopulationModel : public QAbstractListModel {
  Q_OBJECT

 public:
  enum RoleNames { NumberRole = Qt::UserRole + 1, ColorRole, ParametersRole };

  explicit CarsPopulationModel(const cer::CarsEvolutionRoot &root,
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
  QVariantList toVariantList(cer::ParametersMatrix::citer begin,
                             cer::ParametersMatrix::citer end) const;
  const cer::CarsEvolutionRoot &root_;
  cer::ParametersMatrix cars_;
  std::vector<QColor> cars_colors_;
};

#endif  // CARSPOPULATIONMODEL_H
