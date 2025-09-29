#ifndef COMMON_FUNC_FILE_H
#define COMMON_FUNC_FILE_H

#include <QFileInfo>

QString getFileNameWithoutExt(const QString &filePath);

bool deleteFile(const QString &filePath);

QByteArray readFile(const QString &filePath);

bool pathIsExist(const QString &filePath);

#endif // COMMON_FUNC_FILE_H
