#ifndef STRING_COMMON_FUNC_H
#define STRING_COMMON_FUNC_H

#include <QCoreApplication>

QString extract_substring_between_char(const QString& str, const QChar &startChar, int startAppearance, const QChar &endChar, int endAppearance);

QString preend_char(const QString &input, const int &limit_length, const QChar &char_preend);

QString extractIPFromRTSP(const QString& rtspUrl);

bool isNumeric(const QString& str);

QString getAlphanumeric(const QString& input);

QString filterAlphanumeric(QString input);

QByteArray hashPassword(const QString& password);

#endif // STRING_COMMON_FUNC_H
