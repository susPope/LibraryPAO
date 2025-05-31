#ifndef MEDIAFORMWIDGET_H
#define MEDIAFORMWIDGET_H

#include <QWidget>
#include <QComboBox>
#include <QStackedWidget>
#include <QLineEdit>
#include <QFormLayout>
#include <QDate>
#include <QSpinBox>

#include "Project/media.h"

class MediaFormWidget : public QWidget {
    Q_OBJECT

public:
    explicit MediaFormWidget(QWidget *parent = nullptr);
    Media* creaMedia() const;

private slots:
    void cambiaTipo(int index);

private:
    // Selettore tipo media
    QComboBox *tipoBox;
    QStackedWidget *stack;

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
    QLineEdit *cast;

    // Campi articolo
    QLineEdit *autoreArt;
    QLineEdit *rivista;
    QSpinBox *volume;
    QSpinBox *pagineArt;

    // Metodi per creare le form
    QWidget* creaFormMedia();
    QWidget* creaFormLibro();
    QWidget* creaFormFilm();
    QWidget* creaFormArticolo();
};

#endif // MEDIAFORMWIDGET_H
