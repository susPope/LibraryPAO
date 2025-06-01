#include "mediaformwidget.h"
#include <QFormLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include "Project/libro.h"
#include "Project/film.h"
#include "Project/articolo.h"

MediaFormWidget::MediaFormWidget(QWidget *parent) : QWidget(parent) {
    // Radio button
    radioLibro = new QRadioButton("Libro", this);
    radioFilm = new QRadioButton("Film", this);
    radioArticolo = new QRadioButton("Articolo", this);

    radioLibro->setChecked(true); // default

    tipoGroup = new QButtonGroup(this);
    tipoGroup->addButton(radioLibro, 0);
    tipoGroup->addButton(radioFilm, 1);
    tipoGroup->addButton(radioArticolo, 2);

    QHBoxLayout* tipoLayout = new QHBoxLayout;
    tipoLayout->addWidget(radioLibro);
    tipoLayout->addWidget(radioFilm);
    tipoLayout->addWidget(radioArticolo);

    // Stack con form specifici
    stack = new QStackedWidget(this);
    stack->addWidget(creaFormLibro());    // index 0
    stack->addWidget(creaFormFilm());     // index 1
    stack->addWidget(creaFormArticolo()); // index 2

    // Main Layout
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(creaFormMedia());
    mainLayout->addLayout(tipoLayout);
    mainLayout->addWidget(stack);
    setLayout(mainLayout);

    // Connect
    connect(tipoGroup, &QButtonGroup::idClicked, stack, &QStackedWidget::setCurrentIndex);
}

QWidget* MediaFormWidget::creaFormMedia() {
    QWidget *widget = new QWidget;
    QFormLayout *layout = new QFormLayout(widget);
    titolo = new QLineEdit;
    genere = new QLineEdit;
    anno = new QSpinBox;
    anno->setRange(1700, QDate::currentDate().year()); // range anni valido
    anno->setValue(QDate::currentDate().year());
    layout->addRow("Titolo:", titolo);
    layout->addRow("Genere:", genere);
    layout->addRow("Anno:", anno);
    return widget;
}

QWidget* MediaFormWidget::creaFormLibro() {
    QWidget *widget = new QWidget;
    QFormLayout *layout = new QFormLayout(widget);
    autoreLib = new QLineEdit;
    editore = new QLineEdit;
    pagineLib = new QSpinBox;
    pagineLib->setRange(0, 99999);
    isbn = new QLineEdit;
    layout->addRow("Autore:", autoreLib);
    layout->addRow("Editore:", editore);
    layout->addRow("Pagine:", pagineLib);
    layout->addRow("ISBN:", isbn);
    return widget;
}

QWidget* MediaFormWidget::creaFormFilm() {
    QWidget *widget = new QWidget;
    QFormLayout *layout = new QFormLayout(widget);    
    regista = new QLineEdit;
    durata = new QSpinBox;
    durata->setRange(0, 99999);
    cast = new QLineEdit;
    layout->addRow("Regista:", regista);
    layout->addRow("Durata (minuti):", durata);
    layout->addRow("Cast:", cast);
    return widget;
}

QWidget* MediaFormWidget::creaFormArticolo() {
    QWidget *widget = new QWidget;
    QFormLayout *layout = new QFormLayout(widget);
    autoreArt = new QLineEdit;
    rivista = new QLineEdit;
    volume = new QSpinBox;
    volume->setRange(0, 99999);
    pagineArt = new QSpinBox;
    pagineArt->setRange(0, 99999);
    layout->addRow("Autore:", autoreArt);
    layout->addRow("Rivista:", rivista);
    layout->addRow("Volume:", volume);
    layout->addRow("Pagine:", pagineArt);
    return widget;
}

std::unique_ptr<Media> MediaFormWidget::creaMedia() const {
    QStringList list;

    switch (tipoGroup->checkedId()) {
    case 0:
        return std::make_unique<Libro>(
            titolo->text(), genere->text(), anno->value(),
            autoreLib->text(), editore->text(), pagineLib->value(), isbn->text());
    case 1:
        list = cast->text().split(",", Qt::SkipEmptyParts);
        for (QString &item : list) {
            item = item.trimmed();
        }
        return std::make_unique<Film>(
            titolo->text(), genere->text(), anno->value(),
            regista->text(), durata->value(), list);
    case 2:
        return std::make_unique<Articolo>(
            titolo->text(), genere->text(), anno->value(),
            autoreArt->text(), rivista->text(), volume->value(), pagineArt->value());
    default:
        return nullptr;
    }
}


//TODO: Finire implementazione se dovesse servire
void MediaFormWidget::cambiaTipo(int index) {
    // Esempio: mostra/nasconde widget in base al tipo selezionato
    qDebug() << "Tipo selezionato:" << index;
    // Implementazione reale qui...
}
