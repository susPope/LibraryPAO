#include <QFile>
#include <QJsonDocument>
#include <QDebug>
#include <algorithm>    // per std::remove_if
#include "mediarepo.h"
#include "libro.h"
#include "film.h"
#include "articolo.h"

MediaRepo::MediaRepo() {}

MediaRepo::~MediaRepo() {
    svuota(); // Libera memoria all'uscita
}

MediaRepo& MediaRepo::instance() {
    static MediaRepo repo;
    return repo;
}

void MediaRepo::aggiungiMedia(std::unique_ptr<Media> m) {
    mediaList.push_back(std::move(m));
    salvaSuJson();
}

bool MediaRepo::rimuoviMedia(Media* m) {
    auto it = std::remove_if(mediaList.begin(), mediaList.end(),
                [m](const std::unique_ptr<Media>& ptr) { return ptr.get() == m; });

    if (it != mediaList.end()) {
        mediaList.erase(it, mediaList.end());
        salvaSuJson();
        return true;
    }
    return false;
}

const std::vector<std::unique_ptr<Media>>& MediaRepo::getTuttiIMedia() const {
    return mediaList;
}

void MediaRepo::svuota() {
    mediaList.clear();
}

void MediaRepo::salvaSuJson() {
    QJsonArray libri, film, articoli;

    for (const auto& ptr : mediaList) {
        Media* m = ptr.get();
        if (auto l = dynamic_cast<Libro*>(m))
            libri.append(l->toJson());
        else if (auto f = dynamic_cast<Film*>(m))
            film.append(f->toJson());
        else if (auto a = dynamic_cast<Articolo*>(m))
            articoli.append(a->toJson());
    }

    QJsonObject root;
    root["libri"] = libri;
    root["film"] = film;
    root["articoli"] = articoli;

    qDebug() << "Salvataggio su path:" << path;

    QFile file(path);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(QJsonDocument(root).toJson(QJsonDocument::Indented));
        file.close();
    } else {
        qWarning() << "Errore apertura file:" << file.errorString();
    }
}
