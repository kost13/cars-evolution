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

  //  QTextCodec *codec = QTextCodec::codecForHtml(data);
  //  setText(codec->toUnicode(data));
  //  if (m_doc) m_doc->setModified(false);
  //  if (fileName.isEmpty())
  //    m_documentTitle = QStringLiteral("untitled.txt");
  //  else
  //    m_documentTitle = QFileInfo(fileName).fileName();

  //  emit textChanged();
  //  emit documentTitleChanged();

  //  reset();
  //}
  //}
  // emit fileUrlChanged();
  //}
}
