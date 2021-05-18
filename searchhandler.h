#ifndef SEARCHHANDLER_H
#define SEARCHHANDLER_H

#include <QByteArray>
#include <QList>
#include <QObject>
#include <QString>
#include <QVector>
#include "model.h"

#include <QProcess>
#include <QTimer>



class SearchHandler : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(SearchHandler)


public:
    SearchHandler(const QString &pattern, const QString &category
                  , const QStringList &usedPlugins);
    bool isActive() const;
    QString pattern() const;
    QList<SearchResult> results() const;
    void cancelSearch();

signals:
    void searchFinished(bool cancelled = false);
    void searchFailed();
    void newSearchResults(const QVector<SearchResult> &results);

private:
    void readSearchOutput();
    void processFailed();
    void processFinished(int exitcode);
    bool parseSearchResult(const QString &line, SearchResult &searchResult);
    void handleStateChange(QProcess::ProcessState newState);

    const QString m_pattern;
    const QString m_category;
    const QStringList m_usedPlugins;
    QProcess *m_searchProcess;
    QTimer *m_searchTimeout;
    QByteArray m_searchResultLineTruncated;
    bool m_searchCancelled = false;
    QList<SearchResult> m_results;
};

#endif // SEARCHHANDLER_H
