#ifndef ASYNCWATCHER_H
#define ASYNCWATCHER_H

#include <QObject>
#include <QtConcurrent>
#include <QFutureWatcher>

template <typename T>
class AsyncWatcher : public QObject
{

public:
    using ResultHandler = std::function<void(const T&)>;
    using TaskFunction = std::function<T()>;

    explicit AsyncWatcher(const TaskFunction& taskFunc, const ResultHandler& resultHandler, QObject *parent = nullptr)
        : QObject(parent)
    {
        QFuture<T> future = QtConcurrent::run(taskFunc);
        auto* watcher = new QFutureWatcher<T>(this);

        QObject::connect(watcher, &QFutureWatcher<T>::finished, this, [=]() {
            T result = future.result();
            resultHandler(result);
            watcher->deleteLater();
            this->deleteLater();
        });

        watcher->setFuture(future);
    }

    static void run(const TaskFunction& taskFunc, const ResultHandler& resultHandler) {
        new AsyncWatcher(taskFunc, resultHandler);
    }

signals:

};

#endif // ASYNCWATCHER_H
