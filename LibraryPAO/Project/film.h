#ifndef FILM_H
#define FILM_H

#include "media.h"
#include <QStringList>
#include <QJsonObject>

class Film : public Media {
private:
    QString regista;
    int durata;         // in minuti
    QStringList cast;

public:
    //Film(const QString& regista, int durata, const QStringList& cast);
    Film();
    Film(const QString& titolo, const QString& genere, int anno,
          const QString& regista, int durata, const QStringList& cast);
    ~Film() override;

    // Implementazione metodi polimorfi
    QString generaId(int count) const override;
    QString mostraDettagli() const override;
    QDate calcolaPrestito() const override;
    QJsonObject toJson() const override;

    static Film fromJson(const QJsonObject& obj);


    // GETTER
    QString getRegista() const { return regista; }
    int getDurata() const { return durata; }
    QStringList getCast() const { return cast; }

    // SETTER
    void setRegista(const QString& nuovoregista);
    void setDurata(const int& nuovodurata);
    void setCast(const QStringList& nuovocast);
};

#endif // FILM_H
