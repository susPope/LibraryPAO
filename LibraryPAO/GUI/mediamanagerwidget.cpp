#include "mediamanagerwidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QListWidget>
#include <QPushButton>
#include <QMessageBox>

MediaManagerWidget::MediaManagerWidget(QWidget *parent) : QWidget(parent) {
    mediaList = new QListWidget(this);

    addButton = new QPushButton("Aggiungi", this);
    editButton = new QPushButton("Modifica", this);
    deleteButton = new QPushButton("Elimina", this);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(editButton);
    buttonLayout->addWidget(deleteButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(mediaList);
    mainLayout->addLayout(buttonLayout);

    setLayout(mainLayout);
    setWindowTitle("Gestione Media");

    // Connessioni
    connect(addButton, SIGNAL(clicked()), this, SLOT(addMedia()));
    connect(editButton, SIGNAL(clicked()), this, SLOT(editSelectedMedia()));
    connect(deleteButton, SIGNAL(clicked()), this, SLOT(deleteSelectedMedia()));
}

void MediaManagerWidget::addMedia() {
    // In versione reale apriresti una dialog per inserire i dettagli
    QString nuovoMedia = "Nuovo Media"; // placeholder
    mediaList->addItem(nuovoMedia);

    //TODO: creazione oggetto media
    //TODO: selezione tipo di oggetto
    //TODO: mostrare attributi da compilare in base al tipo
    //TODO: aggiungere oggetto al database

}

void MediaManagerWidget::editSelectedMedia() {
    QListWidgetItem *item = mediaList->currentItem();
    if (item) {
        item->setText("Media modificato"); // in realtà apriresti una dialog di modifica
    } else {
        QMessageBox::warning(this, "Modifica", "Seleziona un media da modificare.");
    }
}

void MediaManagerWidget::deleteSelectedMedia() {
    QListWidgetItem *item = mediaList->takeItem(mediaList->currentRow());
    if (item) {
        delete item;
    } else {
        QMessageBox::warning(this, "Elimina", "Seleziona un media da eliminare.");
    }
}
