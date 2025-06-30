#include "mediamanagerwidget.h"
#include "Project/media.h"
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
#include <QShortcut>
#include <QKeySequence>

MediaManagerWidget::MediaManagerWidget(QWidget *parent) : QWidget(parent) {
    mediaList = new QListWidget(this);

    mediaForm = new MediaFormWidget(this);

    addButton = new QPushButton("Aggiungi", this);
    addButton->setToolTip("Aggiunge un media");
    editButton = new QPushButton("Modifica", this);
    editButton->setToolTip("Modifica il media selezionato con i campi del form");
    deleteButton = new QPushButton("Elimina", this);
    deleteButton->setToolTip("Elimina il media selezionato");
    cleanFieldsButton = new QPushButton("Clean", this);
    cleanFieldsButton->setToolTip("Ripulisce tutti i campi del form");
    importDBButton = new QPushButton("Importa DataBase", this);
    importDBButton->setToolTip("Importa un nuovo JSON");
    deleteDBButton = new QPushButton("Elimina DataBase", this);
    deleteDBButton->setToolTip("Elimina tutti i media dal JSON");

    QGridLayout* actionLayout = new QGridLayout;
    actionLayout->addWidget(addButton, 0, 0);
    actionLayout->addWidget(editButton, 0, 1);
    actionLayout->addWidget(deleteButton, 0, 2);
    actionLayout->addWidget(importDBButton, 1, 0);
    actionLayout->addWidget(deleteDBButton, 1, 1);
    actionLayout->addWidget(cleanFieldsButton, 1, 2);
    actionLayout->setSpacing(10);
    actionLayout->setContentsMargins(10, 10, 10, 10);

    searchWidget = new SearchWidget(this); // Barra di ricerca

    QVBoxLayout *centralRightLayout = new QVBoxLayout;
    centralRightLayout->addWidget(mediaForm);
    centralRightLayout->addLayout(actionLayout);

    QHBoxLayout *centralLayout = new QHBoxLayout;
    centralLayout->addWidget(mediaList);
    centralLayout->addLayout(centralRightLayout);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(searchWidget);
    mainLayout->addLayout(centralLayout);

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
    connect(cleanFieldsButton, SIGNAL(clicked()), this, SLOT(cleanFields()));

    // Shortcut per addMedia: Ctrl + A
    QShortcut* shortcutAdd = new QShortcut(QKeySequence("Ctrl+A"), this);
    connect(shortcutAdd, &QShortcut::activated, this, &MediaManagerWidget::addMedia);

    // Shortcut per editSelectedMedia: Ctrl + E
    QShortcut* shortcutEdit = new QShortcut(QKeySequence("Ctrl+E"), this);
    connect(shortcutEdit, &QShortcut::activated, this, &MediaManagerWidget::editSelectedMedia);

    // Shortcut per deleteSelectedMedia: Canc o Backspace
    QShortcut* shortcutDelete1 = new QShortcut(QKeySequence(Qt::Key_Delete), this);
    connect(shortcutDelete1, &QShortcut::activated, this, &MediaManagerWidget::deleteSelectedMedia);
    QShortcut* shortcutDelete2 = new QShortcut(QKeySequence(Qt::Key_Backspace), this);
    connect(shortcutDelete2, &QShortcut::activated, this, &MediaManagerWidget::deleteSelectedMedia);

    // Shortcut per importDB: Ctrl + I
    QShortcut* shortcutImport = new QShortcut(QKeySequence("Ctrl+I"), this);
    connect(shortcutImport, &QShortcut::activated, this, &MediaManagerWidget::importDB);

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
    QString errore = MediaRepo::instance().aggiungiMedia(std::move(nuovo));
    if (!errore.isEmpty()) {
        QMessageBox::critical(this, "Errore durante l'aggiunta", errore);
        return;
    }

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

    // Controllo del tipo con typeid invece di getTipo()
    const std::type_info& tipoAttuale = typeid(*media);
    const std::type_info& tipoSelezionato = mediaForm->getTipoSelezionatoTypeInfo();
    if (tipoAttuale != tipoSelezionato) {
        QMessageBox::warning(this, "Modifica non valida", "Non puoi cambiare il tipo del media.");
        return;
    }
    QString errore = mediaForm->aggiornaMedia(media);
    if (errore.isEmpty()) {
        QWidget* w = mediaList->itemWidget(item);  // Ottiene il widget associato all'elemento selezionato
        if (auto* view = dynamic_cast<MediaViewWidget*>(w)) {
            view->aggiorna();  // Chiama un metodo che rilegge i dati da 'media' e li mostra
        }
        QString errore = MediaRepo::instance().aggiornaMedia(media);
        if (!errore.isEmpty()) {
            QMessageBox::critical(this, "Errore durante l'aggiornamento", errore);
            return;
        }
        mediaForm->pulisciCampi();
    } else {
        QMessageBox::warning(this, "Errore", errore);
        return;
    }
}

void MediaManagerWidget::deleteSelectedMedia() {
    int row = mediaList->currentRow();
    QListWidgetItem *item = mediaList->takeItem(row);
    if (item) {
        Media* media = getMediaFromItem(item);
        if (media) {
            QString errore = MediaRepo::instance().rimuoviMedia(media);
            if (!errore.isEmpty()) {
                QMessageBox::critical(this, "Errore durante l'eliminazione", errore);
                return;
            }
        }
        QWidget* widget = mediaList->itemWidget(item);
        if (widget) {
            mediaList->removeItemWidget(item);
            widget->deleteLater();  // Evita memory leak
        }
        delete item;
        mediaForm->pulisciCampi();
    } else {
        QMessageBox::warning(this, "Elimina", "Seleziona un media da eliminare.");
        return;
    }
}

void MediaManagerWidget::importDB() {
    // Codice per aprire la comunicazione con il so e richiedere l'importazione di un json
    QString fileName = QFileDialog::getOpenFileName(this, "Importa Database", "", "File JSON (*.json)");
    if (fileName.isEmpty())
        return;

    // Imposta il nuovo path nel MediaRepo
    MediaRepo::instance().setPath(fileName);
    mediaForm->pulisciCampi();
    mediaList->clear();  // svuota la lista visuale

    QString errore = MediaRepo::instance().importaDB();
    if (!errore.isEmpty()) {
        QMessageBox::critical(this, "Errore durante l'importazione", errore);
        return;
    }

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
        QString errore = MediaRepo::instance().svuotaDB();
        if (!errore.isEmpty()) {
            QMessageBox::critical(this, "Errore durante la cancellazione", errore);
            return;
        }
        mediaList->clear();  // svuota la lista visuale
        QMessageBox::information(this, "Eliminazione completata", "Tutti i media sono stati eliminati.");
        mediaForm->pulisciCampi();
    }
}

void MediaManagerWidget::cleanFields() {
    mediaForm->pulisciCampi();
}

Media* MediaManagerWidget::getMediaFromItem(QListWidgetItem* item) {
    if (!item) return nullptr;
    quintptr ptrVal = item->data(Qt::UserRole).value<quintptr>();
    return reinterpret_cast<Media*>(ptrVal);
}

void MediaManagerWidget::populateFormFromSelected(QListWidgetItem* item) {
    Media* media = getMediaFromItem(item);
    if (media) {
        mediaForm->caricaMedia(media);
    }
}

void MediaManagerWidget::onRicercaAvviata(const QString& testo, const QString& criterio) {
    // Pulisce la lista attuale
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
