#ifndef MARCO_H
#define MARCO_H

#include <QCoreApplication>
#include <QJsonObject>
#include <QJsonArray>
#include <QMutex>
#include <QMutexLocker>
#include <QQueue>

#define APP_FOLDER              QCoreApplication::instance()->applicationDirPath().append("/")

#define MIN_TO_MS(a) qint64(60000*a)
#define MIN_TO_S(a) qint64(60*a)
#define DAY_TO_MIN(a) qint64(24*60*a)

#define SWAP(a,b) {     \
    auto temp = a;      \
    a = b;              \
    b = temp;           \
}

#define RETURN_IF_STRING_EMPTY(A, B) ((A).isEmpty() ? (B) : (A))

#define QVariable(type, var_name) \
    private: \
    type m_##var_name; \
    public:\
    type get_##var_name() {\
    return m_##var_name;\
    }\
    void set_##var_name(const type &value) {\
     m_##var_name = value;\
    }

#define QListVariable(type, var_name) \
private: \
    QList<type> m_##var_name; \
public: \
    const QList<type>& get_##var_name() const { \
        return m_##var_name; \
    } \
    void set_##var_name(const QList<type> &list) { \
        if (m_##var_name == list) return; \
        m_##var_name = list; \
    } \
    void add_##var_name##_element(const type &value) { \
        m_##var_name.append(value); \
    } \
    void update_##var_name##_element(int index, const type &value) { \
        if (index < 0 || index >= m_##var_name.size() || m_##var_name.at(index) == value) { \
            return; \
        } \
        m_##var_name[index] = value; \
    } \
    void remove_##var_name##_element(int index) { \
        if (index < 0 || index >= m_##var_name.size()) { \
            return; \
        } \
        m_##var_name.removeAt(index); \
    } \
    void clear_##var_name() { \
        m_##var_name.clear(); \
    }

#define QHashVariable(type_key, type_value, var_name) \
    private: \
    QHash<type_key, type_value> m_##var_name; \
    public: \
    void update_##var_name##_element(const type_key &key, const type_value &value) \
    { \
        m_##var_name.insert(key, value); \
    } \
    void remove_##var_name##_element(const type_key &key) \
    { \
        m_##var_name.remove(key); \
    } \
    type_value get_##var_name##_value(const type_key &key) \
    { \
        return m_##var_name.value(key); \
    } \

#define QQueueVariable(type, name) \
private: \
    QQueue<type> m_##name##Queue; \
    mutable QMutex m_##name##Mutex; \
public: \
    /** \
     * @brief Thêm một phần tử vào cuối hàng đợi một cách an toàn. \
     */ \
    void push_##name(const type &element) \
    { \
        QMutexLocker locker(&m_##name##Mutex); \
        m_##name##Queue.enqueue(element); \
    } \
    /** \
     * @brief Lấy và xóa một phần tử từ đầu hàng đợi một cách an toàn. \
     * @return Trả về một đối tượng của 'type'. Nếu hàng đợi rỗng, \
     * trả về một đối tượng được khởi tạo mặc định. \
     */ \
    type pull_##name() \
    { \
        QMutexLocker locker(&m_##name##Mutex); \
        if (m_##name##Queue.isEmpty()) { \
            return type(); /* Trả về giá trị mặc định nếu rỗng */ \
        } \
        return m_##name##Queue.dequeue(); \
    } \
    /** \
     * @brief Kiểm tra xem hàng đợi có rỗng không một cách an toàn. \
     */ \
    bool is_##name##_empty() const \
    { \
        QMutexLocker locker(&m_##name##Mutex); \
        return m_##name##Queue.isEmpty(); \
    } \
    /** \
     * @brief Lấy số lượng phần tử trong hàng đợi một cách an toàn. \
     */ \
    int get_##name##_size() const \
    { \
        QMutexLocker locker(&m_##name##Mutex); \
        return m_##name##Queue.size(); \
    }

#define QStaticVariable(type, var_name) \
    private: \
    static type m_##var_name; \
    public:\
    static type &get_##var_name() {\
    return m_##var_name;\
    }\
    static void set_##var_name(type value) {\
     m_##var_name = value;\
    }

#define MyQ_PROPERTY(type, var_name) \
    Q_PROPERTY(type var_name READ get_##var_name WRITE set_##var_name NOTIFY var_name##Changed) \
private: \
    type m_##var_name; \
public: \
    type get_##var_name() const { return m_##var_name; } \
    void set_##var_name(type value, bool is_duplicate = false) { \
        if (m_##var_name != value || is_duplicate) { \
            m_##var_name = value; \
            emit var_name##Changed(); \
        } \
    } \
Q_SIGNAL \
    void var_name##Changed();

#define MyQ_PROPERTY_CONST(type, var_name) \
    Q_PROPERTY(type var_name READ get_##var_name CONSTANT) \
private: \
    type m_##var_name; \
public: \
    type get_##var_name() const { return m_##var_name; }

#define MyListQ_PROPERTY(type, var_name) \
    MyQ_PROPERTY(QList<type>, var_name) \
public: \
    Q_INVOKABLE void append_##var_name(type value) { \
        m_##var_name.append(value); \
        emit var_name##Changed(); \
    } \
    Q_INVOKABLE void remove_##var_name(type value) { \
        m_##var_name.removeOne(value); \
        emit var_name##Changed(); \
    }

#define MyVectorQ_PROPERTY(type, var_name) \
    MyQ_PROPERTY(QVector<type>, var_name) \
public: \
    Q_INVOKABLE void append_##var_name(type value) { \
        m_##var_name.append(value); \
        emit var_name##Changed(); \
    } \
    Q_INVOKABLE void remove_##var_name(type value) { \
        m_##var_name.removeOne(value); \
        emit var_name##Changed(); \
    } \
    Q_INVOKABLE void change_##var_name(int index, type value) { \
        m_##var_name[index] = value; \
        emit var_name##Changed(); \
    } \

#endif // MARCO_H
