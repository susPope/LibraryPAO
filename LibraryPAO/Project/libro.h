#ifndef LIBRO_H
#define LIBRO_H

#include "media.h"

class Libro : public Media {
private:
    QString autore;
    QString editore;
    int pagine;
    QString isbn;

public:
    // Costruttori / Distruttori
    Libro(const QString& titolo, const QString& genere, int anno,
          const QString& autore, const QString& editore,
          int pagine, const QString& isbn);
    ~Libro() override;  // Distruttore esplicito

    // Implementazione metodi polimorfi
    QString generaId() const override;
    QString mostraDettagli() const override;
    QDate calcolaPrestito(int days) const override;

    // GETTER
    QString getAutore() const { return autore; }
    QString getEditore() const { return editore; }
    int getPagine() const { return pagine; }
    QString getIsbn() const { return isbn; }

    // SETTER
    void setAutore(const QString& nuovoautore);
    void setEditore(const QString& nuovoeditore);
    void setPagine(const int& nuovopagine);
    void setIsbn(const QString& nuovoisbn);
};

#endif // LIBRO_H
