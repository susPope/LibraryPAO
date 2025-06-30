#ifndef MEDIAVIEWWIDGET_H
#define MEDIAVIEWWIDGET_H

#include "Project/media.h"

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>

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
