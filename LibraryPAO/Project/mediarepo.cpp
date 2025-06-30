#include "mediarepo.h"
#include "libro.h"
#include "film.h"
#include "articolo.h"

#include <QFile>
#include <QFileInfo>
#include <QJsonDocument>
#include <QDebug>
#include <QDate>
#include <algorithm>

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

QString MediaRepo::aggiungiMedia(std::unique_ptr<Media> m) {
    QString check = checkScrivibilitaFile();
    if (!check.isEmpty()) return check;

    mediaList.push_back(std::move(m));
    QString errore = salvaSuJson();
    if (!errore.isEmpty()) { return errore; }
    return "";
}

QString MediaRepo::aggiornaMedia(Media* m) {
    QString check = checkScrivibilitaFile();
    if (!check.isEmpty()) return check;

    if (auto l = dynamic_cast<Libro*>(m))
        qInfo() << "Modificato Libro: " << l->getTitolo() << "Autore: " << l->getAutore();
    else if (auto f = dynamic_cast<Film*>(m))
        qInfo() << "Modificato Film: " << f->getTitolo() << "Regista: " << f->getRegista();
    else if (auto a = dynamic_cast<Articolo*>(m))
        qInfo() << "Modificato Articolo: " << a->getTitolo() << "Autore: " << a->getAutore();

    QString errore = salvaSuJson();
    if (!errore.isEmpty()) { return errore; }
    return "";
}

QString MediaRepo::rimuoviMedia(Media* m) {
    QString check = checkScrivibilitaFile();
    if (!check.isEmpty()) return check;

    auto it = std::remove_if(mediaList.begin(), mediaList.end(),
                [m](const std::unique_ptr<Media>& ptr) { return ptr.get() == m; });

    if (it != mediaList.end()) {
        mediaList.erase(it, mediaList.end());
        QString errore = salvaSuJson();
        if (!errore.isEmpty()) { return errore; }
        return "";
    }
    return "Elemento non trovato nel repository.";
}

const std::vector<std::unique_ptr<Media>>& MediaRepo::getTuttiIMedia() const {
    return mediaList;
}

QString MediaRepo::importaDB() {
    QString check = checkScrivibilitaFile();
    if (!check.isEmpty()) return check;

    svuota();
    QString errore = caricaDaJson();
    if (!errore.isEmpty()) { return errore; }
    qInfo() << "DataBase importato";
    return "";
}

QString MediaRepo::svuotaDB() {
    QString check = checkScrivibilitaFile();
    if (!check.isEmpty()) return check;

    svuota();
    QString errore = salvaSuJson();
    if (!errore.isEmpty()) { return errore; }
    qInfo() << "DataBase eliminato";
    return "";
}

void MediaRepo::svuota() {
    mediaList.clear();
}

QString MediaRepo::salvaSuJson() {
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

    QFile file(path);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(QJsonDocument(root).toJson(QJsonDocument::Indented));
        file.close();
        return "";
    } else {
        qWarning() << "Errore apertura file:" << file.errorString();
        return "Errore apertura file: " + file.errorString();
    }
}

QString MediaRepo::caricaDaJson() {
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Impossibile aprire il file per la lettura:" << file.errorString();
        return "Impossibile aprire il file per la lettura: " + file.errorString();
    }

    QByteArray jsonData = file.readAll();
    file.close();

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(jsonData, &parseError);
    if (parseError.error != QJsonParseError::NoError) {
        qWarning() << "Errore parsing JSON:" << parseError.errorString();
        return "Errore parsing JSON: " + parseError.errorString();
    }

    QJsonObject root = doc.object();

    auto aggiungiDaArray = [this](const QJsonArray& array, const QString& tipo) {
        for (const QJsonValue& val : array) {
            QJsonObject obj = val.toObject();
            std::unique_ptr<Media> m;

            if (tipo == "libri")         m = std::make_unique<Libro>(Libro::fromJson(obj));
            else if (tipo == "film")     m = std::make_unique<Film>(Film::fromJson(obj));
            else if (tipo == "articoli") m = std::make_unique<Articolo>(Articolo::fromJson(obj));

            if (m) mediaList.push_back(std::move(m));
        }
    };

    aggiungiDaArray(root["libri"].toArray(), "libri");
    aggiungiDaArray(root["film"].toArray(), "film");
    aggiungiDaArray(root["articoli"].toArray(), "articoli");

    qDebug() << "Caricati" << mediaList.size() << "media dal JSON.";
    return "";  // Tutto ok
}

