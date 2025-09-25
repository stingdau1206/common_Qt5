#include "fuzzysearch_index.h"

void FuzzySearchIndex::buildIndex(const QList<QString> &elements) {
    m_ngramIndex.clear();
    m_originalElements = elements;
    m_elementToIndex.clear();

    for (int i = 0; i < m_originalElements.size(); ++i) {
        const QString& element = m_originalElements[i];
        if (!element.isEmpty()) {
            m_elementToIndex[element] = i;
            updateNgramsForElement(i, element);
        }
    }
}

void FuzzySearchIndex::addElement(const QString &element) {
    if (m_elementToIndex.contains(element) || element.isEmpty()) return;

    m_originalElements.append(element);
    int newElementIndex = m_originalElements.size() - 1;
    m_elementToIndex[element] = newElementIndex;
    updateNgramsForElement(newElementIndex, element);
}

void FuzzySearchIndex::removeElement(const QString &element) {
    if (!m_elementToIndex.contains(element)) return;

    int elementIndexToRemove = m_elementToIndex.value(element);
    QString normalized = normalize(element);
    QSet<QString> ngrams = getNgrams(normalized);
    for (const QString& ngram : ngrams) {
        if (m_ngramIndex.contains(ngram)) {
            m_ngramIndex[ngram].removeOne(elementIndexToRemove);
        }
    }
    m_originalElements[elementIndexToRemove] = QString();
    m_elementToIndex.remove(element);
}

QList<QPair<QString, double> > FuzzySearchIndex::findTopNSimilar(const QString &input, int N, double thresholdScore, double maxCpuUsageRatio) const
{
    if (N <= 0) return {};

    QList<QString> candidates = findTopCandidates(input, 20*N);

    if (candidates.isEmpty()) return {};

    QString normalizedA = normalize(input);
    QThreadPool customPool;
    double clampedRatio = qBound(0.0, maxCpuUsageRatio, 1.0);
    int threadsToUse = qMax(1, static_cast<int>(QThread::idealThreadCount() * clampedRatio));
    customPool.setMaxThreadCount(threadsToUse);

    QList<QFuture<SimilarityResult>> futures;
    for (const QString& key : candidates) {
        auto computeTask = [=]() -> SimilarityResult {
            QString normalizedKey = normalize(key);
            const QString& shortStr = (normalizedA.length() < normalizedKey.length()) ? normalizedA : normalizedKey;
            const QString& longStr = (normalizedA.length() < normalizedKey.length()) ? normalizedKey : normalizedA;
            double score = calculateSingleSimilarity(shortStr, longStr);
            return {key, score};
        };
        futures.append(QtConcurrent::run(&customPool, computeTask));
    }

    QList<SimilarityResult> allResults;
    for (auto& future : futures) {
        future.waitForFinished();
        allResults.append(future.result());
    }

    std::priority_queue<SimilarityResult, std::vector<SimilarityResult>, std::greater<SimilarityResult>> minHeap;
    for (const auto& result : allResults) {
        if (minHeap.size() < N) {
            minHeap.push(result);
        } else if (result.score > minHeap.top().score) {
            minHeap.pop();
            minHeap.push(result);
        }
    }

    QList<QPair<QString, double>> topNResults;
    topNResults.reserve(minHeap.size());
    while (!minHeap.empty()) {
        if (minHeap.top().score >= thresholdScore) {
                        topNResults.append({minHeap.top().key, minHeap.top().score});
                    }
        minHeap.pop();
    }
    std::sort(topNResults.begin(), topNResults.end(), [](const QPair<QString, double>& a, const QPair<QString, double>& b){
        return a.second > b.second;
    });

    return topNResults;
}

QList<QString> FuzzySearchIndex::findTopCandidates(const QString &input, int numCandidates) const {
    if (input.isEmpty() || m_ngramIndex.isEmpty()) return {};

    QHash<int, int> candidateScores;
    QSet<QString> inputNgrams = getNgrams(normalize(input));

    for (const QString& ngram : inputNgrams) {
        auto it = m_ngramIndex.find(ngram);
        if (it != m_ngramIndex.end()) {
            for (int elementIndex : it.value()) {
                candidateScores[elementIndex]++;
            }
        }
    }

    using ScorePair = QPair<int, int>;
    std::priority_queue<ScorePair, QList<ScorePair>, std::greater<ScorePair>> minHeap;
    for (auto it = candidateScores.constBegin(); it != candidateScores.constEnd(); ++it) {
        int elementIndex = it.key();
        if (m_originalElements.at(elementIndex).isEmpty()) continue;
        int score = it.value();
        if (minHeap.size() < numCandidates) {
            minHeap.push({score, elementIndex});
        } else if (score > minHeap.top().first) {
            minHeap.pop();
            minHeap.push({score, elementIndex});
        }
    }

    QList<QString> topCandidates;
    topCandidates.reserve(minHeap.size());
    while (!minHeap.empty()) {
        topCandidates.append(m_originalElements.at(minHeap.top().second));
        minHeap.pop();
    }
    return topCandidates;
}

void FuzzySearchIndex::updateNgramsForElement(int elementIndex, const QString &element) {
    QSet<QString> ngrams = getNgrams(normalize(element));
    for (const QString& ngram : ngrams) {
        m_ngramIndex[ngram].append(elementIndex);
    }
}

QSet<QString> FuzzySearchIndex::getNgrams(const QString &text) const {
    QSet<QString> ngrams;
    if (text.length() < m_ngramSize) {
        if (!text.isEmpty()) ngrams.insert(text);
        return ngrams;
    }
    for (int i = 0; i <= text.length() - m_ngramSize; ++i) {
        ngrams.insert(text.mid(i, m_ngramSize));
    }
    return ngrams;
}

QString FuzzySearchIndex::normalize(const QString &text) {
    QString normalized;
    normalized.reserve(text.length());
    for (const QChar &ch : text) {
        if (ch.isLetterOrNumber()) {
            normalized.append(ch.toUpper());
        }
    }
    return normalized;
}

double FuzzySearchIndex::calculateSingleSimilarity(const QString &shortStr, const QString &longStr) {
    int shortLen = shortStr.length();
    int longLen = longStr.length();
    if (shortLen == 0 || longLen == 0) return 0.0;
    int maxMatches = 0;
    QStringRef longStrRef(&longStr);
    for (int i = 0; i <= longLen - shortLen; ++i) {
        QStringRef sub = longStrRef.mid(i, shortLen);
        int currentMatches = 0;
        for (int j = 0; j < shortLen; ++j) {
            if (shortStr[j] == sub[j]) {
                currentMatches++;
            }
        }
        if (currentMatches > maxMatches) {
            maxMatches = currentMatches;
        }
        if (maxMatches == shortLen) break;
    }
    return (static_cast<double>(maxMatches) / longLen) * 100.0;
}
