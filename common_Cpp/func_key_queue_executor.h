#ifndef FUNCKEYQUEUEEXECUTOR_H
#define FUNCKEYQUEUEEXECUTOR_H

#include <QObject>
#include <QTimer>
#include <QHash>
#include <functional>
#include <QMutex>
#include "macro.h"

class FuncKeyQueueExecutor : public QObject
{
    QVariable(int , intervalMs)
    Q_OBJECT
public:
    explicit FuncKeyQueueExecutor(QObject *parent = nullptr);

    void start(int intervalMs);
    void addFunc(const QString &key, const std::function<void()>& func);
    bool isEmpty();

private slots:
    void executeNext();

private:
    QHash<QString, std::function<void()>> functionHash;  // Vector to track keys and functions
    QTimer exec_timer;
};

#endif // FUNCKEYQUEUEEXECUTOR_H
