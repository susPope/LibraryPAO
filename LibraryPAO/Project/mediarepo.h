#ifndef MEDIAREPO_H
#define MEDIAREPO_H

#include <QVector>
#include <QCoreApplication>
#include <vector>
#include <memory>
#include "QJsonArray"
#include "media.h"

class MediaRepo {
public:
    static MediaRepo& instance(); // Singleton

    // Gestione RAM
    void aggiungiMedia(std::unique_ptr<Media> m);
    bool rimuoviMedia(Media* m);
    const std::vector<std::unique_ptr<Media>>& getTuttiIMedia() const;

    void svuota();

private:
    MediaRepo(); // Costruttore privato per singleton
    ~MediaRepo();

    std::vector<std::unique_ptr<Media>> mediaList;
    const QString path = QCoreApplication::applicationDirPath() + "/../../Database/libraryDB.json";

    // Gestione JSON
    void salvaSuJson();
};

#endif // MEDIAREPOSITORY_H
