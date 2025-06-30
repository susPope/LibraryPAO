#ifndef ARTICOLO_H
#define ARTICOLO_H

#include "media.h"

#include <QJsonObject>

class Articolo : public Media {
private:
    QString autore;
    QString rivista;
    int volume;
    int pagine;

public:
    Articolo();
    Articolo(const QString& titolo, const QString& genere, int anno,
             const QString& autore, const QString& rivista, int volume, int pagine);
    ~Articolo() override;

    // Implementazione metodi polimorfi
    QString generaId(int count) const override;
    QDate calcolaPrestito() const override;
    QJsonObject toJson() const override;
    static Articolo fromJson(const QJsonObject& obj);
    QString getTipoIcona() const override;
    QString getDettagliString() const override;
    QString getIconaPath(bool darkTheme) const override;


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
