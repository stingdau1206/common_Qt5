#include "log.h"

#include <QDateTime>
#include <QFile>
#include <QDir>
#include <QLoggingCategory>

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QDateTime currentDate = QDateTime::currentDateTime();

    // Define the log file name with the path
    QString logFileName = QString("logs/app_%1.log").arg(currentDate.toString("dd-MM-yyyy"));

    // Open the log file
    QFile outFile(logFileName);
    if (outFile.open(QIODevice::WriteOnly | QIODevice::Append)) {
        QTextStream ts(&outFile);
        // Include milliseconds in the timestamp
        ts << "[" << currentDate.toString("hh:mm:ss.zzz") << "] " << msg << Qt::endl;
    }
}

void log_init()
{

}

void setLogLevel(LogLevel level)
{
    int threshold = static_cast<int>(level);

    QStringList rules;
    for (int i = 0; i <= static_cast<int>(LogLevel::Fatal); ++i) {
        bool enabled = i >= threshold;

        QString levelName;
        switch (static_cast<LogLevel>(i)) {
        case LogLevel::Debug:    levelName = "debug";    break;
        case LogLevel::Info:     levelName = "info";     break;
        case LogLevel::Warning:  levelName = "warning";  break;
        case LogLevel::Critical: levelName = "critical"; break;
        case LogLevel::Fatal:    levelName = "fatal";    break;
        }

        rules << QString("*.%1=%2").arg(levelName).arg(enabled ? "true" : "false");
    }

    QLoggingCategory::setFilterRules(rules.join('\n'));
}
