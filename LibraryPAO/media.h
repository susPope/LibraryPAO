#ifndef MEDIA_H
#define MEDIA_H

#include <QString>
#include <QDate>

class Media {
public:
    // Costruttori / Distruttori (TODO: verificare se servono)
    Media();
    virtual ~Media();

    // Metodi polimorfi (non banali)
    virtual QString mostraDettagli() const = 0;
    virtual QDate calcolaPrestito(int days) const = 0;
    // Metodo polimorfo #2: Creazione widget UI
    //virtual QWidget* createDetailsWidget(QWidget* parent = nullptr) const = 0;

    // Getters per attributi comuni
    QString getTitolo() const { return titolo; }
    int getAnno() const { return anno; }
    QString getId() const { return id; }
    bool isDisponibile() const { return disponibile; }
    QDate getProssimaDisponibilita() const { return proxDisp; }

protected:
    QString titolo;
    int anno;
    QString id;     // Identificatore unico
    bool disponibile;
    QDate proxDisp; // Data dalla quale sarà possibile prender in prestito nuovamente
};

#endif // MEDIA_H
