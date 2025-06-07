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
    void aggiornaMedia(Media* m);
    bool rimuoviMedia(Media* m);
    void svuotaDB();
    const std::vector<std::unique_ptr<Media>>& getTuttiIMedia() const;

    // Controlli per l'inserimento e la modifica
    bool checkLibro(const QString& isbn);

    void svuota();
    int countMedia(Media* media);

    // Gestione Ricerca
    std::vector<Media*> cercaMedia(const QString& testo, const QString& criterio);
    std::vector<Media*> cercaPrestiti(const QString& testo, const QString& criterio, const QString& filtroDisponibilità);

    //Gestione Prestiti
    void aggiungiPrestito(Media* m);
    void restituisciPrestito(Media* m);

private:
    MediaRepo(); // Costruttore privato per singleton
    ~MediaRepo();

    std::vector<std::unique_ptr<Media>> mediaList;
    const QString path = QCoreApplication::applicationDirPath() + "/../../Database/libraryDB.json";

    // Gestione JSON
    void salvaSuJson();
    void caricaDaJson();
};

#endif // MEDIAREPOSITORY_H
