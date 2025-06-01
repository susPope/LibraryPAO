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
    std::unique_ptr<Media> nuovo = mediaForm->creaMedia(); // ipotetico metodo che ritorna unique_ptr

    if (nuovo) {
        // Salviamo raw pointer per metterlo nella QListWidgetItem
        Media* rawPtr = nuovo.get();

        // Spostiamo la unique_ptr dentro la repo
        MediaRepo::instance().aggiungiMedia(std::move(nuovo));

        QListWidgetItem* item = new QListWidgetItem(rawPtr->getTitolo());
        item->setData(Qt::UserRole, QVariant::fromValue(reinterpret_cast<quintptr>(rawPtr)));
        mediaList->addItem(item);
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
    int row = mediaList->currentRow();
    QListWidgetItem *item = mediaList->takeItem(row);
    if (item) {
        Media* media = getMediaFromItem(item);
        if (media) {
            MediaRepo::instance().rimuoviMedia(media);
        }
        delete item;
    } else {
        QMessageBox::warning(this, "Elimina", "Seleziona un media da eliminare.");
    }
}

Media* MediaManagerWidget::getMediaFromItem(QListWidgetItem* item) {
    if (!item) return nullptr;
    quintptr ptrVal = item->data(Qt::UserRole).value<quintptr>();
    return reinterpret_cast<Media*>(ptrVal);
}
