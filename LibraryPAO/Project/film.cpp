#include "film.h"
#include <QDebug>

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

// Distruttore (implementazione anche se 'default')
Film::~Film(){
    qDebug() << "Distrutto libro:" << getTitolo();
}

// Implementazione metodo polimorfo per generare ID
QString Film::generaId() const {
    return QString("FIL-%1-%2").arg(regista.left(3).toUpper(), getAnno());
}

/* Implementazione metodi polimorfi
QString Film::mostraDettagli() const {
    QString dettagli = QString("%1\nRegista: %2\nDurata: %3 min\nCast: %4")
                        .arg(titolo, regista)
                        .arg(durata)
                        .arg(cast.join(", "));
    return dettagli;
}

QDate Film::calcolaPrestito(int days) const {
    // Film hanno prestito più breve (es. 7 giorni)
    return QDate::currentDate().addDays(days > 7 ? 7 : days);
}
*/

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
