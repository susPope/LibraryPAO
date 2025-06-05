#ifndef MEDIAVIEWWIDGET_H
#define MEDIAVIEWWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include "Project/media.h"

class MediaViewWidget : public QWidget {
    Q_OBJECT

public:
    explicit MediaViewWidget(Media* media, QWidget* parent = nullptr);
    void aggiorna();

private:
    Media* media;
    QLabel* titoloLabel;
    QLabel* dettagliLabel;
    QLabel* tipoLabel;
};

#endif // MEDIAVIEWWIDGET_H
