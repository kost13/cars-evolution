// module: App
// author: Lukasz Kostrzewa

#include "AppBuilder.h"

#include <QCoreApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>

#include "CarsEvolutionRoot.h"

#include "AppInterface.h"
#include "CarsPopulationModel.h"
#include "EvolutionSettingsModel.h"
#include "FileIO.h"

int AppBuilder::run(const std::vector<char *> & /*args*/) {
  QQmlApplicationEngine engine;

  cer::CarsEvolutionRoot root;

  qApp->setOrganizationName("CarsEvolution");
  qApp->setOrganizationDomain("CarsEvolution");

  AppInterface interface(&root);

  CarsPopulationModel population_model(root);
  EvolutionSettingsModel evolution_settings_model(&root);

  QObject::connect(&interface, &AppInterface::newPopulationGenerated,
                   &population_model, &CarsPopulationModel::updatePoplation);

  qmlRegisterType<FileIO>("carsevolution", 1, 0, "FileIO");

  engine.rootContext()->setContextProperty("AppInterface", &interface);

  engine.rootContext()->setContextProperty("PopulationModel",
                                           &population_model);

  engine.rootContext()->setContextProperty("EvolutionSettingsModel",
                                           &evolution_settings_model);

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
