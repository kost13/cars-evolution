#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <QQuickStyle>

#include "CarsEvolutionCore/CarsEvolutionRoot.h"

#include "AppInterface.h"

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

  CarsEvolutionRoot root;

  AppInterface interface(&root);

  QQmlApplicationEngine engine;
  engine.rootContext()->setContextProperty("AppInterface", &interface);

  QQuickStyle::setStyle("Material");

  const QUrl url(QStringLiteral("qrc:/gui/main.qml"));
  QObject::connect(&engine, &QQmlApplicationEngine::objectCreated, &app,
                   [url](QObject *obj, const QUrl &objUrl) {
                     if ((obj == nullptr) && url == objUrl) {
                       QCoreApplication::exit(-1);
                     }
                   },
                   Qt::QueuedConnection);
  engine.load(url);

  return QApplication::exec();
}
