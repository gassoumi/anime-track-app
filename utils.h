#ifndef UTILS_H
#define UTILS_H
#include <QString>
#include "model.h"
#include "episoderepository.h"

namespace utils {
QString toNativePath(const QString &path);
int getEpisodeNumber(QString const &fileName, bool *ok = nullptr);
//find new episodes in results of anime then add them to the database
void handlenewSearchResults(const QVector<SearchResult> &results, database::EpisodeRepository *episodeRep, const model::Anime &anime);
}

#endif // UTILS_H
