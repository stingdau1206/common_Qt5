#include "task_priority_queue.h"

TaskPriorityQueue::TaskPriorityQueue(QObject *parent) : QObject(parent)
{

}

void TaskPriorityQueue::push(const CallableTask &task) {
    QMutexLocker locker(&m_mutex);
    m_queue.push(task);
    // Đánh thức một luồng đang chờ (nếu có)
    m_condition.wakeOne();
}

TaskPriorityQueue::CallableTask TaskPriorityQueue::waitAndPop() {
    QMutexLocker locker(&m_mutex);
    // Chờ trong khi queue rỗng để tránh lãng phí CPU
    while (m_queue.empty()) {
        m_condition.wait(&m_mutex);
    }

    CallableTask task = m_queue.top();
    m_queue.pop();
    return task;
}
