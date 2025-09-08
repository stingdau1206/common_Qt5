#include "func_queue_executor.h"

FuncQueueExecutor::FuncQueueExecutor(QObject *parent)
    : QObject(parent){
    connect(&m_timer, &QTimer::timeout, this, &FuncQueueExecutor::executeNext);
}

void FuncQueueExecutor::start(int intervalMs) {
    m_intervalMs = intervalMs;
    m_timer.setInterval(intervalMs);
    m_timer.start();
}

void FuncQueueExecutor::stop() {
    m_timer.stop();
}

void FuncQueueExecutor::addFunc(const std::function<void ()> &func) {
    m_queue.enqueue(func);
}

void FuncQueueExecutor::executeNext() {
    if (!m_queue.isEmpty()) {
        auto func = m_queue.dequeue();
        if (func) {
            func();
        }
    }
}
