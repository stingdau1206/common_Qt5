#ifndef PROCESSEACHELEMENT_H
#define PROCESSEACHELEMENT_H

#include <QObject>
#include <QHash>
#include <QVariant>
#include <QTimer>

class ProcessEachElement : public QObject
{
    Q_OBJECT
public:
    static void set(const QHash<QString, QVariant>& hash, std::function<void(const QString&, const QVariant&)> processFunc, std::function<void()> stopFunc, int period = 500, QObject *parent = nullptr);
private:
    explicit ProcessEachElement(const QHash<QString, QVariant>& hash, std::function<void(const QString&, const QVariant&)> processFunc, std::function<void()> stopFunc, int period = 500, QObject *parent = nullptr);

    void startProcessing();
signals:

private slots:
    void processNextElement();
private:
    QHash<QString, QVariant> m_hash;
    QHash<QString, QVariant>::iterator m_iterator;
    std::function<void(const QString&, const QVariant&)> m_processFunc;
    std::function<void()> m_stopFunc;

    int m_period;
    QTimer m_timer;

};

#endif // PROCESSEACHELEMENT_H
