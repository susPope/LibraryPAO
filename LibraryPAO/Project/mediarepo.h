#ifndef MEDIAREPO_H
#define MEDIAREPO_H

#include <QVector>
#include "media.h"

class MediaRepo {
public:
    static MediaRepo& instance(); // Singleton

    void aggiungiMedia(Media* m);
    bool rimuoviMedia(Media* m);
    const QVector<Media*>& getTuttiIMedia() const;

    void svuota();

private:
    MediaRepo(); // Costruttore privato per singleton
    ~MediaRepo();

    QVector<Media*> mediaList;
};

#endif // MEDIAREPOSITORY_H
