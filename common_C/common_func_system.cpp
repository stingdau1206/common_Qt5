#include <QDateTime>
#include <QtDebug>
#include "common_func_system.h"

qint64 get_current_unix_time_ms()
{
    return QDateTime::currentDateTime().toMSecsSinceEpoch();
}

qint64 get_current_unix_time_s()
{
    return QDateTime::currentDateTime().toSecsSinceEpoch();
}

QString get_current_datetime_with_format(const QString &format)
{
    return QDateTime::currentDateTime().toString(format);
}



