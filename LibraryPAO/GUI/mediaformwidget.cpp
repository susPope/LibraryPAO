#include "mediaformwidget.h"
#include <QFormLayout>
#include <QVBoxLayout>

#include "Project/libro.h"
#include "Project/film.h"
#include "Project/articolo.h"

MediaFormWidget::MediaFormWidget(QWidget *parent) : QWidget(parent) {
    tipoBox = new QComboBox(this);
    tipoBox->addItem("Libro");
    tipoBox->addItem("Film");
    tipoBox->addItem("Articolo");



    // Stack con form specifici
    stack = new QStackedWidget(this);
    stack->addWidget(creaFormLibro());    // index 0
    stack->addWidget(creaFormFilm());     // index 1
    stack->addWidget(creaFormArticolo()); // index 2

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(tipoBox);
    mainLayout->addWidget(creaFormMedia());
    mainLayout->addWidget(stack);
    setLayout(mainLayout);

    connect(tipoBox, QOverload<int>::of(&QComboBox::currentIndexChanged), stack, &QStackedWidget::setCurrentIndex);
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
    genere = new QLineEdit;
    pagineLib = new QSpinBox;
    pagineLib->setRange(0, 99999);
    isbn = new QLineEdit;
    layout->addRow("Autore:", autoreLib);
    layout->addRow("Genere:", genere);
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

Media* MediaFormWidget::creaMedia() const {
    QStringList list;

    switch (tipoBox->currentIndex()) {
    case 0:
        return new Libro(titolo->text(), genere->text(), anno->value(),
            autoreLib->text(), editore->text(), pagineLib->value(), isbn->text());
    case 1:
        list = cast->text().split(",", Qt::SkipEmptyParts);
        for (QString &item : list) {
            item = item.trimmed();      // Rimuove spazi iniziali e finali da ogni elemento
        }
        return new Film(titolo->text(), genere->text(), anno->value(),
            regista->text(), durata->value(), list);
    case 2:
        return new Articolo(titolo->text(), genere->text(), anno->value(),
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
