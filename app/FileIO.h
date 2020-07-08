// module: App
// author: Lukasz Kostrzewa

#ifndef FILEIO_H
#define FILEIO_H

#include <QObject>

///
/// \brief The FileIO class allows to read the given file
///
class FileIO : public QObject {
  Q_OBJECT
 public:
  FileIO() = default;

  ///
  /// \brief read file
  /// \param path to the file
  /// \return string with the given file content
  Q_INVOKABLE QString readFile(const QUrl &path);
};

#endif  // FILEIO_H
