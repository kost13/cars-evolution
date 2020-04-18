#ifndef FILEIO_H
#define FILEIO_H

#include <QObject>

class FileIO : public QObject {
  Q_OBJECT
 public:
  FileIO() = default;
  Q_INVOKABLE QString readFile(const QUrl &path);
};

#endif  // FILEIO_H
