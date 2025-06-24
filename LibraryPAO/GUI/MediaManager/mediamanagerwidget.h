#ifndef MANAGEMEDIAWIDGET_H
#define MANAGEMEDIAWIDGET_H

#include "mediaformwidget.h"
#include "GUI/SearchBar/searchwidget.h"
#include <QWidget>
#include <QListWidgetItem>

class QListWidget;
class QPushButton;

class MediaManagerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MediaManagerWidget(QWidget *parent = nullptr);
    //void aggiornaRicerca();

private slots:
    void addMedia();
    void editSelectedMedia();
    void deleteSelectedMedia();
    void importDB();
    void deleteAllMedia();
    void populateFormFromSelected(QListWidgetItem* item);
    void onRicercaAvviata(const QString& testo, const QString& criterio);


private:
    QListWidget *mediaList;
    QPushButton *addButton;
    QPushButton *editButton;
    QPushButton *deleteButton;
    QPushButton *importDBButton;
    QPushButton *deleteDBButton;
    MediaFormWidget* mediaForm;
    SearchWidget* searchWidget;

    Media* getMediaFromItem(QListWidgetItem* item);
};

#endif // MANAGEMEDIAWIDGET_H
