#include "utils.h"
#include <QDir>
#include <QRegExp>

QString utils::toNativePath(const QString &path)
{
    return QDir::toNativeSeparators(path);
}


int utils::getEpisodeNumber(QString const &fileName, bool *ok) {
    QRegExp rx(" - \\d+");
    int episodeNumber = -1 ;
    int pos = 0;
    if ((pos = rx.indexIn(fileName, pos)) != -1)  {
        //qDebug() << rx.captureCount();
        QString stringNumber = rx.cap(0);
        QRegExp rx2("(\\d+)");
        if(rx2.indexIn(stringNumber) != -1) {
            QString EpisodeValue = rx2.cap(1);
            episodeNumber = EpisodeValue.toInt(ok);
        }
    }
    return episodeNumber;
}

//find new episodes in results of anime then add them to the database
void utils::handlenewSearchResults(const QVector<SearchResult> &results, database::EpisodeRepository *episodeRep, const model::Anime &anime)
{
    //qDebug() << "newSearchResultsRecevied called ";
    //add the result in qmap

    //add the result in qmap
    QRegExp rx(" - \\d+");
    QMultiMap<int, SearchResult> episodes;
    QVector<SearchResult> filtredEpisode ;
    for(auto &item : results) {
        if(rx.indexIn(item.fileName) != -1 ) {
            // appel de constructeur de copie
            model::Episode newTorrent = item;
            newTorrent.number = utils::getEpisodeNumber(item.fileName);
            episodes.insert(newTorrent.number, newTorrent);
        }
    }
    for (auto it = episodes.keyBegin(), end = episodes.keyEnd(); it != end; ++it) {
        QList<SearchResult> values = episodes.values((*it));

        auto maxSeedersTorrent = std::max_element(values.constBegin(),values.constEnd(),[](const SearchResult& torrent, const SearchResult& other) {
            return torrent.nbSeeders < other.nbSeeders ;
        });

        bool isFound = false;
        model::Episode episodeToAdd = *maxSeedersTorrent;
        episodeToAdd.anime = anime;
        episodeRep->findByNumber(episodeToAdd, &isFound);
        if(!isFound)  {
            episodeRep->add(episodeToAdd);
        }
    }
}
