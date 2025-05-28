#include "searchwidget.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QListWidget>
#include <QString>
#include <QMessageBox>

SearchWidget::SearchWidget(QWidget *parent)
    : QWidget(parent)
{
    searchInput = new QLineEdit(this);
    searchButton = new QPushButton("Cerca", this);
    resultsList = new QListWidget(this);

    QHBoxLayout *searchLayout = new QHBoxLayout;
    searchLayout->addWidget(searchInput);
    searchLayout->addWidget(searchButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(searchLayout);
    mainLayout->addWidget(resultsList);

    setLayout(mainLayout);
    setWindowTitle("Ricerca Media");

    connect(searchButton, SIGNAL(clicked()), this, SLOT(performSearch()));
}

void SearchWidget::performSearch()
{
    QString query = searchInput->text().trimmed();

    resultsList->clear();

    if (query.isEmpty()) {
        QMessageBox::information(this, "Ricerca", "Inserisci un termine di ricerca.");
        return;
    }

    // Simulazione risultati fittizi
    QStringList mediaFinti;
    mediaFinti << "Libro: Il Signore degli Anelli"
               << "Film: Il Padrino"
               << "Articolo: Quantum Computing 101"
               << "Libro: La Divina Commedia"
               << "Film: Matrix";

    for (int i = 0; i < mediaFinti.size(); ++i) {
        if (mediaFinti[i].contains(query, Qt::CaseInsensitive)) {
            resultsList->addItem(mediaFinti[i]);
        }
    }

    if (resultsList->count() == 0) {
        resultsList->addItem("Nessun risultato trovato.");
    }
}
