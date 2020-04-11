#include <QApplication>
#include <QQmlApplicationEngine>

#include "AppBuilder.h"

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

  std::vector<char *> args(argv + 1, argv + argc);

  QQmlApplicationEngine engine;

  AppBuilder appBuilder(engine, args);

  return QApplication::exec();
}
