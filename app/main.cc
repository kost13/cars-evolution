// module: App
// author: Lukasz Kostrzewa

#include <QApplication>

#include "AppBuilder.h"

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

  std::vector<char *> args(argv + 1, argv + argc);

  AppBuilder appBuilder;
  return appBuilder.run(args);
}
