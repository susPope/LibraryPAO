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
    bool disponibile;   // default = true
    int nprestiti;
    QDate proxDisp;     // Data dalla quale sarà possibile prender in prestito nuovamente

public:
    // Costruttori / Distruttori
    Media(const QString &titolo, const QString &genere, int anno,
        bool disponibile = true, int nprestiti = 0);
    Media() : titolo(""), genere(""), anno(0), id(""), disponibile(true),
        nprestiti(0), proxDisp(QDate::currentDate()){}
    virtual ~Media() = default; // Distruttore puro -> classe astratta

    // Metodi polimorfi (non banali)
    virtual QDate calcolaPrestito() const = 0;
    virtual QString generaId(int count) const = 0;
    virtual QJsonObject toJson() const = 0;
    virtual QString getTipoIcona() const = 0;
    virtual QString getDettagliString() const = 0;
    virtual QString getIconaPath(bool darkTheme) const = 0;

    bool isInRitardo() const;

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
    void setId(const QString& nuovoId);
    void setDisponibilita(bool nuovaDisponibilita);
    void setNprestiti(int nuovoNprestiti);
    void setProssimaDisponibilita(const QDate& nuovaproxdisponibilita);
};

#endif // MEDIA_H
