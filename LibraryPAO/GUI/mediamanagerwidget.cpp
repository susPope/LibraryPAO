#include "Project/media.h"
#include "mediamanagerwidget.h"
#include "Project/mediarepo.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QListWidget>
#include <QPushButton>
#include <QMessageBox>

MediaManagerWidget::MediaManagerWidget(QWidget *parent) : QWidget(parent) {
    mediaList = new QListWidget(this);

    mediaForm = new MediaFormWidget(this);

    addButton = new QPushButton("Aggiungi", this);
    editButton = new QPushButton("Modifica", this);
    deleteButton = new QPushButton("Elimina", this);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(editButton);
    buttonLayout->addWidget(deleteButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(mediaList);
    mainLayout->addWidget(mediaForm);
    mainLayout->addLayout(buttonLayout);

    setLayout(mainLayout);
    setWindowTitle("Gestione Media");

    // Connessioni
    connect(addButton, SIGNAL(clicked()), this, SLOT(addMedia()));
    connect(editButton, SIGNAL(clicked()), this, SLOT(editSelectedMedia()));
    connect(deleteButton, SIGNAL(clicked()), this, SLOT(deleteSelectedMedia()));
}

void MediaManagerWidget::addMedia() {
    Media* nuovo = mediaForm->creaMedia();
    if (nuovo) {
        mediaList->addItem(nuovo->getTitolo()); // o come hai chiamato il getter del titolo
        MediaRepo::instance().aggiungiMedia(nuovo);  // se usi la repo singleton
    }

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
