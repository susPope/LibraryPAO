#include "mainwindow.h"
#include "MediaManager/mediamanagerwidget.h"
#include "LoanManager/loanmanagerwidget.h"

#include <QVBoxLayout>
#include <QWidget>
#include <QToolBar>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowIcon(QIcon(":/resources/icons/logo.png"));

    titleLabel = new QLabel("📚 Gestionale Biblioteca 📚");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet(R"(
        QLabel {
            font-size: 30px;
            font-weight: bold;
        }
    )");


    // Linea orizzontale
    QFrame* line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    line->setStyleSheet(R"(
        QFrame {
            border: 3px solid #bbb;
            margin-left: 10px;
            margin-right: 10px;
        }
    )");

    gestioneBtn = new QPushButton("Gestione Media");
    prestitiBtn = new QPushButton("Gestione Prestiti");

    gestioneBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    prestitiBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    gestioneBtn->setCheckable(true);
    prestitiBtn->setCheckable(true);
    gestioneBtn->setChecked(true);

    QButtonGroup* navGroup = new QButtonGroup(this);
    navGroup->setExclusive(true);
    navGroup->addButton(gestioneBtn, 0);
    navGroup->addButton(prestitiBtn, 1);

    // Stile solo per pulsanti selezionati
    QString navButtonStyle = R"(
        QPushButton {
            border-radius: 10px;
            padding: 8px 16px;
        }
        QPushButton:checked {
            background-color: palette(highlight);
            color: palette(highlightedText);
            font-weight: bold;
            border: 1px solid palette(dark);
            border-radius: 10px;
        }
    )";

    gestioneBtn->setStyleSheet(navButtonStyle);
    prestitiBtn->setStyleSheet(navButtonStyle);

    // Creazione Widget specifici
    mediaWidget = new MediaManagerWidget();
    loanWidget = new LoanManagerWidget();

    // Creazione QStackedWidget (layer di widget)
    stackedWidget = new QStackedWidget;
    stackedWidget->addWidget(mediaWidget);  // index 0
    stackedWidget->addWidget(loanWidget);   // index 1

    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout;

    // Pulsanti in alto (barra di navigazione)
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(gestioneBtn);
    buttonLayout->addWidget(prestitiBtn);

    // Aggiunta al layout principale
    layout->addWidget(titleLabel);

    layout->addWidget(line);
    layout->addLayout(buttonLayout);
    layout->addWidget(stackedWidget);

    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);

    setWindowTitle("Gestionale Biblioteca");

    // Connessione dei pulsanti per cambiare pagina
    connect(gestioneBtn, &QPushButton::clicked, this, &MainWindow::showMediaManager);
    connect(prestitiBtn, &QPushButton::clicked, this, &MainWindow::showLoanManager);

    // Imposta pagina iniziale
    stackedWidget->setCurrentIndex(0);
}

// Esecuzione del MediaManagerWidget
void MainWindow::showMediaManager() {
    //mediaWidget->aggiornaRicerca();
    stackedWidget->setCurrentIndex(0);
}

// Esecuzione del LoanManagerWidget
void MainWindow::showLoanManager() {
    loanWidget->aggiornaRicerca();
    stackedWidget->setCurrentIndex(1);
}
