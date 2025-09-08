#include "process_each_element.h"

ProcessEachElement::ProcessEachElement(const QHash<QString, QVariant> &hash, std::function<void (const QString &, const QVariant &)> processFunc, std::function<void()> stopFunc, int period, QObject *parent)
    : QObject(parent), m_hash(hash), m_iterator(m_hash.begin()), m_processFunc(processFunc), m_stopFunc(stopFunc), m_period(period)
{
    startProcessing();
}

void ProcessEachElement::set(const QHash<QString, QVariant> &hash, std::function<void (const QString &, const QVariant &)> processFunc, std::function<void()> stopFunc, int period, QObject *parent)
{
    new ProcessEachElement(hash, processFunc, stopFunc, period, parent);
}

void ProcessEachElement::startProcessing() {
    connect(&m_timer, &QTimer::timeout, this, &ProcessEachElement::processNextElement);
    m_timer.start(m_period);
}

void ProcessEachElement::processNextElement() {
    if (m_iterator != m_hash.end()) {
        m_processFunc(m_iterator.key(), m_iterator.value());
        ++m_iterator;
    } else {
        m_timer.stop();
        m_stopFunc();
        this->deleteLater();
    }
}
