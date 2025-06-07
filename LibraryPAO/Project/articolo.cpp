#include "articolo.h"
#include <QDebug>
#include <QJsonValue>
#include <QDate>

// Costruttore
Articolo::Articolo(const QString& titolo, const QString& genere, int anno,
           const QString& autore, const QString& rivista, int volume, int pagine)
    : Media(titolo, genere, anno),
    autore(autore),
    rivista(rivista),
    volume(volume),
    pagine(pagine)
{
    qDebug() << "Creata rivista:" << titolo;
}

Articolo::Articolo() : Media(), autore(""), rivista(""), volume(0), pagine(0) {}

// Distruttore (implementazione anche se 'default')
Articolo::~Articolo(){
    qDebug() << "Distrutto rivista: " << getId();
}

// Implementazione metodo polimorfo per generare ID
QString Articolo::generaId(int count) const {
    QString cleanTitolo = getTitolo().simplified().remove(' ').toUpper();
    QString cleanAutore = autore.simplified().remove(' ').toUpper();

    return QString("ART-%1-%2-%3").arg(cleanTitolo, cleanAutore, QString::number(count));
}

// Implementazione metodi polimorfi
QString Articolo::mostraDettagli() const {
    return QString("%1\nAutore: %2\nRivista: %3 min\nVolume: %4\nPagine: %5")
        .arg(getTitolo(), autore, rivista).arg(volume).arg(pagine);
}

QDate Articolo::calcolaPrestito() const {
    // Articoli hanno prestito brevissimo (2 giorni)
    return QDate::currentDate().addDays(2);
}

QJsonObject Articolo::toJson() const {
    QJsonObject obj;

    // Attributi comuni (ipotizzando che esistano i getter)
    obj["titolo"] = getTitolo();
    obj["genere"] = getGenere();
    obj["anno"] = getAnno();
    obj["id"] = getId();
    obj["disponibile"] = getDisponibilita();
    obj["nprestiti"] = getNprestiti();
    obj["proxDisp"] = getProssimaDisponibilita().toString(Qt::ISODate);

    // Attributi specifici di Articolo
    obj["autore"] = autore;
    obj["rivista"] = rivista;
    obj["volume"] = volume;
    obj["pagine"] = pagine;

    return obj;
}

Articolo Articolo::fromJson(const QJsonObject& obj) {
    // Costruzione base (può anche essere un Articolo() vuoto se serve)
    Articolo a;
    a.setTitolo(obj["titolo"].toString());
    a.setGenere(obj["genere"].toString());
    a.setAnno(obj["anno"].toInt());
    a.setId(obj["id"].toString());
    a.setDisponibilita(obj["disponibile"].toBool());
    a.setNprestiti(obj["nprestiti"].toInt());
    a.setProssimaDisponibilita(QDate::fromString(obj["proxDisp"].toString(), Qt::ISODate));

    // Attributi specifici di Articolo
    a.setAutore(obj["autore"].toString());
    a.setRivista(obj["rivista"].toString());
    a.setVolume(obj["volume"].toInt());
    a.setPagine(obj["pagine"].toInt());

    return a;
}

// SETTER implementations
void Articolo::setAutore(const QString& nuovoautore) {
    if(!nuovoautore.isEmpty()) {
        autore = nuovoautore;
    } else {
        qWarning() << "Nome autore non valido";
    }
}
void Articolo::setRivista(const QString& nuovorivista) {
    if(!nuovorivista.isEmpty()) {
        rivista = nuovorivista;
    } else {
        qWarning() << "Nome rivista non valido";
    }
}
void Articolo::setVolume(const int& nuovovolume) {
    if(nuovovolume > 0) {
        volume = nuovovolume;
    } else {
        qWarning() << "Numero volume non valido";
    }
}
void Articolo::setPagine(const int& nuovopagine) {
    if(nuovopagine > 0) {
        pagine = nuovopagine;
    } else {
        qWarning() << "Numero pagine non valido";
    }
}
