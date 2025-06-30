#ifndef MEDIAREPO_H
#define MEDIAREPO_H

#include "media.h"

#include <QVector>
#include <QCoreApplication>
#include <vector>
#include <memory>
#include <QJsonArray>

class MediaRepo {
private:
    MediaRepo(); // Costruttore privato per singleton
    ~MediaRepo();

    std::vector<std::unique_ptr<Media>> mediaList;
    QString path = QCoreApplication::applicationDirPath() + "/../../Database/libraryDB.json"; //path default

    // Gestione JSON
    QString salvaSuJson();
    QString caricaDaJson();
    QString checkScrivibilitaFile() const;

public:
    static MediaRepo& instance(); // Singleton

    // Gestione RAM
    QString aggiungiMedia(std::unique_ptr<Media> m);
    QString aggiornaMedia(Media* m);
    QString rimuoviMedia(Media* m);
    QString importaDB();
    QString svuotaDB();
    const std::vector<std::unique_ptr<Media>>& getTuttiIMedia() const;

    // Controlli per l'inserimento e la modifica
    static QString normalizeISBN(const QString& raw);
    static bool isValidISBN(const QString& isbn);
    static bool checkLibro(const QString& isbn, QString& isbnPulito);

    void svuota();
    int countMedia(Media* media);

    // Gestione Ricerca
    std::vector<Media*> cercaMedia(const QString& testo, const QString& criterio);
    std::vector<Media*> cercaPrestiti(const QString& testo, const QString& criterio, const QString& filtroDisponibilità);

    //Gestione Prestiti
    QString aggiungiPrestito(Media* m);
    QString restituisciPrestito(Media* m);

    bool setPath(const QString& nuovaPath);
};

#endif // MEDIAREPOSITORY_H
