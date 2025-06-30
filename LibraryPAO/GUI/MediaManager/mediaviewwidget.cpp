#include "mediaviewwidget.h"
#include "../themeutils.h"

#include <QFrame>
#include <QPixmap>
#include <QSizePolicy>
#include <QApplication>

MediaViewWidget::MediaViewWidget(Media* media, QWidget* parent) : QWidget(parent), media(media) {
    titoloLabel = new QLabel(media->getTitolo());
    tipoLabel = new QLabel();
    dettagliLabel = new QLabel();

    titoloLabel->setStyleSheet("border: none;");
    tipoLabel->setStyleSheet("border: none;");
    dettagliLabel->setStyleSheet("border: none;");

    QFont fontTitolo = titoloLabel->font();
    fontTitolo.setPointSize(16);
    fontTitolo.setBold(true);
    titoloLabel->setFont(fontTitolo);
    tipoLabel->setText(media->getTipoIcona());
    dettagliLabel->setText(media->getDettagliString());

    // Icona sulla sinistra
    QLabel* iconaLabel = new QLabel();
    QPixmap iconaPixmap(media->getIconaPath(isDarkTheme()));
    iconaLabel->setPixmap(iconaPixmap.scaled(64, 64, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    iconaLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    iconaLabel->setStyleSheet("border: none;");

    // Layout verticale con titolo, dettagli, tipo
    QVBoxLayout* infoLayout = new QVBoxLayout();
    infoLayout->addWidget(titoloLabel);
    infoLayout->addWidget(dettagliLabel);
    infoLayout->addWidget(tipoLabel);
    infoLayout->setSpacing(2);
    infoLayout->setContentsMargins(5, 0, 0, 0);

    // Layout orizzontale: [icona] [info]
    QHBoxLayout* contenutoLayout = new QHBoxLayout();
    contenutoLayout->addWidget(iconaLabel);
    contenutoLayout->addLayout(infoLayout);
    contenutoLayout->setSpacing(10);
    contenutoLayout->setContentsMargins(5, 5, 5, 5);

    // Frame con bordo
    QFrame* frame = new QFrame(this);
    frame->setLayout(contenutoLayout);
    frame->setStyleSheet("border: 1px solid black;");

    // Layout principale
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(frame);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(mainLayout);
}

void MediaViewWidget::aggiorna() {
    titoloLabel->setText(media->getTitolo());
    tipoLabel->setText(media->getTipoIcona());
    dettagliLabel->setText(media->getDettagliString());
}
