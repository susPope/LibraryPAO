#include "mediarepo.h"

MediaRepo::MediaRepo() {}

MediaRepo::~MediaRepo() {
    svuota(); // Libera memoria all'uscita
}

MediaRepo& MediaRepo::instance() {
    static MediaRepo repo;
    return repo;
}

void MediaRepo::aggiungiMedia(Media* m) {
    if(m){ mediaList.append(m); }
}

bool MediaRepo::rimuoviMedia(Media* m) {
    return mediaList.removeOne(m);
}

const QVector<Media*>& MediaRepo::getTuttiIMedia() const {
    return mediaList;
}

void MediaRepo::svuota() {
    for(int i = 0; i < mediaList.size(); ++i){
        delete mediaList[i];
    }
    mediaList.clear();
}