// CONTROLLI
QString MediaRepo::normalizeISBN(const QString& raw) {
    QString cleaned = raw;
    cleaned.remove(' ');
    cleaned.remove('-');

    if (cleaned.length() == 10 && cleaned[9].toUpper() == 'X') {
        cleaned[9] = 'X';  // forza la 'X' maiuscola
    }

    return cleaned;
}

bool MediaRepo::isValidISBN(const QString& isbn) {
    int len = isbn.length();
    if (len != 10 && len != 13)
        return false;

    for (int i = 0; i < len; ++i) {
        if (!isbn[i].isDigit()) {
            if (len == 10 && i == 9 && isbn[i].toUpper() == 'X') {
                continue;
            }
            return false;
        }
    }

    return true;
}

bool MediaRepo::checkLibro(const QString& isbn, QString& isbnPulito) {
    isbnPulito = normalizeISBN(isbn);
    return isValidISBN(isbnPulito);
}

// Cerca l'ultimo numero di disambiguità negli id con titolo e autore uguali per creare un id univoco
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
                    // Estrae numero da ID: LIB-TITOLO-AUTORE-<numero>
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
        Media* m = mediaPtr.get();  // Non trasferisce la proprietà
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

QString MediaRepo::aggiungiPrestito(Media* m) {
    if (m->getDisponibilita()) {
        QString check = checkScrivibilitaFile();
        if (!check.isEmpty()) return check;

        m->setProssimaDisponibilita(m->calcolaPrestito());
        m->setDisponibilita(false); // Non disponibile
        m->setNprestiti(m->getNprestiti()+1);

        QString errore = salvaSuJson();
        if (!errore.isEmpty()) { return errore; }
    } else {
        qWarning() << "Media non disponibile. Non può essere prenotato!";
        return "Il media selezionato non è disponibile per il prestito.";
    }
    return "";
}

QString MediaRepo::restituisciPrestito(Media* m){
    if (!m->getDisponibilita()) {
        QString check = checkScrivibilitaFile();
        if (!check.isEmpty()) return check;

        m->setProssimaDisponibilita(QDate::currentDate());
        m->setDisponibilita(true); // Disponibile

        QString errore = salvaSuJson();
        if (!errore.isEmpty()) { return errore; }
    } else {
        qWarning() << "Libro non prenotato. Non può essere restituito!";
        return "Il media non è attualmente in prestito, quindi non può essere restituito.";
    }
    return "";
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
        // Se "Tutti", non filtra

        // Filtra testo e criterio
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

bool MediaRepo::setPath(const QString& nuovaPath) {
    if(path.isEmpty()) {
        qWarning() << "Nuova PATH non valida";
        return false;
    }
    path = nuovaPath;
    return true;
}

QString MediaRepo::checkScrivibilitaFile() const {
    if (path.trimmed().isEmpty()) {
        return "Percorso del file JSON non valido (vuoto).";
    }

    QFileInfo fileInfo(path);
    if (!fileInfo.exists()) {
        return "Il file non esiste: " + path;
    }
    if (fileInfo.isDir()) {
        return "Il percorso punta a una directory, non a un file: " + path;
    }
    if (!fileInfo.isFile()) {
        return "Il percorso non punta a un file regolare: " + path;
    }

    QFile file(path);
    try {
        if (!file.open(QIODevice::ReadWrite)) {
            return "Impossibile aprire il file JSON in scrittura: " + file.errorString();
        }
        file.close();
    } catch (...) {
        return "Eccezione durante l'apertura del file: " + path;
    }

    return "";  // OK
}
