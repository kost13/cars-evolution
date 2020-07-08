// module: App
// author: Lukasz Kostrzewa

#ifndef CARSPOPULATIONMODEL_H
#define CARSPOPULATIONMODEL_H

#include <vector>

#include <QAbstractListModel>
#include <QColor>

#include "CarsPopulationData.h"

namespace cer {
class CarsEvolutionRoot;
}

///
/// \brief The CarsPopulationModel class stores cars parameters and colors
///
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

  ///
  /// \brief get parameters of the car with the given index
  /// \param row
  /// \return list with parameters
  Q_INVOKABLE QVariantList parameters(int row) const;

  ///
  /// \brief get color of the car with the given index
  /// \param row
  /// \return color in #RRGGBB format
  Q_INVOKABLE QString color(int row) const;

  QHash<int, QByteArray> roleNames() const override;

  ///
  /// \brief updatePoplation
  /// Resets the model, loads a new population and generates colors for new
  /// cars
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
