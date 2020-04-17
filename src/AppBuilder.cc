#include "AppBuilder.h"

#include <QCoreApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>

#include "CarsEvolutionCore/CarsEvolutionRoot.h"

#include "AppInterface.h"
#include "CarsPopulationModel.h"
#include "FileIO.h"

int AppBuilder::run(const std::vector<char *> & /*args*/) {
  QQmlApplicationEngine engine;

  cer::CarsEvolutionRoot root;

  AppInterface interface(&root);

  CarsPopulationModel population_model(root);

  QObject::connect(&interface, &AppInterface::newPopulationGenerated,
                   &population_model, &CarsPopulationModel::updatePoplation);

  qmlRegisterType<FileIO>("carsevolution", 1, 0, "FileIO");

  engine.rootContext()->setContextProperty("AppInterface", &interface);

  engine.rootContext()->setContextProperty("PopulationModel",
                                           &population_model);

  QQuickStyle::setStyle("Material");

  const QUrl url(QStringLiteral("qrc:/gui/main.qml"));
  QObject::connect(&engine, &QQmlApplicationEngine::objectCreated, qApp,
                   [url](QObject *obj, const QUrl &objUrl) {
                     if ((obj == nullptr) && url == objUrl) {
                       QCoreApplication::exit(-1);
                     }
                   },
                   Qt::QueuedConnection);
  engine.load(url);

  return qApp->exec();
}
