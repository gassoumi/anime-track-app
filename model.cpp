#include "model.h"

bool model::Anime::operator==(const Anime &other) const {
    return name == other.name;
}

bool model::Episode::operator==(const Episode &other) const {
    return fileUrl == other.fileUrl;
}

model::Anime::Anime() {

}

model::Anime::~Anime() {

}

model::Episode::Episode() {

}

model::Episode::~Episode() {

}


void model::Episode::read(const QJsonObject &json)
{
    if (json.contains("fileName") && json["fileName"].isString())
        fileName = json["fileName"].toString();

    if (json.contains("fileUrl") && json["fileUrl"].isString())
        fileUrl = json["fileUrl"].toString();

    if (json.contains("fileSize") && json["fileSize"].isDouble())
        fileSize = json["fileSize"].toDouble();

    if (json.contains("nbSeeders") && json["nbSeeders"].isDouble())
        nbSeeders = json["nbSeeders"].toDouble();

    if (json.contains("nbLeechers") && json["nbLeechers"].isDouble())
        nbLeechers = json["nbLeechers"].toDouble();

    if (json.contains("siteUrl") && json["siteUrl"].isString())
        siteUrl = json["siteUrl"].toString();

    if (json.contains("descrLink") && json["descrLink"].isString())
        descrLink = json["descrLink"].toString();

    if (json.contains("episodeNumber") && json["episodeNumber"].isDouble())
        number = json["number"].toInt();

    if (json.contains("name") && json["name"].isString()) {
        anime.name = json["name"].toString();
    }

}

void model::Episode::write(QJsonObject &json) const
{
    json["fileName"] = fileName;
    json["fileUrl"] = fileUrl;
    json["fileSize"] = fileSize;
    json["nbSeeders"] = nbSeeders;
    json["nbLeechers"] = nbLeechers;
    json["siteUrl"] = siteUrl;
    json["descrLink"] = descrLink;
    json["number"] = number;
}

void model::Anime::read(const QJsonObject &json)
{
    if (json.contains("name") && json["name"].isString())
        name = json["name"].toString();

    if (json.contains("searchKey") && json["searchKey"].isString())
        searchKey = json["searchKey"].toString();

    if (json.contains("dateAjout") && json["dateAjout"].isString())
        dateAjout = QDateTime::fromString(json["dateAjout"].toString(), Qt::ISODate);
}

void model::Anime::write(QJsonObject &json) const
{
    json["name"] = name;
    json["searchKey"] = searchKey;
    json["dateAjout"] =  dateAjout.toString(Qt::ISODate);
}
