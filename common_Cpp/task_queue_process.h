#ifndef TASK_QUEUE_PROCESS_H
#define TASK_QUEUE_PROCESS_H

#include <QObject>
#include <QQueue>
#include <QTimer>
#include <functional>

class TaskQueueProcess : public QObject
{
    Q_OBJECT

public:
    explicit TaskQueueProcess(QObject *parent = nullptr);

    /**
     * @brief Đặt khoảng thời gian nghỉ (ms) giữa mỗi task.
     */
    void setTaskInterval(int intervalMs);

    /**
     * @brief Thêm một task mới vào hàng đợi.
     * @param task Một std::function chứa công việc cần làm.
     */
    void addTask(const std::function<void()>& task);
    void addTask(std::function<void()>&& task);

    /**
     * @brief Bắt đầu xử lý các task trong hàng đợi.
     */
    void process();

signals:
    /**
     * @brief Signal được phát ra khi hàng đợi bắt đầu được xử lý.
     */
    void processingStarted();

    /**
     * @brief Signal được phát ra khi tất cả các task trong hàng đợi đã được xử lý xong.
     */
    void allTasksFinished();

private slots:
    void processNextTask();

private:
    QQueue<std::function<void()>> m_taskQueue;
    QTimer m_taskTimer;
};

#endif // TASK_QUEUE_PROCESS_H
