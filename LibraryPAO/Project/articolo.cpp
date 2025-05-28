#include "articolo.h"
#include <QDebug>

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

// Distruttore (implementazione anche se 'default')
Articolo::~Articolo(){
    qDebug() << "Distrutto rivista: " << getId();
}

// Implementazione metodo polimorfo per generare ID
    QString Articolo::generaId() const {
        return QString("ART-%1-%2").arg(rivista.left(3).toUpper(), (volume+getAnno()));
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
