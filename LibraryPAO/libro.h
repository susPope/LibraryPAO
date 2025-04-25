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
          int pagine, const QString& isbn,
          bool disponibile = true, int nprestiti = 0,
          const QDate& proxDisp = QDate());
    ~Libro() override;  // Distruttore esplicito

    // Implementazione metodi polimorfi
    //QString mostraDettagli() const override;
    //QDate calcolaPrestito(int days) const override;

    // GETTER
    QString getAutore() const;
    QString getEditore() const;
    int getPagine() const;
    QString getIsbn() const;

    // SETTER
    void setAutore(const QString& nuovoautore);
    void setEditore(const QString& nuovoeditore);
    void setPagine(const int& nuovopagine);
    void setIsbn(const QString& nuovoisbn);
};

#endif // LIBRO_H
