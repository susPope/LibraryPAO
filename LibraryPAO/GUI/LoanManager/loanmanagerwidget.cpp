#include "loanmanagerwidget.h"
#include "GUI/LoanManager/loanviewwidget.h"
#include "Project/mediarepo.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QShortcut>
#include <QKeySequence>

LoanManagerWidget::LoanManagerWidget(QWidget *parent) : QWidget(parent) {
    searchWidget = new SearchWidget(this); // Barra di ricerca

    loanList = new QListWidget(this);

    addButton = new QPushButton("Aggiungi Prestito", this);
    returnButton = new QPushButton("Restituisci", this);

    disponibilitaComboBox = new QComboBox(this);
    disponibilitaComboBox->addItems({"In prestito", "Disponibili", "Tutti", "In ritardo"});

    QHBoxLayout* searchLayout = new QHBoxLayout;
    searchLayout->addWidget(disponibilitaComboBox);
    searchLayout->addWidget(searchWidget);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(returnButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(searchLayout);
    mainLayout->addWidget(loanList);
    mainLayout->addLayout(buttonLayout);

    setLayout(mainLayout);
    setWindowTitle("Gestione Prestiti");

    // Dati di esempio (usa MediaRepo o la tua fonte dati reale)
    const auto& tuttiIMedia = MediaRepo::instance().getTuttiIMedia();

    for (const auto& ptr : tuttiIMedia) {
        Media* m = ptr.get();

        // Creiamo item e LoanViewWidget come nel MediaManagerWidget
        QListWidgetItem* item = new QListWidgetItem();
        LoanViewWidget* widget = new LoanViewWidget(m);
        item->setSizeHint(widget->sizeHint());

        loanList->addItem(item);
        loanList->setItemWidget(item, widget);

        // Salviamo puntatore media in Qt::UserRole, per riconoscerlo dopo
        item->setData(Qt::UserRole, QVariant::fromValue(reinterpret_cast<quintptr>(m)));
    }

    // Shortcut per addLoan: Ctrl + P
    QShortcut* shortcutAdd = new QShortcut(QKeySequence("Ctrl+P"), this);
    connect(shortcutAdd, &QShortcut::activated, this, &LoanManagerWidget::addLoan);

    // Shortcut per returnLoan: Ctrl + R
    QShortcut* shortcutEdit = new QShortcut(QKeySequence("Ctrl+R"), this);
    connect(shortcutEdit, &QShortcut::activated, this, &LoanManagerWidget::returnLoan);

    connect(addButton, &QPushButton::clicked, this, &LoanManagerWidget::addLoan);
    connect(returnButton, &QPushButton::clicked, this, &LoanManagerWidget::returnLoan);
    connect(searchWidget, &SearchWidget::ricercaAvviata, this, &LoanManagerWidget::onRicercaAvviata);
    connect(disponibilitaComboBox, &QComboBox::currentTextChanged, this, &LoanManagerWidget::aggiornaRicerca);
}

void LoanManagerWidget::addLoan() {
    QListWidgetItem *item = loanList->currentItem();
    if (!item) {
        QMessageBox::warning(this, "Modifica", "Seleziona un media da prestare.");
        return;
    }

    Media* media = getMediaFromItem(item);
    if (!media) return;

    QString errore = MediaRepo::instance().aggiungiPrestito(media);
    if (!errore.isEmpty()) {
        QMessageBox::critical(this, "Errore durante il prestito", errore);
    }

    LoanViewWidget* view = qobject_cast<LoanViewWidget*>(loanList->itemWidget(item));
    if (view) {
        view->aggiorna();
    }
}

Media* LoanManagerWidget::getMediaFromItem(QListWidgetItem* item) {
    if (!item) return nullptr;
    quintptr ptrVal = item->data(Qt::UserRole).value<quintptr>();
    return reinterpret_cast<Media*>(ptrVal);
}

void LoanManagerWidget::returnLoan() {
    QListWidgetItem *item = loanList->currentItem();
    if (!item) {
        QMessageBox::warning(this, "Restituzione", "Seleziona un prestito da restituire.");
        return;
    }

    Media* media = getMediaFromItem(item);
    if (!media) return;

    QString errore = MediaRepo::instance().restituisciPrestito(media);
    if (!errore.isEmpty()) {
        QMessageBox::critical(this, "Errore durante la restituzione", errore);
    }

    LoanViewWidget* view = qobject_cast<LoanViewWidget*>(loanList->itemWidget(item));
    if (view) {
        view->aggiorna();
    }
}

void LoanManagerWidget::onRicercaAvviata(const QString& testo, const QString& criterio) {
    loanList->clear();

    QString filtro = disponibilitaComboBox->currentText();  // "In prestito", "Disponibili", "Tutti"
    auto risultati = MediaRepo::instance().cercaPrestiti(testo, criterio, filtro);

    for (Media* m : risultati) {
        QListWidgetItem* item = new QListWidgetItem();
        LoanViewWidget* widget = new LoanViewWidget(m);
        item->setSizeHint(widget->sizeHint());
        loanList->addItem(item);
        loanList->setItemWidget(item, widget);
        item->setData(Qt::UserRole, QVariant::fromValue(reinterpret_cast<quintptr>(m)));
    }
}

void LoanManagerWidget::aggiornaRicerca() {
    // Reinvoca la ricerca con i parametri attuali
    onRicercaAvviata(searchWidget->getTestoRicerca(), searchWidget->getCriterioRicerca());
}
