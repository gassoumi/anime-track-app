#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QSettings>
#include <QJsonObject>
#include "searchhandler.h"
#include <QTimer>
#include <QObject>
#include <QNetworkAccessManager>
#include "model.h"
#include "repository.h"
#include "animeRepository.h"
#include "episoderepository.h"

class MainWindow : public QObject
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();

private:
    void initialize();
    void fetchNextKey();

public slots:
    void newSearchResultsRecevied(const QVector<SearchResult> &results);
    void searchFinishedRecevied(bool cancelled = false);
    void searchFailedRecevied();
    void newEpisodeAdded(const QString &key,const SearchResult &episode);

private:
    QSettings settings;
    SearchHandler *mSearchHandler;
    QVector<model::Anime>::const_iterator index;
    QNetworkAccessManager *manager;
    database::AnimeRepository *animeRep;
    database::EpisodeRepository *episodeRep;
    QVector<model::Anime> animes;
};
#endif // MAINWINDOW_H
