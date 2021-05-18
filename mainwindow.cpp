#include "mainwindow.h"
#include <QTimer>
#include <QDebug>
#include <QJsonDocument>
#include <QFile>
#include <QJsonArray>
#include <QMap>
#include <QStringList>
#include <QRegExp>
#include <QDateTime>
#include <QDir>
#include <QNetworkRequest>
#include <QUrl>
#include <QNetworkReply>
#include "utils.h"

MainWindow::MainWindow()
    : QObject()
    , settings(QSettings::IniFormat, QSettings::UserScope, "My anime organistaion", "Anime App rss")
    , mSearchHandler(nullptr)
    , manager(nullptr)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "first");
    db.setDatabaseName("anime.db");
    if( db.open()) {
        animeRep = new database::AnimeRepositorySql(db);
        episodeRep = new database::EpisodeRepositorySql(db);
        animes = animeRep->findAll();
        initialize();
    } else {
        qDebug() << "connection not open in MainWindow";
        //deleteLater();
    }

}

void MainWindow::initialize() {
    manager = new QNetworkAccessManager(this);

    QString intervalBetweenKey_key ="intervalBetweenKey";
    if(!settings.contains(intervalBetweenKey_key)) {
        settings.setValue(intervalBetweenKey_key, 5);
    }

    QString intervalBetweenAllKey_key ="intervalBetweenAllKey";
    if(!settings.contains(intervalBetweenAllKey_key)) {
        settings.setValue(intervalBetweenAllKey_key, 5);
    }

    qInfo() << settings.fileName();

    index = animes.constBegin();
    fetchNextKey();
}

void MainWindow::newEpisodeAdded(const QString &key,const SearchResult &episode) {
    qInfo() << "new Episode added " << key << "  " << episode.fileName ;

    if(settings.contains("TOKEN")) {
        QString token = settings.value("TOKEN").toString();
        QString authorization = "Bearer " + token;
        const QUrl url(QStringLiteral("https://api.pushbullet.com/v2/pushes"));
        QNetworkRequest request(url);
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
        request.setRawHeader(QByteArray("Authorization"), QByteArray(authorization.toUtf8()));

        QJsonObject obj;
        obj["type"] = "note";
        obj["title"] = key;
        obj["body"] = "episode " + QString::number(episode.number) + " added to " + key;
        QJsonDocument doc(obj);
        QByteArray data = doc.toJson();
        // or
        // QByteArray data("{\"key1\":\"value1\",\"key2\":\"value2\"}");
        QNetworkReply *reply = manager->post(request, data);

        QObject::connect(reply, &QNetworkReply::finished, [=]() {
            if(reply->error() == QNetworkReply::NoError) {
                QString contents = QString::fromUtf8(reply->readAll());
                //qInfo() << contents;
            }
            else {
                QString err = reply->errorString();
                qWarning() << err;
            }
            reply->deleteLater();
        });
    } else {
        qWarning() << tr("cannot send message notification there is no token in setting file");
    }


    if(!episode.fileUrl.contains("magnet")) {
        qWarning() << "file url doesn't contain a valid magnet url";
        return;
    }

    QString path = utils::toNativePath(QDir::currentPath() + "//" + key) ;
    qDebug() << path ;
    if( !QDir(path).exists() ) {
        if( !QDir().mkdir(path) ) {
            qWarning() << "cannot create directory for saving new episodes"  ;
            return;
        }
    }
    QDir savedDirectory(path);
    if( savedDirectory.isReadable() ) {
        QFile saveFile( utils::toNativePath(path +"//" + key + " - " + QString::number(episode.number) + ".magnet")) ;
        if(saveFile.open(QIODevice::WriteOnly)) {
            QTextStream out(&saveFile);
            out << episode.fileUrl << "\n" ;
        }
        saveFile.close();
    } else {
        qWarning() << "cannot save new episode"  ;
    }

}


void MainWindow::fetchNextKey() {
    if( index != animes.constEnd() ) {
        QString key = index->searchKey;
        QStringList engines;
        engines << "all";
        delete mSearchHandler;
        mSearchHandler = nullptr;
        mSearchHandler = new SearchHandler(key,"anime",engines);
        connect(mSearchHandler,&SearchHandler::newSearchResults, this, &MainWindow::newSearchResultsRecevied);
        connect(mSearchHandler,&SearchHandler::searchFinished, this, &MainWindow::searchFinishedRecevied);
        connect(mSearchHandler,&SearchHandler::searchFailed, this, &MainWindow::searchFailedRecevied);
    }
}

void MainWindow::searchFinishedRecevied(bool cancelled) {
    QString message = cancelled ? tr("Search is finish but it was cancelled") : tr("Search is finish");

    qDebug() << message << " | " << QDateTime::currentDateTime() << "|" << mSearchHandler->pattern() ;
    // we make a save of episodes after we finish a search request
    //saveAnimesSlot();

    // start a timer for the next key fetch after 5 min
    int intervalBetweenKey = settings.value("intervalBetweenKey",5).toInt();
    // start the timer after 5 hours
    int intervalBetweenAllKey = settings.value("intervalBetweenAllKey",5).toInt();
    // update to the index iterartor when the search is finished
    index ++;
    int interval = 0;
    if( index == animes.constEnd() ) {
        qInfo() << tr("all keys are fetched \n restart next fetch after 3 hours");
        //reset from the begin
        animes = animeRep->findAll();
        index = animes.constBegin();
        interval = intervalBetweenAllKey * 60 * 60 * 1000;
        QTimer::singleShot(interval, this,  &MainWindow::fetchNextKey);
    } else {
        interval = intervalBetweenKey * 60 * 1000;
        QTimer::singleShot(interval, this,  &MainWindow::fetchNextKey);
    }
}

void MainWindow::searchFailedRecevied() {
    qWarning() << tr("Search fail") ;
}

void MainWindow:: newSearchResultsRecevied(const QVector<SearchResult> &results) {

    utils::handlenewSearchResults(results, episodeRep, *index);

    //add the result in qmap
    /*
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
        episodeToAdd.anime = *index;
        episodeRep->findByNumber(episodeToAdd, &isFound);
        if(!isFound)  {
            episodeRep->add(episodeToAdd);
        }
    }
    */
}

MainWindow::~MainWindow()
{
    delete mSearchHandler;
    mSearchHandler = nullptr;
    delete manager;
    manager = nullptr;
}






