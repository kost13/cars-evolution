// module: App
// author: Lukasz Kostrzewa

#ifndef APPBUILDER_H
#define APPBUILDER_H

#include <QObject>
#include <QQmlApplicationEngine>

#include <vector>

///
/// \brief The AppBuilder class creates main app objects
///
class AppBuilder : public QObject {
  Q_OBJECT

 public:
  explicit AppBuilder() = default;
  int run(const std::vector<char *> &args);
};

#endif  // APPBUILDER_H
