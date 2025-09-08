#ifndef FUNCKEYEXECUTOR_H
#define FUNCKEYEXECUTOR_H

#include <QObject>
#include <functional>
#include <QHash>
#include "macro.h"

class FuncKeyExecutor : public QObject
{
    QVariable(int , intervalMs)
    Q_OBJECT
public:
    explicit FuncKeyExecutor(QObject *parent = nullptr);

    void start(int intervalMs);
    void addFunc(const QString &key, const std::function<void()>& func);
signals:

private:
    QHash<QString, std::function<void()>> functionHash;  // Vector to track keys and functions
};

#endif // FUNCKEYEXECUTOR_H
