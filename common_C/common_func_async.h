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

#endif // COMMON_FUNC_ASYNC_H
