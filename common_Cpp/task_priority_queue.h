#ifndef TASKPRIORITYQUEUE_H
#define TASKPRIORITYQUEUE_H

#include <QObject>
#include <queue>
#include <QMutex>
#include <QWaitCondition>
// Cấu trúc của một tác vụ


class TaskPriorityQueue : public QObject
{
    Q_OBJECT
public:

    enum Priority {
        Low = 0,
        Normal = 1,
        High = 2
    };
    Q_ENUMS(Priority)

    struct CallableTask {
        quint64 id;
        Priority priority;
        std::function<QVariant()> command; // THAY ĐỔI: Chứa lambda hoặc function

        // Nạp chồng toán tử so sánh để std::priority_queue biết cách sắp xếp.
        // Lưu ý: std::priority_queue là max-heap, nên toán tử '<' sẽ đẩy
        // giá trị "lớn hơn" (priority cao hơn) lên trên cùng.
        bool operator<(const CallableTask& other) const {
            return priority < other.priority;
        }
    };

    explicit TaskPriorityQueue(QObject *parent = nullptr);

    void push(const CallableTask& task);

    // Hàm này sẽ block cho đến khi có task mới
    CallableTask waitAndPop();

private:
    mutable QMutex m_mutex;
    QWaitCondition m_condition;
    std::priority_queue<CallableTask> m_queue;
};

#endif // TASKPRIORITYQUEUE_H
