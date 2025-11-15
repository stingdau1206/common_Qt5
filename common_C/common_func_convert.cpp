#include "common_func_convert.h"
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDataStream>
#include <QBuffer>
#include <QImage>
#include <QJsonArray>

QString convertEpochMsToDateTimeString(qint64 epoch_ms, const QString &format){
    return QDateTime::fromMSecsSinceEpoch(epoch_ms).toString(format);
}

QString convertEpochMsToDuration(qint64 epochMs) {
    if(epochMs <= 0)
    {
        return QString();
    }
    // Convert to total minutes
    qint64 totalMinutes = epochMs / (1000 * 60);

    qint64 days = totalMinutes / (60 * 24);
    qint64 hours = (totalMinutes % (60 * 24)) / 60;
    qint64 minutes = totalMinutes % 60;

    return QString("%1 ngày %2 giờ %3 phút").arg(days).arg(hours).arg(minutes);
}

qint64 dateTimeStringToMsEpoch(const QString& dateTimeString, const QString &format) {
    QDateTime dateTime = QDateTime::fromString(dateTimeString, format);

    // Kiểm tra xem việc chuyển đổi có thành công không
    if (!dateTime.isValid()) {
        qWarning() << "Lỗi: Chuỗi thời gian không hợp lệ -" << dateTimeString;
        return -1;
    }

    // 3. Chuyển đối tượng QDateTime sang mili giây epoch và trả về
    return dateTime.toMSecsSinceEpoch();
}

QString convertNumberToStringWithDots(int number) {
    QString numStr = QString::number(number);
    QString result;
    int count = 0;

    // Iterate from the end of the string to the beginning
    for (int i = numStr.length() - 1; i >= 0; --i) {
        result.prepend(numStr[i]);
        count++;
        // Insert a dot every three digits, except at the very end
        if (count % 3 == 0 && i != 0) {
            result.prepend('.');
        }
    }
    return result;
}

QJsonObject convertQByteArrayToQJsonobject(const QByteArray &input)
{
    return QJsonDocument::fromJson(input).object();
}

qint64 convertDateTimeStringToMsEpoch(const QString &dateTimeString, const QString &format)
{
    return QDateTime::fromString(dateTimeString, format).toMSecsSinceEpoch();
}

QJsonObject convertQByteArrayToQjsonObject(const QByteArray &input)
{
    return QJsonDocument::fromJson(input).object();
}

QByteArray convertHashToByteArray(const QVariantHash &hash)
{
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream << hash;  // QVariantHash is streamable
    return data;
}

QVariantHash convertByteArrayToHash(const QByteArray &data)
{
    QVariantHash hash;
    QDataStream stream(data);
    stream >> hash;  // Deserialize back to QVariantHash
    return hash;
}

QByteArray convertQImageToQByteArray(const QImage &image, const char *format)
{
    QByteArray imageData;
    QBuffer buffer(&imageData);
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, format);

    return imageData;
}

QByteArray convertQJsonObjectToQByteArray(const QJsonObject &input)
{
    return QJsonDocument(input).toJson(QJsonDocument::Compact);
}

QStringList convertQJsonArrayToQStringList(const QJsonArray &jsonArray)
{
    QStringList stringList;
    for (const QJsonValue& value : jsonArray) {
        if (value.isString()) {
            stringList.append(value.toString());
        }
    }
    return stringList;
}
