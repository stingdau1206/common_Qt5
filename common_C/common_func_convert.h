#ifndef COMMON_FUNC_CONVERT_H
#define COMMON_FUNC_CONVERT_H

#include <QDateTime>
#include <QJsonObject>

qint64 convertDateTimeStringToMsEpoch(const QString& dateTimeString, const QString &format = "hh:mm:ss dd-MM-yyyy");

QString convertEpochMsToDateTimeString(qint64 epoch_ms, const QString &format = "hh:mm:ss dd-MM-yyyy");

QString convertEpochMsToDuration(qint64 epochMs);

QString convertNumberToStringWithDots(int number);

QJsonObject convertQByteArrayToQjsonObject(const QByteArray &input);

QByteArray convertHashToByteArray(const QVariantHash &hash);

QVariantHash convertByteArrayToHash(const QByteArray &data);

#endif // COMMON_FUNC_CONVERT_H
