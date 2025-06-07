#include "mainwindow.h"
#include "mediamanagerwidget.h"
#include "searchwidget.h"
#include "loanmanagerwidget.h"

#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include <QStackedWidget>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    // Barra menu di navigazione
    gestioneBtn = new QPushButton("Gestione Media");
    prestitiBtn = new QPushButton("Gestione Prestiti");

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

    // Aggiunta dei layout
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
    stackedWidget->setCurrentIndex(0);
}

// Esecuzione del SearchWidget
//void MainWindow::showSearchMedia() {
//    stackedWidget->setCurrentIndex(1);
//}

// Esecuzione del LoanManagerWidget
void MainWindow::showLoanManager() {
    stackedWidget->setCurrentIndex(1);
}
