#ifndef MANAGEMEDIAWIDGET_H
#define MANAGEMEDIAWIDGET_H

#include <QWidget>

class QListWidget;
class QPushButton;

class MediaManagerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MediaManagerWidget(QWidget *parent = nullptr);

private slots:
    void addMedia();
    void editSelectedMedia();
    void deleteSelectedMedia();

private:
    QListWidget *mediaList;
    QPushButton *addButton;
    QPushButton *editButton;
    QPushButton *deleteButton;
};

#endif // MANAGEMEDIAWIDGET_H
