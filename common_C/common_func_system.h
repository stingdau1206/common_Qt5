#ifndef COMMON_FUNC_SYSTEM_CPP
#define COMMON_FUNC_SYSTEM_CPP

#include <QObject>

qint64 get_current_unix_time_ms();

qint64 get_current_unix_time_s();

QString get_current_datetime_with_format(const QString &format = "hh:mm:ss dd-MM-yyyy");

#endif // COMMON_FUNC_SYSTEM_CPP
