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

Libro::Libro() : Media(), autore(""), editore(""), pagine(0), isbn("") {}

// Distruttore
Libro::~Libro() {
    qDebug() << "Distrutto libro:" << getTitolo();
}

// Implementazione metodo polimorfo per generare ID
QString Libro::generaId(int count) const {
    QString cleanTitolo = getTitolo().simplified().remove(' ').toUpper();
    QString cleanAutore = autore.simplified().remove(' ').toUpper();

    return QString("LIB-%1-%2-%3").arg(cleanTitolo, cleanAutore, QString::number(count));
}


// Implementazione altri metodi polimorfi
QDate Libro::calcolaPrestito() const {
    // Film hanno prestito più lungo (30 giorni)
    return QDate::currentDate().addDays(30);
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

Libro Libro::fromJson(const QJsonObject& obj) {
    // Costruzione base (può anche essere un Articolo() vuoto se serve)
    Libro l;
    l.setTitolo(obj["titolo"].toString());
    l.setGenere(obj["genere"].toString());
    l.setAnno(obj["anno"].toInt());
    l.setId(obj["id"].toString());
    l.setDisponibilita(obj["disponibile"].toBool());
    l.setNprestiti(obj["nprestiti"].toInt());
    l.setProssimaDisponibilita(QDate::fromString(obj["proxDisp"].toString(), Qt::ISODate));

    // Attributi specifici di Libro
    l.setAutore(obj["autore"].toString());
    l.setEditore(obj["editore"].toString());
    l.setPagine(obj["pagine"].toInt());
    l.setIsbn(obj["isbn"].toString());

    return l;
}

QString Libro::getTipoIcona() const {
    return "📚 Libro";
}

QString Libro::getDettagliString() const {
    return QString("Autore: %1 • ISBN: %2").arg(getAutore(), getIsbn());
}

QString Libro::getIconaPath(bool darkTheme) const {
    if(darkTheme) {
        return ":/resources/icons/light/libro.png";
    } else {
        return ":/resources/icons/dark/libro.png";
    }
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
