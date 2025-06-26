#include "mediaformwidget.h"
#include <QFormLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>

#include "Project/libro.h"
#include "Project/film.h"
#include "Project/articolo.h"
#include "Project/mediarepo.h"

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
    id = new QLineEdit(this);
    id->setReadOnly(true);
    anno->setRange(1700, QDate::currentDate().year()); // range anni valido
    anno->setValue(QDate::currentDate().year());
    layout->addRow("Titolo:", titolo);
    layout->addRow("Genere:", genere);
    layout->addRow("Anno:", anno);
    layout->addRow("ID:", id);
    return widget;
}

QWidget* MediaFormWidget::creaFormLibro() {
    QWidget *widget = new QWidget;
    QFormLayout *layout = new QFormLayout(widget);
    autoreLib = new QLineEdit;
    editore = new QLineEdit;
    pagineLib = new QSpinBox;
    pagineLib->setRange(1, 99999);
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
    durata->setRange(1, 99999);
    cast = new QTextEdit;
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
    volume->setRange(1, 99999);
    pagineArt = new QSpinBox;
    pagineArt->setRange(1, 99999);
    layout->addRow("Autore:", autoreArt);
    layout->addRow("Rivista:", rivista);
    layout->addRow("Volume:", volume);
    layout->addRow("Pagine:", pagineArt);
    return widget;
}

std::unique_ptr<Media> MediaFormWidget::creaMedia() const {
    auto isEmpty = [](const QString& s) { return s.trimmed().isEmpty(); };

    switch (tipoGroup->checkedId()) {
    case 0: { // Libro
        if (isEmpty(titolo->text()) || isEmpty(genere->text()) ||
            isEmpty(autoreLib->text()) || isEmpty(editore->text()) ||
            isEmpty(isbn->text()))
        {
            QMessageBox::warning(nullptr, "Errore", "Compila tutti i campi del libro.");
            return nullptr;
        }
        if (!MediaRepo::instance().checkLibro(isbn->text())) {
            QMessageBox::warning(nullptr, "Errore", "ISBN non valido o già presente.");
            return nullptr;
        }

        QString cleanIsbn = isbn->text().remove(' ').remove('-');

        return std::make_unique<Libro>(
            titolo->text(), genere->text(), anno->value(),
            autoreLib->text(), editore->text(), pagineLib->value(), cleanIsbn);
    }
    case 1: { // Film
        if (isEmpty(titolo->text()) || isEmpty(genere->text()) ||
            isEmpty(regista->text()) || isEmpty(cast->toPlainText()))
        {
            QMessageBox::warning(nullptr, "Errore", "Compila tutti i campi del film.");
            return nullptr;
        }
        QStringList list = cast->toPlainText().split(",", Qt::SkipEmptyParts);
        for (QString& item : list)
            item = item.trimmed();

        return std::make_unique<Film>(
            titolo->text(), genere->text(), anno->value(),
            regista->text(), durata->value(), list);
    }
    case 2: { // Articolo
        if (isEmpty(titolo->text()) || isEmpty(genere->text()) ||
            isEmpty(autoreArt->text()) || isEmpty(rivista->text()))
        {
            QMessageBox::warning(nullptr, "Errore", "Compila tutti i campi dell'articolo.");
            return nullptr;
        }
        return std::make_unique<Articolo>(
            titolo->text(), genere->text(), anno->value(),
            autoreArt->text(), rivista->text(), volume->value(), pagineArt->value());
    }
    default:
        QMessageBox::warning(nullptr, "Errore", "Tipo di media non selezionato.");
        return nullptr;
    }
}


