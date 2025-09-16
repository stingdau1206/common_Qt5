#include "common_func_file.h"
#include <QtDebug>


QString getFileNameWithoutExt(const QString &filePath)
{
    QFileInfo info(filePath);
    return info.completeBaseName(); // name without extension
}

bool deleteFile(const QString &filePath) {
    // Create a QFile object with the provided file path
    QFile file(filePath);

    // Check if the file exists before attempting to delete
    if (!file.exists()) {
        qWarning() << "File does not exist:" << filePath;
        return false; // Or true, depending on desired behavior for non-existent files
    }

    // Attempt to remove the file
    if (file.remove()) {
        qDebug() << "Successfully deleted file:" << filePath;
        return true;
    } else {
        // If removal fails, print the error message
        qWarning() << "Failed to delete file:" << filePath;
        qWarning() << "Error:" << file.errorString();
        return false;
    }
}

QByteArray readFile(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly))
        return QByteArray(); // trả về rỗng nếu mở file thất bại

    return file.readAll(); // đọc toàn bộ nội dung
}
