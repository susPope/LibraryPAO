#include "libro.h"
#include <QDebug>

Libro::Libro(const QString& titolo, const QString& genere, int anno,
             const QString& autore, const QString& editore,
             int pagine, const QString& isbn)
    : Media(titolo, genere, anno),
    autore(autore),
    editore(editore),
    pagine(pagine),
    isbn(isbn)
{
    qDebug() << "Creato libro: " << titolo;
}

// Distruttore
Libro::~Libro() {
    qDebug() << "Distrutto libro:" << getTitolo();
}

// Implementazione metodo polimorfo per generare ID
QString Libro::generaId() const {
    return QString("LIB-%1-%2").arg(editore.left(3).toUpper(), isbn);
}

// Implementazione altri metodi polimorfi
QString Libro::mostraDettagli() const {
    return QString("%1\nAutore: %2\nEditore: %3\nPagine: %4\nISBN: %5")
        .arg(getTitolo(), autore, editore).arg(pagine).arg(isbn);
}

QDate Libro::calcolaPrestito(int days) const {
    return QDate::currentDate().addDays(days);
}

QJsonObject Libro::toJson() const {
    QJsonObject obj;

    // Attributi comuni
    obj["tipo"] = "Libro";  // utile per distinguere i media
    obj["titolo"] = getTitolo();
    obj["genere"] = getGenere();
    obj["anno"] = getAnno();
    obj["id"] = getId();
    obj["disponibile"] = getDisponibilita();
    obj["nprestiti"] = getNprestiti();
    obj["proxDisp"] = getProssimaDisponibilita().toString(Qt::ISODate);

    // Attributi specifici di Libro
    obj["autore"] = autore;
    obj["editore"] = editore;
    obj["pagine"] = pagine;
    obj["isbn"] = isbn;

    return obj;
}


// SETTER implementations
void Libro::setAutore(const QString& nuovoautore) {
    if(!nuovoautore.isEmpty()) {
        Libro::autore = nuovoautore;
    } else {
        qWarning() << "Autore non valido";
    }
}
void Libro::setEditore(const QString& nuovoeditore) {
    if(!nuovoeditore.isEmpty()) {
        Libro::editore = nuovoeditore;
    } else {
        qWarning() << "Editore non valido";
    }
}
void Libro::setPagine(const int& nuovopagine) {
    if(nuovopagine > 0) {
        pagine = nuovopagine;
    } else {
        qWarning() << "Numero di pagine non valido";
    }
}
void Libro::setIsbn(const QString& nuovoisbn) {
    // Validazione base ISBN
    // TODO: Aggiungere controlli sulla struttura dell'ISBN
    if(!nuovoisbn.isEmpty()) {
        isbn = nuovoisbn;
    } else {
        qWarning() << "ISBN non valido";
    }
}
