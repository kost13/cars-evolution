#ifndef APPBUILDER_H
#define APPBUILDER_H

#include <QObject>
#include <QQmlApplicationEngine>

#include <vector>

class AppBuilder : public QObject {
  Q_OBJECT

 public:
  explicit AppBuilder() = default;
  int run(const std::vector<char *> &args);
};

#endif  // APPBUILDER_H
