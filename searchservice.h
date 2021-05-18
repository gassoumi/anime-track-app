#ifndef SEARCHSERVICE_H
#define SEARCHSERVICE_H

#include <QObject>
#include "searchhandler.h"
#include <QString>
#include "episoderepository.h"
#include "model.h"
#include <QThread>
#include "searchanime2.h"


namespace service {

class SearchService : public QObject
{
    Q_OBJECT
    //Q_DISABLE_COPY(SearchService)
public:
    SearchService(QObject *parent = nullptr);
    void fetchEpisodeAnime(const model::Anime &anime);
    ~SearchService();

public slots:
    void handleResults(const QVector<SearchResult> &results, const model::Anime &anime);

signals:
    void operate();

private:
    //QList<SearchAnime*> mWorkerThreads;
};
}
#endif // SEARCHSERVICE_H
