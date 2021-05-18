#include "searchservice.h"
#include "utils.h"
#include <QtDebug>
#include <QDateTime>


#ifndef QT_NO_DEBUG
#define CHECK_TRUE(instruction) Q_ASSERT(instruction)
#else
#define CHECK_TRUE(instruction) (instruction)
#endif

namespace service {

SearchService::SearchService(QObject *parent)
    : QObject(parent)
{

}


void SearchService::fetchEpisodeAnime(const model::Anime &anime)
{

    /*
    SearchAnime *mSearchAnime = new SearchAnime(anime);
    connect(mSearchAnime, &SearchAnime::resultReady, this, &SearchService::handleResults);
    mWorkerThreads.append(mSearchAnime);
    */

    qDebug() << "fetch Episode is called";
    SearchAnime *worker = new SearchAnime(anime);
    QThread *workerThread = new QThread;

    worker->moveToThread(workerThread);
    connect(workerThread, &QThread::finished, worker, &QObject::deleteLater);
    connect(workerThread, &QThread::finished, workerThread, &QObject::deleteLater);
    connect(this, &SearchService::operate, worker, &SearchAnime::doWork);
    bool success = connect(worker, &SearchAnime::resultReady, this, &SearchService::handleResults);
    Q_ASSERT(success);
    qDebug() << success;
    workerThread->start();

    emit operate();

}

void SearchService::handleResults(const QVector<SearchResult> &results, const model::Anime &anime)
{
    qDebug()<< "handles results is called";
    //utils::handlenewSearchResults(results, mEpisodeRep, anime);
}

SearchService::~SearchService()
{
    /*
    for(const auto thread : mWorkerThreads) {
        thread->quit();
        thread->wait();
    }
    delete mEpisodeRep;
    */
    qDebug() << tr(" SearchService destructor is called");

}

}
