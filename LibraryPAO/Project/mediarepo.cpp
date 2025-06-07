#include "mediarepo.h"
#include "libro.h"
#include "film.h"
#include "articolo.h"

#include <QFile>
#include <QJsonDocument>
#include <QDebug>
#include <QDate>
#include <algorithm>    // per std::remove_if

MediaRepo::MediaRepo() {
    svuota();
    caricaDaJson();
}

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

void MediaRepo::aggiornaMedia(Media* m) {

    if (auto l = dynamic_cast<Libro*>(m))
        qInfo() << "Modificato Libro: " << l->getTitolo() << "Autore: " << l->getAutore();
    else if (auto f = dynamic_cast<Film*>(m))
        qInfo() << "Modificato Film: " << f->getTitolo() << "Regista: " << f->getRegista();
    else if (auto a = dynamic_cast<Articolo*>(m))
        qInfo() << "Modificato Articolo: " << a->getTitolo() << "Autore: " << a->getAutore();

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

void MediaRepo::svuotaDB() {
    svuota();
    salvaSuJson();
    qInfo() << "DataBase eliminato";
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

void MediaRepo::caricaDaJson() {
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Impossibile aprire il file per la lettura:" << file.errorString();
        return;
    }

    QByteArray jsonData = file.readAll();
    file.close();

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(jsonData, &parseError);
    if (parseError.error != QJsonParseError::NoError) {
        qWarning() << "Errore parsing JSON:" << parseError.errorString();
        return;
    }

    QJsonObject root = doc.object();

    auto aggiungiDaArray = [this](const QJsonArray& array, const QString& tipo) {
        for (const QJsonValue& val : array) {
            QJsonObject obj = val.toObject();
            std::unique_ptr<Media> m;

            if (tipo == "libri")      m = std::make_unique<Libro>(Libro::fromJson(obj));
            else if (tipo == "film")  m = std::make_unique<Film>(Film::fromJson(obj));
            else if (tipo == "articoli") m = std::make_unique<Articolo>(Articolo::fromJson(obj));

            if (m) mediaList.push_back(std::move(m));
        }
    };

    aggiungiDaArray(root["libri"].toArray(), "libri");
    aggiungiDaArray(root["film"].toArray(), "film");
    aggiungiDaArray(root["articoli"].toArray(), "articoli");

    qDebug() << "Caricati" << mediaList.size() << "media dal JSON.";
}

// CONTROLLI
bool MediaRepo::checkLibro(const QString& isbn) {
    // Rimuovi spazi e trattini
    QString cleaned = isbn;
    cleaned.remove(' ');
    cleaned.remove('-');
    int len = cleaned.length();

    return (len == 10 || len == 13);
}

// Cerca l'ultimo numero di disambiguità negli id con titoloe autore uguali per creare un id univoco
int MediaRepo::countMedia(Media* media) {
    if (!media) return 0;

    QString titolo = media->getTitolo().simplified().remove(' ').toUpper();
    QString autore_regista;
    QString tipo;

    // Determina tipo e autore/regista
    if (auto* libro = dynamic_cast<Libro*>(media)) {
        autore_regista = libro->getAutore().simplified().remove(' ').toUpper();
        tipo = "Libro";
    } else if (auto* film = dynamic_cast<Film*>(media)) {
        autore_regista = film->getRegista().simplified().remove(' ').toUpper();
        tipo = "Film";
    } else if (auto* articolo = dynamic_cast<Articolo*>(media)) {
        autore_regista = articolo->getAutore().simplified().remove(' ').toUpper();
        tipo = "Articolo";
    } else {
        return 0; // Tipo sconosciuto
    }

    int maxCount = -1;

    for (const auto& mediaPtr : mediaList) {
        if (mediaPtr.get() == media) continue;

        QString id = mediaPtr->getId();

        if (tipo == "Libro") {
            if (auto* libro = dynamic_cast<Libro*>(mediaPtr.get())) {
                QString t = libro->getTitolo().simplified().remove(' ').toUpper();
                QString a = libro->getAutore().simplified().remove(' ').toUpper();
                if (t == titolo && a == autore_regista) {
                    // Estrai numero da ID: LIB-TITOLO-AUTORE-<numero>
                    QStringList parts = id.split("-");
                    if (parts.size() >= 4) {
                        bool ok = false;
                        int numero = parts.last().toInt(&ok);
                        if (ok && numero > maxCount)
                            maxCount = numero;
                    }
                }
            }
        } else if (tipo == "Film") {
            if (auto* film = dynamic_cast<Film*>(mediaPtr.get())) {
                QString t = film->getTitolo().simplified().remove(' ').toUpper();
                QString r = film->getRegista().simplified().remove(' ').toUpper();
                if (t == titolo && r == autore_regista) {
                    QStringList parts = id.split("-");
                    if (parts.size() >= 4) {
                        bool ok = false;
                        int numero = parts.last().toInt(&ok);
                        if (ok && numero > maxCount)
                            maxCount = numero;
                    }
                }
            }
        } else if (tipo == "Articolo") {
            if (auto* art = dynamic_cast<Articolo*>(mediaPtr.get())) {
                QString t = art->getTitolo().simplified().remove(' ').toUpper();
                QString a = art->getAutore().simplified().remove(' ').toUpper();
                if (t == titolo && a == autore_regista) {
                    QStringList parts = id.split("-");
                    if (parts.size() >= 4) {
                        bool ok = false;
                        int numero = parts.last().toInt(&ok);
                        if (ok && numero > maxCount)
                            maxCount = numero;
                    }
                }
            }
        }
    }

    return maxCount + 1;
}

std::vector<Media*> MediaRepo::cercaMedia(const QString& testo, const QString& criterio) {
    std::vector<Media*> risultati;
    for (const auto& mediaPtr : mediaList) {
        Media* m = mediaPtr.get();  // non trasferisce la proprietà
        if (criterio == "Titolo") {
            if (m->getTitolo().contains(testo, Qt::CaseInsensitive)) {
                risultati.push_back(m);
            }
        } else if (criterio == "Autore o Regista") {
            bool match = false;
            if (auto libro = dynamic_cast<Libro*>(m)) {
                match = libro->getAutore().contains(testo, Qt::CaseInsensitive);
            } else if (auto articolo = dynamic_cast<Articolo*>(m)) {
                match = articolo->getAutore().contains(testo, Qt::CaseInsensitive);
            } else if (auto film = dynamic_cast<Film*>(m)) {
                match = film->getRegista().contains(testo, Qt::CaseInsensitive);
            }
            if (match)
                risultati.push_back(m);
        }
    }
    return risultati;
}

void MediaRepo::aggiungiPrestito(Media* m) {
    if (m->getDisponibilita()) {
        m->setProssimaDisponibilita(m->calcolaPrestito());
        m->setDisponibilita(false); // Non disponibile
        m->setNprestiti(m->getNprestiti()+1);

        salvaSuJson();
    } else {
        qWarning() << "Libro non disponibile. Non può essere prenotato!";
    }
}

void MediaRepo::restituisciPrestito(Media* m){
    if (!m->getDisponibilita()) {
        m->setProssimaDisponibilita(QDate::currentDate());
        m->setDisponibilita(true); // Disponibile

        salvaSuJson();
    } else {
        qWarning() << "Libro non prenotato. Non può essere restituito!";
    }
}

std::vector<Media*> MediaRepo::cercaPrestiti(const QString& testo, const QString& criterio, const QString& filtroDisponibilita) {
    std::vector<Media*> risultati;

    for (const auto& mediaPtr : mediaList) {
        Media* m = mediaPtr.get();

        // Filtro disponibilità e ritardo
        if (filtroDisponibilita == "In prestito" && m->getDisponibilita())
            continue;
        else if (filtroDisponibilita == "Disponibili" && !m->getDisponibilita())
            continue;
        else if (filtroDisponibilita == "In ritardo" && !m->isInRitardo()) // <- aggiunto controllo ritardo
            continue;
        // Se "Tutti", non filtriamo

        // Filtro testo e criterio
        if (criterio == "Titolo") {
            if (m->getTitolo().contains(testo, Qt::CaseInsensitive)) {
                risultati.push_back(m);
            }
        } else if (criterio == "Autore o Regista") {
            bool match = false;
            if (auto libro = dynamic_cast<Libro*>(m)) {
                match = libro->getAutore().contains(testo, Qt::CaseInsensitive);
            } else if (auto articolo = dynamic_cast<Articolo*>(m)) {
                match = articolo->getAutore().contains(testo, Qt::CaseInsensitive);
            } else if (auto film = dynamic_cast<Film*>(m)) {
                match = film->getRegista().contains(testo, Qt::CaseInsensitive);
            }
            if (match)
                risultati.push_back(m);
        }
    }

    return risultati;
}
