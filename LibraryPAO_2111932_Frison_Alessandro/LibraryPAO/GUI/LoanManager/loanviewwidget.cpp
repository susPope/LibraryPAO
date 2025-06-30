#include "loanviewwidget.h"
#include "../themeutils.h"

#include <QPixmap>
#include <QSizePolicy>
#include <QVBoxLayout>
#include <QApplication>

LoanViewWidget::LoanViewWidget(Media* media, QWidget* parent) : QWidget(parent), media(media) {
    // Etichette
    titoloLabel = new QLabel(media->getTitolo());
    tipoLabel = new QLabel();
    dettagliLabel = new QLabel();
    prestitoLabel = new QLabel();

    titoloLabel->setStyleSheet("border: none;");
    tipoLabel->setStyleSheet("border: none;");
    dettagliLabel->setStyleSheet("border: none;");
    prestitoLabel->setStyleSheet("border: none;");

    QFont fontTitolo = titoloLabel->font();
    fontTitolo.setPointSize(16);
    fontTitolo.setBold(true);
    titoloLabel->setFont(fontTitolo);

    tipoLabel->setText(media->getTipoIcona());
    dettagliLabel->setText(media->getDettagliString());

    // Imposta il colore del titolo in base allo stato
    QString coloreTitolo;
    if (media->isInRitardo()) {
        coloreTitolo = "red";
    } else if (!media->getDisponibilita()) {
        coloreTitolo = "orange";
    } else if (media->getDisponibilita()) {
        coloreTitolo = "green";
    } else {
        // default
    }

    titoloLabel->setStyleSheet(QString("color: %1; border: none;").arg(coloreTitolo));

    QString stato = media->getDisponibilita()
                        ? "✅ Disponibile"
                        : QString("❌ Non disponibile • Prossima: %1").arg(media->getProssimaDisponibilita().toString("dd/MM/yyyy"));
    prestitoLabel->setText(QString("%1 • Prestiti: %2").arg(stato).arg(media->getNprestiti()));

    QLabel* iconaLabel = new QLabel(this);
    QPixmap iconaPixmap(media->getIconaPath(isDarkTheme()));
    iconaLabel->setPixmap(iconaPixmap.scaled(64, 64, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    iconaLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    iconaLabel->setStyleSheet("border: none;");

    QVBoxLayout* infoLayout = new QVBoxLayout();
    infoLayout->addWidget(titoloLabel);
    infoLayout->addWidget(dettagliLabel);
    infoLayout->addWidget(tipoLabel);
    infoLayout->addWidget(prestitoLabel);
    infoLayout->setSpacing(2);
    infoLayout->setContentsMargins(5, 0, 0, 0);

    QHBoxLayout* contenutoLayout = new QHBoxLayout();
    contenutoLayout->addWidget(iconaLabel);
    contenutoLayout->addLayout(infoLayout);
    contenutoLayout->setSpacing(10);
    contenutoLayout->setContentsMargins(5, 5, 5, 5);

    QFrame* frame = new QFrame(this);
    frame->setLayout(contenutoLayout);
    frame->setStyleSheet("border: 1px solid black;");

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(frame);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(mainLayout);
}

void LoanViewWidget::aggiorna() {
    titoloLabel->setText(media->getTitolo());
    tipoLabel->setText(media->getTipoIcona());
    dettagliLabel->setText(media->getDettagliString());

    // Imposta il colore del titolo in base allo stato
    QString coloreTitolo;
    if (media->isInRitardo()) {
        coloreTitolo = "red";
    } else if (!media->getDisponibilita()) {
        coloreTitolo = "orange";
    } else if (media->getDisponibilita()) {
        coloreTitolo = "green";
    } else {
        // default
    }

    titoloLabel->setStyleSheet(QString("color: %1; border: none;").arg(coloreTitolo));

    QString stato = media->getDisponibilita()
                        ? "✅ Disponibile"
                        : QString("❌ Non disponibile • Prossima: %1").arg(media->getProssimaDisponibilita().toString("dd/MM/yyyy"));
    prestitoLabel->setText(QString("%1 • Prestiti: %2").arg(stato).arg(media->getNprestiti()));
}
