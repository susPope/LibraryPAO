#ifndef LIBRO_H
#define LIBRO_H

#include "media.h"

class Libro : public Media {
public:
    Libro(const QString& titolo, int anno, const QString& autore,
          const QString& editore, int pagine, const QString& isbn, const QString& genere);

    // Implementazione metodi polimorfi

protected:
    QString autore;
    QString editore;
    int pagine;
    QString isbn;
    QString genere;
};

#endif // LIBRO_H
