#include "JsonParser.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include <cpputils/logger.hpp>

#include "CarsEvolutionCore/CarsEvolutionRoot.h"
#include "CarsEvolutionCore/CarsPopulationData.h"

namespace {
QJsonObject carParametersToJson(const cer::CarParameters &car) {
  QJsonObject params{{"front_wheel", car.front_wheel},
                     {"rear_wheel", car.rear_wheel}};
  QJsonArray points;
  for (auto p : car.body_points) {
    points.append(p);
  }
  params.insert("body_points", points);
  return params;
}

cer::CarParameters carParametersFromJson(const QJsonObject &car) {
  cer::CarParameters params;
  params.front_wheel = car.value("front_wheel").toDouble();
  params.front_wheel = car.value("rear_wheel").toDouble();

  auto points = car.value("body_points").toArray();
  std::vector<double> body_points;
  body_points.reserve(points.size());
  for (const auto &p : points) {
    body_points.push_back(p.toDouble());
  }
  params.body_points = body_points;
  return params;
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

std::pair<std::vector<cer::CarParameters>, bool> json_parser::readParameters(
    const QString &file_path) {
  auto parameters = readFile(file_path);
  if (parameters.empty()) {
    cpputils::log::critical() << "Cannot read parameters from file "
                              << file_path.toStdString();
    return {{}, false};
  }

  std::vector<cer::CarParameters> cars;
  cars.reserve(parameters.size());

  for (const auto &p : parameters) {
    auto o = p.toObject();
    cars.push_back(carParametersFromJson(o));
  }

  return {std::move(cars), true};
}

bool json_parser::writeParameters(
    const std::vector<cer::CarParameters> &parameters,
    const QString &file_path) {
  QJsonArray array;
  for (const auto &car : parameters) {
    array.append(carParametersToJson(car));
  }

  QJsonDocument doc(QJsonObject{{"parameters", array}});

  return saveFile(file_path, doc.toJson());
}
