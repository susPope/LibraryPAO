#include "film.h"
#include <QDebug>
#include <QJsonArray>

// Costruttore
Film::Film(const QString& titolo, const QString& genere, int anno,
           const QString& regista, int durata, const QStringList& cast)
    : Media(titolo, genere, anno),
    regista(regista),
    durata(durata),
    cast(cast)
{
    qDebug() << "Creato film:" << titolo << "Regista:" << regista;
}

Film::Film() : Media(), regista(""), durata(0), cast() {}

// Distruttore (implementazione anche se 'default')
Film::~Film(){
    qDebug() << "Distrutto libro:" << getTitolo();
}

// Implementazione metodo polimorfo per generare ID
QString Film::generaId(int count) const {
    QString cleanRegista = regista.simplified().remove(' ').toUpper();
    QString cleanTitolo = getTitolo().simplified().remove(' ').toUpper();

    return QString("FIL-%1-%2-%3").arg(cleanTitolo, cleanRegista, QString::number(count));
}

// Implementazione metodi polimorfi
QString Film::mostraDettagli() const {
    QString dettagli = QString("%1\nRegista: %2\nDurata: %3 min\nCast: %4")
                           .arg(getTitolo(), regista)
                        .arg(durata)
                        .arg(cast.join(", "));
    return dettagli;
}

QDate Film::calcolaPrestito() const {
    // Film hanno prestito più breve (7 giorni)
    return QDate::currentDate().addDays(7);
}

QJsonObject Film::toJson() const {
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

    // Attributi specifici di Film
    obj["regista"] = regista;
    obj["durata"] = durata;

    QJsonArray castArray;
    for (const QString& attore : cast) {
        castArray.append(attore);
    }
    obj["cast"] = castArray;

    return obj;
}

Film Film::fromJson(const QJsonObject& obj) {
    // Costruzione base (può anche essere un Articolo() vuoto se serve)
    Film f;
    f.setTitolo(obj["titolo"].toString());
    f.setGenere(obj["genere"].toString());
    f.setAnno(obj["anno"].toInt());
    f.setId(obj["id"].toString());
    f.setDisponibilita(obj["disponibile"].toBool());
    f.setNprestiti(obj["nprestiti"].toInt());
    f.setProssimaDisponibilita(QDate::fromString(obj["proxDisp"].toString(), Qt::ISODate));

    // Attributi specifici di Film
    f.setRegista(obj["regista"].toString());
    f.setDurata(obj["durata"].toInt());

    QJsonArray castArray = obj["cast"].toArray();
    QStringList castList;
    for (const QJsonValue &val : std::as_const(castArray)) { // Per eliminare il warning: range-based ‘for’ loops with reference variables bind to temporary objects
        castList.append(val.toString());
    }

    f.setCast(castList);

    return f;
}


// SETTER implementations
void Film::setRegista(const QString& nuovoregista) {
    if(!nuovoregista.isEmpty()) {
        regista = nuovoregista;
    } else {
        qWarning() << "Nome regista non valido";
    }
}
void Film::setDurata(const int& nuovodurata) {
    if(nuovodurata > 0) {
        durata = nuovodurata;
    } else {
        qWarning() << "Durata non valida:" << nuovodurata << "minuti";
    }
}
void Film::setCast(const QStringList& nuovocast) {
    if(!nuovocast.isEmpty()) {
        cast = nuovocast;
    } else {
        qWarning() << "Lista cast vuota";
    }
}
