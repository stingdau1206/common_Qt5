#include "common_func_string.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QRegularExpression>
#include <QCryptographicHash>

QString getAlphanumeric(const QString& input) {
    // Regular expression to match only alphanumeric characters (numbers and letters)
    QRegExp re("[^a-zA-Z0-9]");
    // Replace all non-alphanumeric characters with an empty string
    QString result = input;
    result.remove(re);
    return result;
}

QString extract_substring_between_char(const QString& str, QChar startChar, int startAppearance, QChar endChar, int endAppearance)
{
    int startIndex = -1;
    int endIndex = -1;

    // Find the start index
    for (int i = 0; i <= startAppearance; ++i) {
        startIndex = str.indexOf(startChar, startIndex + 1);
        if (startIndex == -1)
            break;
    }

    // Find the end index
    for (int i = 0; i <= endAppearance; ++i) {
        endIndex = str.indexOf(endChar, endIndex + 1);
        if (endIndex == -1)
            break;
    }

    if (startIndex != -1 && endIndex != -1 && endIndex > startIndex) {
        startIndex++; // Move past the startChar
        return str.mid(startIndex, endIndex - startIndex);
    }

    return QString(); // Return an empty string if the special characters or appearances are not found or in the wrong order
}

QString preend_char(const QString &input, const int &limit_length, const QChar &char_preend)
{
    return input.rightJustified(limit_length, char_preend);
}

QString extractIPFromRTSP(const QString &rtspUrl) {
    // This regex handles both cases with and without username:password@
    QRegularExpression re(R"((\w+://)(?:[a-zA-Z0-9._-]+:[a-zA-Z0-9._-]+@)?((\d{1,3}\.){3}\d{1,3})(?::\d+)?(/.*)?)");
    QRegularExpressionMatch match = re.match(rtspUrl);

    if (match.hasMatch()) {
        // Capture group 2 contains the IP address
        return match.captured(2);
    }

    return QString();
}

bool isNumeric(const QString &str) {
    QRegularExpression re("^\\d+$"); // Regular expression to match one or more digits
    QRegularExpressionMatch match = re.match(str);
    return match.hasMatch();
}

QString filterAlphanumeric(QString input) {
    QRegularExpression regex("[^a-zA-Z0-9]");
    return input.remove(regex);
}

QString hashPassword(const QString &password) {
    QByteArray data = password.toUtf8();
    QByteArray hash = QCryptographicHash::hash(data, QCryptographicHash::Sha256);
    return hash.toHex();  // trả về dạng chuỗi hex dễ lưu trữ
}
