#include "media.h"

Media::Media(const QString &titolo, const QString &genere, int anno,
             bool disponibile, int nprestiti)
    : titolo(titolo),
      genere(genere),
      anno(anno),
      disponibile(disponibile),
      nprestiti(nprestiti),
      proxDisp(QDate::currentDate())
{}

// SETTER implementations
void Media::setTitolo(const QString& nuovotitolo) {
    if(!nuovotitolo.isEmpty()) {
        Media::titolo = nuovotitolo;
    } else {
        qWarning() << "Titolo non valido";
    }
}
void Media::setGenere(const QString& nuovogenere) {
    if(!nuovogenere.isEmpty()) {
        Media::genere = nuovogenere;
    } else {
        qWarning() << "Genere non valido";
    }
}
void Media::setAnno(const int& nuovoanno) {
    if(nuovoanno > 0) {
        Media::anno = nuovoanno;
    } else {
        qWarning() << "Anno non valido";
    }
}
void Media::setId(const QString& nuovoId) {
    if (!nuovoId.isEmpty()) {
        Media::id = nuovoId;
    } else {
        qWarning() << "ID non valido";
    }
}

void Media::setDisponibilita(bool nuovaDisponibilita) {
    Media::disponibile = nuovaDisponibilita;
}
void Media::setNprestiti(int nuovoNprestiti) {
    if (nuovoNprestiti >= 0) {
        Media::nprestiti = nuovoNprestiti;
    } else {
        qWarning() << "Numero di prestiti non valido";
    }
}
void Media::setProssimaDisponibilita(const QDate& nuovaproxdisponibilita) {
    if (nuovaproxdisponibilita.isValid()) {
        Media::proxDisp = nuovaproxdisponibilita;
    } else {
        qWarning() << "Data di disponibilità non valida";
    }
}
