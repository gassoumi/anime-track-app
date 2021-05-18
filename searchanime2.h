#ifndef SEARCHANIME2_H
#define SEARCHANIME2_H


#include <QObject>
#include "model.h"
#include <QVector>
#include <QSqlDatabase>

namespace service {

class SearchAnime : public QObject
{
    Q_OBJECT
    //Q_DISABLE_COPY(SearchAnime)
public:
    SearchAnime(const model::Anime &anime);
    ~SearchAnime();

signals:
    void resultReady(const QVector<SearchResult> &, const model::Anime &);

public slots:
    void doWork();
    void newSearchResultsRecevied(const QVector<SearchResult> &results);
    void searchFinishedRecevied(bool cancelled = false);
    void searchFailedRecevied();

private:
    model::Anime mAnime;
    QSqlDatabase db;
    bool isDbOpen;
    QString connectionName;
    static int nextIdDB;
};
}

#endif // SEARCHANIME2_H
