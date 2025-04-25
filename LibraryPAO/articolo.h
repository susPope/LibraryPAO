#ifndef ARTICOLO_H
#define ARTICOLO_H

#include "media.h"

class Articolo : public Media {
private:
    QString autore;
    QString rivista;
    int volume;         // in minuti
    int pagine;

public:
    //Film(const QString& regista, int durata, const QStringList& cast);
    Articolo(const QString& titolo, const QString& genere, int anno,
             const QString& autore, const QString& rivista, int volume, int pagine);
    ~Articolo() override;

    // Implementazione metodi polimorfi
    QString generaId() const override;
    //QString mostraDettagli() const override;
    //QDate calcolaPrestito(int days) const override;

    // GETTER
    QString getAutore() const { return autore; }
    QString getRivista() const { return rivista; }
    int getVolume() const { return volume; }
    int getPagine() const { return pagine; }

    // SETTER
    void setAutore(const QString& nuovoautore);
    void setRivista(const QString& nuovorivista);
    void setVolume(const int& nuovovolume);
    void setPagine(const int& nuovopagine);
};

#endif // ARTICOLO_H
