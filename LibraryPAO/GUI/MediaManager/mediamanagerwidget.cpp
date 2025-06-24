#include "mediamanagerwidget.h"
#include "Project/media.h"
#include "Project/libro.h"
#include "Project/film.h"
#include "Project/articolo.h"
#include "Project/mediarepo.h"
#include "GUI/MediaManager/mediaviewwidget.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

MediaManagerWidget::MediaManagerWidget(QWidget *parent) : QWidget(parent) {
    mediaList = new QListWidget(this);

    mediaForm = new MediaFormWidget(this);

    addButton = new QPushButton("Aggiungi", this);
    editButton = new QPushButton("Modifica", this);
    deleteButton = new QPushButton("Elimina", this);
    importDBButton = new QPushButton("Importa DataBase", this);
    deleteDBButton = new QPushButton("Elimina DataBase", this);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(editButton);
    buttonLayout->addWidget(deleteButton);
    buttonLayout->addWidget(importDBButton);
    buttonLayout->addWidget(deleteDBButton);

    searchWidget = new SearchWidget(this); // Barra di ricerca

    QHBoxLayout *centralLayout = new QHBoxLayout;
    centralLayout->addWidget(mediaList);
    centralLayout->addWidget(mediaForm);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(searchWidget);
    mainLayout->addLayout(centralLayout);
    mainLayout->addLayout(buttonLayout);

    setLayout(mainLayout);
    setWindowTitle("Gestione Media");

    const auto& tuttiIMedia = MediaRepo::instance().getTuttiIMedia();
    for (const auto& ptr : tuttiIMedia) {
        Media* m = ptr.get();

        QListWidgetItem* item = new QListWidgetItem();
        MediaViewWidget* widget = new MediaViewWidget(m);
        item->setSizeHint(widget->sizeHint());
        mediaList->addItem(item);
        mediaList->setItemWidget(item, widget);
        item->setData(Qt::UserRole, QVariant::fromValue(reinterpret_cast<quintptr>(m)));
    }

    // Connessioni
    connect(addButton, SIGNAL(clicked()), this, SLOT(addMedia()));
    connect(editButton, SIGNAL(clicked()), this, SLOT(editSelectedMedia()));
    connect(deleteButton, SIGNAL(clicked()), this, SLOT(deleteSelectedMedia()));
    connect(importDBButton, SIGNAL(clicked()), this, SLOT(importDB()));
    connect(deleteDBButton, SIGNAL(clicked()), this, SLOT(deleteAllMedia()));
    connect(mediaList, &QListWidget::itemClicked, this, &MediaManagerWidget::populateFormFromSelected);
    connect(searchWidget, &SearchWidget::ricercaAvviata, this, &MediaManagerWidget::onRicercaAvviata);
}

void MediaManagerWidget::addMedia() {
    std::unique_ptr<Media> nuovo = mediaForm->creaMedia();

    if (!nuovo)
        return;

    // Genera ID univoco
    int count = MediaRepo::instance().countMedia(nuovo.get());
    nuovo->setId(nuovo->generaId(count));

    // Aggiunge alla repository
    Media* rawPtr = nuovo.get();
    MediaRepo::instance().aggiungiMedia(std::move(nuovo));

    // Aggiorna interfaccia
    //QListWidgetItem* item = new QListWidgetItem(rawPtr->getTitolo());
    //item->setData(Qt::UserRole, QVariant::fromValue(reinterpret_cast<quintptr>(rawPtr)));
    //mediaList->addItem(item);

    QListWidgetItem* item = new QListWidgetItem();
    MediaViewWidget* widget = new MediaViewWidget(rawPtr);

    item->setSizeHint(widget->sizeHint());
    mediaList->addItem(item);
    mediaList->setItemWidget(item, widget);

    // Per salvataggio rawPtr nel QListWidgetItem
    item->setData(Qt::UserRole, QVariant::fromValue(reinterpret_cast<quintptr>(rawPtr)));


    mediaForm->pulisciCampi();
}

