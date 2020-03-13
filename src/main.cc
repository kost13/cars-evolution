#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "core/CarsEvolutionRoot.h"

#include "AppInterface.h"
#include "Worker.h"

int main(int argc, char *argv[]) {
  QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
  QGuiApplication app(argc, argv);

  CarsEvolutionRoot root;

  concurrent::Worker worker;
  worker.start();

  AppInterface interface(&root, &worker);

  QQmlApplicationEngine engine;
  engine.rootContext()->setContextProperty("AppInterface", &interface);

  const QUrl url(QStringLiteral("qrc:/gui/main.qml"));
  QObject::connect(&engine, &QQmlApplicationEngine::objectCreated, &app,
                   [url](QObject *obj, const QUrl &objUrl) {
                     if (!obj && url == objUrl) QCoreApplication::exit(-1);
                   },
                   Qt::QueuedConnection);
  engine.load(url);

  return app.exec();
}
