#include <QCoreApplication>
#include <QHash>
#include <QString>
#include <QDebug>
#include <QVector>
#include <QtConcurrent>
#include <queue>
#include <QElapsedTimer>
#include <algorithm>
#include <QSet>
#include <QRandomGenerator>

// Struct để lưu kết quả, có thể để bên ngoài hoặc lồng vào class
struct SimilarityResult {
    QString key;
    double score;
    bool operator>(const SimilarityResult& other) const { return score > other.score; }
};

// ======================================================================
//   LỚP FUZZY SEARCH INDEX HOÀN CHỈNH
// ======================================================================
class FuzzySearchIndex
{
public:
    FuzzySearchIndex(int ngramSize = 3) : m_ngramSize(ngramSize) {}

    // --- Giao diện Public ---

    // 1. Xây dựng chỉ mục từ một danh sách các chuỗi (elements)
    void buildIndex(const QVector<QString>& elements);

    // 2. Thêm một phần tử mới vào chỉ mục
    void addElement(const QString& element);

    // 3. Xóa một phần tử khỏi chỉ mục (xóa mềm)
    void removeElement(const QString& element);

    // 4. Hàm tìm kiếm chính
    QVector<QPair<QString, double>> findTopNSimilar(const QString& input,
        int N,
        double thresholdScore = 50,
        double maxCpuUsageRatio = 0.5
    ) const;


private:
    // --- Các phương thức Private và biến thành viên ---

    QVector<QString> findTopCandidates(const QString& input, int numCandidates) const;

    void updateNgramsForElement(int elementIndex, const QString& element);

    QSet<QString> getNgrams(const QString& text) const;

    static QString normalize(const QString &text);

    static double calculateSingleSimilarity(const QString &shortStr, const QString &longStr);

    int m_ngramSize;
    QHash<QString, QVector<int>> m_ngramIndex;
    QVector<QString> m_originalElements;
    QHash<QString, int> m_elementToIndex;
};
