#include "task_queue_process.h"

TaskQueueProcess::TaskQueueProcess(QObject *parent)
    : QObject(parent)
{
    m_taskTimer.setSingleShot(true);
    connect(&m_taskTimer, &QTimer::timeout, this, &TaskQueueProcess::processNextTask);

    // Mặc định khoảng nghỉ là 500ms
    setTaskInterval(500);
}

void TaskQueueProcess::setTaskInterval(int intervalMs)
{
    m_taskTimer.setInterval(intervalMs > 0 ? intervalMs : 0);
}

void TaskQueueProcess::addTask(const std::function<void()>& task)
{
    m_taskQueue.enqueue(task);
}

void TaskQueueProcess::addTask(std::function<void()>&& task)
{
    m_taskQueue.enqueue(std::move(task));
}

void TaskQueueProcess::process()
{
    if (m_taskQueue.isEmpty()) {
        emit allTasksFinished();
    } else {
        QTimer::singleShot(0, this, &TaskQueueProcess::processNextTask);
    }
}

void TaskQueueProcess::processNextTask()
{
    if (m_taskQueue.isEmpty()) {
        emit allTasksFinished();
        return;
    }

     emit processingStarted();

    std::function<void()> task = m_taskQueue.dequeue();
    task();

    if (!m_taskQueue.isEmpty()) {
        m_taskTimer.start();
    } else {
        emit allTasksFinished();
    }
}
