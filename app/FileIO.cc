// module: App
// author: Lukasz Kostrzewa

#include "FileIO.h"

#include <QFile>
#include <QQmlFile>
#include <QTextStream>

#include "cpputils/logger.hpp"

QString FileIO::readFile(const QUrl &path) {
  QString file_name = QQmlFile::urlToLocalFileOrQrc(path);
  if (!QFile::exists(file_name)) {
    cpputils::log::critical() << "File" << file_name.toStdString()
                              << " doesn't exist.";
    return {};
  }

  QFile f(file_name);
  if (!f.open(QFile::ReadOnly)) {
    cpputils::log::critical() << "Cannot open file" << file_name.toStdString();
    return {};
  }

  QTextStream ts(&f);
  return ts.readAll();
}
