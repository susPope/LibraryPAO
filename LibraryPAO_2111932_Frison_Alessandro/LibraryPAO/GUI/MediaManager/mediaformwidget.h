#ifndef MEDIAFORMWIDGET_H
#define MEDIAFORMWIDGET_H

#include "Project/media.h"

#include <QRadioButton>
#include <QStackedWidget>
#include <QLineEdit>
#include <QFormLayout>
#include <QDate>
#include <QSpinBox>
#include <QButtonGroup>
#include <QPushButton>
#include <QTextEdit>
#include <memory>

class MediaFormWidget : public QWidget {
    Q_OBJECT

public:
    explicit MediaFormWidget(QWidget *parent = nullptr);
    std::unique_ptr<Media> creaMedia() const;
    void pulisciCampi();

    // Metodi per il modifica Media (editMedia())
    void caricaMedia(Media* media);
    QString aggiornaMedia(Media* media);
    QString getTipoSelezionato();

    const std::type_info& getTipoSelezionatoTypeInfo() const;

private:
    // Selettore tipo media
    QRadioButton* radioLibro;
    QRadioButton* radioFilm;
    QRadioButton* radioArticolo;
    QButtonGroup* tipoGroup;
    QStackedWidget *stack;

    // Campi media
    QLineEdit *titolo;
    QLineEdit *genere;
    QSpinBox *anno;

    // Campi libro
    QLineEdit *autoreLib;
    QLineEdit *editore;
    QSpinBox *pagineLib;
    QLineEdit *isbn;

    // Campi film
    QLineEdit *regista;
    QSpinBox *durata;
    QTextEdit *cast;

    // Campi articolo
    QLineEdit *autoreArt;
    QLineEdit *rivista;
    QSpinBox *volume;
    QSpinBox *pagineArt;

    // Campi per id
    QLineEdit* id;

    // Metodi per creare i form
    QWidget* creaFormMedia();
    QWidget* creaFormLibro();
    QWidget* creaFormFilm();
    QWidget* creaFormArticolo();
};

#endif // MEDIAFORMWIDGET_H
