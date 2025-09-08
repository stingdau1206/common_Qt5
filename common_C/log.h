#ifndef LOG_H
#define LOG_H

#include <QtDebug>

enum class LogLevel {
    Debug,
    Info,
    Warning,
    Critical,
    Fatal
};

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg);

void log_init();
void setLogLevel(LogLevel level);

#endif // LOG_H
