#include <QtDebug>
#include "func_key_queue_executor.h"
#include "common_func_convert.h"
#include "async_task.h"

FuncKeyQueueExecutor::FuncKeyQueueExecutor(QObject *parent) : QObject(parent)
{
}

void FuncKeyQueueExecutor::start(int intervalMs)
{
    set_intervalMs(intervalMs);
    connect(&exec_timer, &QTimer::timeout, this, &FuncKeyQueueExecutor::executeNext);
    exec_timer.start(get_intervalMs());
}

void FuncKeyQueueExecutor::addFunc(const QString &key, const std::function<void ()> &func)
{
    functionHash[key] = func;
}

bool FuncKeyQueueExecutor::isEmpty()
{
    return functionHash.isEmpty();
}

void FuncKeyQueueExecutor::executeNext()
{
    if(isEmpty())
    {
        return;
    }
    QString firstKey = functionHash.begin().key();
    qDebug() << "Executing function for key:" << firstKey;
    functionHash[firstKey]();       // Execute the function
    functionHash.remove(firstKey); // Remove the function from the hash
}
