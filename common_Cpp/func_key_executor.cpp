#include <QTimer>
#include "func_key_executor.h"
#include "log.h"

FuncKeyExecutor::FuncKeyExecutor(QObject *parent) : QObject(parent)
{

}

void FuncKeyExecutor::start(int intervalMs)
{
    set_intervalMs(intervalMs);
}

void FuncKeyExecutor::addFunc(const QString &key, const std::function<void ()> &func)
{
    if(!functionHash.contains(key))
    {
        QTimer::singleShot(get_intervalMs(), this, [this, key]() {
            logDebug() << "exec func for key" << key;
            functionHash[key]();
            functionHash.remove(key);
        });
    }
    functionHash[key] = func;
}