void MediaFormWidget::caricaMedia(Media* media) {
    if (!media) return;

    // Campi comuni
    titolo->setText(media->getTitolo());
    genere->setText(media->getGenere());
    anno->setValue(media->getAnno());
    id->setText(media->getId());

    // Cast ai sottotipi e riempi form specifico
    if (Libro* l = dynamic_cast<Libro*>(media)) {
        tipoGroup->button(0)->setChecked(true);
        stack->setCurrentIndex(0);
        autoreLib->setText(l->getAutore());
        editore->setText(l->getEditore());
        pagineLib->setValue(l->getPagine());
        isbn->setText(l->getIsbn());
    } else if (Film* f = dynamic_cast<Film*>(media)) {
        tipoGroup->button(1)->setChecked(true);
        stack->setCurrentIndex(1);
        regista->setText(f->getRegista());
        durata->setValue(f->getDurata());
        cast->setText(f->getCast().join(", "));
    } else if (Articolo* a = dynamic_cast<Articolo*>(media)) {
        tipoGroup->button(2)->setChecked(true);
        stack->setCurrentIndex(2);
        autoreArt->setText(a->getAutore());
        rivista->setText(a->getRivista());
        volume->setValue(a->getVolume());
        pagineArt->setValue(a->getPagine());
    }
}

QString MediaFormWidget::aggiornaMedia(Media* media) {
    if (!media) return "Media non riconosciuto.";

    // Campi comuni obbligatori
    if (titolo->text().isEmpty() || genere->text().isEmpty()) {
        return "Compila tutti i campi comuni: titolo e genere.";
    }

    if (Libro* l = dynamic_cast<Libro*>(media)) {
        // Controlli specifici Libro
        if (autoreLib->text().isEmpty() || editore->text().isEmpty() || isbn->text().isEmpty()) {
            return "Compila tutti i campi del libro.";
        }
        if (!MediaRepo::instance().checkLibro(isbn->text())) {
            return "ISBN non valido o già presente.";
        }

        QString cleanIsbn = isbn->text().remove(' ').remove('-');
        l->setAutore(autoreLib->text());
        l->setEditore(editore->text());
        l->setPagine(pagineLib->value());
        l->setIsbn(cleanIsbn);
    } else if (Film* f = dynamic_cast<Film*>(media)) {
        // Controlli specifici Film
        if (regista->text().isEmpty() || cast->toPlainText().isEmpty()) {
            return "Compila tutti i campi del film.";
        }

        f->setRegista(regista->text());
        f->setDurata(durata->value());

        QStringList list = cast->toPlainText().split(",", Qt::SkipEmptyParts);
        for (QString& s : list) s = s.trimmed();
        f->setCast(list);
    } else if (Articolo* a = dynamic_cast<Articolo*>(media)) {
        // Controlli specifici Articolo
        if (autoreArt->text().isEmpty() || rivista->text().isEmpty()) {
            return "Compila tutti i campi dell'articolo.";
        }

        a->setAutore(autoreArt->text());
        a->setRivista(rivista->text());
        a->setVolume(volume->value());
        a->setPagine(pagineArt->value());
    } else {
        // Tipo di media sconosciuto
        return "Tipo media sconosciuto.";
    }

    media->setTitolo(titolo->text());
    media->setGenere(genere->text());
    media->setAnno(anno->value());

    // Genera nuovo ID univoco
    int count = MediaRepo::instance().countMedia(media);
    media->setId(media->generaId(count));

    return "";
}


QString MediaFormWidget::getTipoSelezionato() {
    int id = tipoGroup->checkedId();
    switch(id) {
    case 0: return "Libro";
    case 1: return "Film";
    case 2: return "Articolo";
    default: return "";
    }
}

void MediaFormWidget::pulisciCampi() {
    titolo->clear();
    genere->clear();
    anno->setValue(2025);  // o un valore neutro
    id->clear();
    int idCheck = tipoGroup->checkedId();
    //radioLibro->setChecked(true);
    //radioFilm->setChecked(false);
    //radioArticolo->setChecked(false);
    // altri campi (tutti i campi di libro, film, articolo)
    switch(idCheck) {
        case 0: //Libro
            autoreLib->clear();
            editore->clear();
            pagineLib->setValue(0);
            isbn->clear();
            break;
        case 1: // Film
            regista->clear();
            durata->setValue(0);
            cast->clear();
            break;
        case 2: // Articolo
            autoreArt->clear();
            rivista->clear();
            volume->setValue(0);
            pagineArt->setValue(0);
            break;
        default:
            break;
    }
}
