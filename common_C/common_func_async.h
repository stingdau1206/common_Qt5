#ifndef COMMON_FUNC_ASYNC_H
#define COMMON_FUNC_ASYNC_H

#include <QtConcurrent>

template<typename Func>
QFuture<void> runFuncInAnotherThread(Func func) {
    // QtConcurrent::run sẽ lấy hàm 'func' và chạy nó trên một thread
    // từ QThreadPool toàn cục. Nó sẽ trả về ngay lập tức.
    return QtConcurrent::run(func);
}

template <typename Func>
void runFuncAfterDelay(Func&& func, int delayMs, QObject* context = nullptr)
{
    QTimer::singleShot(delayMs, context, std::forward<Func>(func));
}

template <typename T, typename Worker, typename Callback>
void runConcurrenWatcher(Worker&& worker, Callback&& onFinished, QObject* parent = nullptr) {
    // chạy worker ở thread pool
    auto future = QtConcurrent::run(std::forward<Worker>(worker));

    // watcher sống theo parent, tự hủy khi xong
    auto *watcher = new QFutureWatcher<T>(parent);

    QObject::connect(watcher, &QFutureWatcher<T>::finished, parent, [watcher, onFinished]() {
        // lấy kết quả (đang ở main thread vì signal/slot)
        const T result = watcher->future().result();
        // gọi callback UI
        onFinished(result);
        watcher->deleteLater();
    }, Qt::QueuedConnection);

    watcher->setFuture(future);
}

#endif // COMMON_FUNC_ASYNC_H
