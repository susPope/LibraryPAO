#ifndef FILM_H
#define FILM_H

#include "media.h"
#include <QStringList>

class Film : public Media {
private:
    QString regista;
    int durata;         // in minuti
    QStringList cast;

public:
    //Film(const QString& regista, int durata, const QStringList& cast);
    Film(const QString& titolo, const QString& genere, int anno,
          const QString& regista, int durata, const QStringList& cast,
          bool disponibile = true, int nprestiti = 0,
          const QDate& proxDisp = QDate());
    virtual ~Film() = default;

    // Implementazione metodi polimorfi
    //QString mostraDettagli() const override;
    //QDate calcolaPrestito(int days) const override;

    // GETTER
    QString getRegista() const;
    int getDurata() const;
    QStringList getCast() const;

    // SETTER
    void setRegista(const QString& nuovoregista);
    void setDurata(const int& nuovodurata);
    void setCast(const QStringList& nuovocast);
};

#endif // FILM_H
