#ifndef MEDIA_H
#define MEDIA_H

#include <QString>
#include <QDate>

class Media {
private:
    QString titolo;
    QString genere;
    int anno;
    QString id;         // Identificatore unico
    bool disponibile;   //default = true
    int nprestiti;
    QDate proxDisp;     // Data dalla quale sarà possibile prender in prestito nuovamente

public:
    // Costruttori / Distruttori
    Media(const QString &titolo, const QString &genere, int anno,
          bool disponibile = true, int nprestiti = 0);
    virtual ~Media() = default; // Distruttore puro -> classe astratta

    /* Metodi:
        - prenota
        - restituisci
        - ???
    */

    // Metodi polimorfi (non banali)
    virtual QString mostraDettagli() const = 0;
    virtual QDate calcolaPrestito(int days) const = 0;
    virtual QString generaId() const = 0;
    virtual QJsonObject toJson() const = 0;
    // Metodo polimorfo #3: Creazione widget UI
    //virtual QWidget* createDetailsWidget(QWidget* parent = nullptr) const = 0;

    // GETTER
    QString getTitolo() const { return titolo; }
    QString getGenere() const { return genere; }
    int getAnno() const { return anno; }
    QString getId() const {return id; }
    bool getDisponibilita() const { return disponibile; }
    int getNprestiti() const { return  nprestiti; }
    QDate getProssimaDisponibilita() const { return proxDisp; }

    // SETTER
    void setTitolo(const QString& nuovotitolo);
    void setGenere(const QString& nuovogenere);
    void setAnno(const int& nuovoanno);
    //QDate setProssimaDisponibilita(const QDate& nuovaproxdisponibilita);
};

#endif // MEDIA_H
