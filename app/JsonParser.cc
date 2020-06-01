// module: App
// author: Lukasz Kostrzewa

#include "JsonParser.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include <cpputils/logger.hpp>

#include "CarsEvolutionRoot.h"
#include "CarsPopulationData.h"

namespace {
QJsonArray carParametersToJson(const cer::ParametersMatrix &parameters) {
  QJsonArray array;
  for (size_t i = 0; i < parameters.carsNum(); ++i) {
    auto it = parameters.cbegin(i);
    auto end = parameters.cend(i);

    QJsonObject params{{"front_wheel", *it}, {"rear_wheel", *(++it)}};
    QJsonArray points;
    for (; it != end; ++it) {
      points.append(*it);
    }
    params.insert("body_points", points);
    array.append(params);
  }
  return array;
}

cer::ParametersMatrix carParametersFromJson(const QJsonArray &parameters) {
  std::vector<double> params;

  for (const auto &par : parameters) {
    auto o = par.toObject();
    params.push_back(o.value("front_wheel").toDouble());
    params.push_back(o.value("rear_wheel").toDouble());

    auto points = o.value("body_points").toArray();
    for (const auto &p : points) {
      params.push_back(p.toDouble());
    }
  }
  return cer::ParametersMatrix{params};
}

bool saveFile(const QString &file_path, const QByteArray &data) {
  QString fname(file_path);
  if (!file_path.endsWith(".json")) {
    fname += ".json";
  }

  QFile f(fname);
  if (!f.open(QIODevice::WriteOnly)) {
    cpputils::log::critical() << "Cannot open file to save "
                              << file_path.toStdString();
    return false;
  }

  f.write(data);
  f.close();
  return true;
}

QJsonArray readFile(const QString &file_path) {
  QFile f(file_path);
  if (!f.open(QIODevice::ReadOnly)) {
    cpputils::log::critical() << "Cannot open file " << file_path.toStdString();
    return {};
  }

  auto doc = QJsonDocument::fromJson(f.readAll());
  return doc.object().value("parameters").toArray();
}

}  // namespace

std::pair<cer::ParametersMatrix, bool> json_parser::readParameters(
    const QString &file_path) {
  auto parameters = readFile(file_path);
  if (parameters.empty()) {
    cpputils::log::critical() << "Cannot read parameters from file "
                              << file_path.toStdString();
    return {{}, false};
  }

  return {carParametersFromJson(parameters), true};
}

bool json_parser::writeParameters(const cer::ParametersMatrix &parameters,
                                  const QString &file_path) {
  QJsonArray array = carParametersToJson(parameters);
  QJsonDocument doc(QJsonObject{{"parameters", array}});

  return saveFile(file_path, doc.toJson());
}
