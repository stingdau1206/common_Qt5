#include <QObject>
#include <QtConcurrent>
#include <QFuture>
#include <QFutureWatcher>
#include <QTimer>
#include <QDebug>
#include <functional>

class AsyncTask : public QObject
{
    Q_OBJECT

public:
    using TaskFunction = std::function<void()>;

    // Constructor
    AsyncTask(TaskFunction task, QObject *parent = nullptr);
    static void run(TaskFunction task);
private slots:
    void onTaskFinished();

private:
    QFuture<void> m_future;
    QFutureWatcher<void> m_futureWatcher; // Not a pointer
    TaskFunction m_task;
};
