#ifndef FUNCQUEUEEXECUTOR_H
#define FUNCQUEUEEXECUTOR_H

#include <QObject>
#include <QQueue>
#include <QTimer>
#include <functional>

class FuncQueueExecutor : public QObject {
    Q_OBJECT

public:
    FuncQueueExecutor(QObject* parent = nullptr);

    void start(int intervalMs);
    void stop();
    void addFunc(const std::function<void()>& func);

private slots:
    void executeNext();

private:
    QQueue<std::function<void()>> m_queue;
    QTimer m_timer;
    int m_intervalMs;
};

#endif // FUNCQUEUEEXECUTOR_H