void MediaManagerWidget::editSelectedMedia() {
    QListWidgetItem *item = mediaList->currentItem();
    if (!item) {
        QMessageBox::warning(this, "Modifica", "Seleziona un media da modificare.");
        return;
    }

    Media* media = getMediaFromItem(item);
    if (!media) return;

    //controllo sul radio button (TODO: spostare in mediarepo)
    QString tipoOriginale;
    if (dynamic_cast<Libro*>(media)) tipoOriginale = "Libro";
    else if (dynamic_cast<Film*>(media)) tipoOriginale = "Film";
    else if (dynamic_cast<Articolo*>(media)) tipoOriginale = "Articolo";

    QString tipoForm = mediaForm->getTipoSelezionato();  // "Libro", "Film", "Articolo"

    if (tipoForm != tipoOriginale) {
        QMessageBox::warning(this, "Modifica non valida", "Non puoi cambiare il tipo del media.");
        return;
    }

    if (mediaForm->aggiornaMedia(media)) {
        QWidget* w = mediaList->itemWidget(item);  // ottieni il widget associato all'elemento selezionato
        if (auto* view = dynamic_cast<MediaViewWidget*>(w)) {
            view->aggiorna();  // chiami un metodo che rilegge i dati da 'media' e li mostra
        }
        //item->setText(media->getTitolo());
        MediaRepo::instance().aggiornaMedia(media);
        mediaForm->pulisciCampi();
    } else {
        QMessageBox::warning(this, "Modifica", "Errore durante l'aggiornamento del media.");
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
        QWidget* widget = mediaList->itemWidget(item);
        if (widget) {
            mediaList->removeItemWidget(item);
            widget->deleteLater();  // evita memory leak
        }
        delete item;
        mediaForm->pulisciCampi();
    } else {
        QMessageBox::warning(this, "Elimina", "Seleziona un media da eliminare.");
    }
}

void MediaManagerWidget::importDB() {
    //codice per aprire la comunicazione con il so e richiedere l'importazione di un json
    QString fileName = QFileDialog::getOpenFileName(this, "Importa Database", "", "File JSON (*.json)");
    if (fileName.isEmpty())
        return;

    // Imposta il nuovo path nel MediaRepo
    MediaRepo::instance().setPath(fileName);
    mediaForm->pulisciCampi();
    mediaList->clear();  // svuota la lista visuale
    MediaRepo::instance().importaDB();

    const auto& tuttiIMedia = MediaRepo::instance().getTuttiIMedia();
    for (const auto& ptr : tuttiIMedia) {
        Media* m = ptr.get();

        QListWidgetItem* item = new QListWidgetItem();
        MediaViewWidget* widget = new MediaViewWidget(m);
        item->setSizeHint(widget->sizeHint());
        mediaList->addItem(item);
        mediaList->setItemWidget(item, widget);
        item->setData(Qt::UserRole, QVariant::fromValue(reinterpret_cast<quintptr>(m)));
    }

    QMessageBox::information(this, "Importazione completata", "Tutti i media sono stati importati.");

}

void MediaManagerWidget::deleteAllMedia() {
    int risposta = QMessageBox::question(this, "Conferma eliminazione",
                                            "Sei sicuro di voler eliminare tutti i media?",
                                            QMessageBox::Yes | QMessageBox::No);

    if (risposta == QMessageBox::Yes) {
        MediaRepo::instance().svuotaDB();
        mediaList->clear();  // svuota la lista visuale
        QMessageBox::information(this, "Eliminazione completata", "Tutti i media sono stati eliminati.");
        mediaForm->pulisciCampi();
    }
}

Media* MediaManagerWidget::getMediaFromItem(QListWidgetItem* item) {
    if (!item) return nullptr;
    quintptr ptrVal = item->data(Qt::UserRole).value<quintptr>();
    return reinterpret_cast<Media*>(ptrVal);
}

void MediaManagerWidget::populateFormFromSelected(QListWidgetItem* item) {
    Media* media = getMediaFromItem(item);
    if (media) {
        mediaForm->caricaMedia(media);  // Metodo che devi implementare in MediaFormWidget
    }
}

void MediaManagerWidget::onRicercaAvviata(const QString& testo, const QString& criterio) {
    // Puliamo la lista attuale
    mediaList->clear();

    // Recupera tutti i media filtrati dal repository
    auto risultati = MediaRepo::instance().cercaMedia(testo, criterio);
    for (Media* m : risultati) {
        QListWidgetItem* item = new QListWidgetItem();
        MediaViewWidget* widget = new MediaViewWidget(m);
        item->setSizeHint(widget->sizeHint());
        mediaList->addItem(item);
        mediaList->setItemWidget(item, widget);
        item->setData(Qt::UserRole, QVariant::fromValue(reinterpret_cast<quintptr>(m)));
    }
}

//void MediaManagerWidget::aggiornaRicerca() {
//    // Reinvoca la ricerca con i parametri attuali
//    onRicercaAvviata(searchWidget->getTestoRicerca(), searchWidget->getCriterioRicerca());
//}
