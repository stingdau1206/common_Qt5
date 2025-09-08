#include "async_task.h"

AsyncTask::AsyncTask(AsyncTask::TaskFunction task, QObject *parent)
    : QObject(parent), m_task(std::move(task))
{
    // Start the task asynchronously
    m_future = QtConcurrent::run(m_task);

    // Connect the futureWatcher to handle completion
    connect(&m_futureWatcher, &QFutureWatcher<void>::finished, this, &AsyncTask::onTaskFinished);

    // Set the future to the watcher
    m_futureWatcher.setFuture(m_future);
}

void AsyncTask::run(AsyncTask::TaskFunction task)
{
    new AsyncTask(task);
}

void AsyncTask::onTaskFinished()
{
    qDebug() << "Task completed!";
    // Cleanup: delete this object when the task is finished
    deleteLater();
}
