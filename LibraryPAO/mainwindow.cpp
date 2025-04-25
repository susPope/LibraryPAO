/*#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
*/

#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QListWidget>
#include <QLabel>
#include <QIcon>
#include <QApplication>
#include <QStyle>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {

    // Configurazione finestra
    setWindowTitle("Library PAO - Gestione Biblioteca");
    setWindowIcon(QIcon(":/icons/library-icon.png")); // Sostituisci con il tuo percorso
    resize(900, 600);

    setupUI();
    setupStyles();
}

void MainWindow::setupUI() {
    // Widget centrale
    QWidget *centralWidget = new QWidget(this);
    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);

    // Menu laterale sinistro (20% larghezza)
    sideMenu = new QListWidget();
    sideMenu->setFixedWidth(180); // Larghezza fissa

    // Aggiungi voci al menu
    QStringList menuItems = {"Dashboard", "Libri", "Film", "Articoli", "Statistiche", "Impostazioni"};
    sideMenu->addItems(menuItems);

    // Area centrale (80% larghezza)
    QWidget *centerWidget = new QWidget();
    QVBoxLayout *centerLayout = new QVBoxLayout(centerWidget);

    // Titolo principale
    mainTitle = new QLabel("LIBRARY PAO");
    mainTitle->setAlignment(Qt::AlignCenter);

    // Aggiungi elementi al layout
    centerLayout->addWidget(mainTitle);
    centerLayout->addStretch(); // Spazio vuoto sotto

    // Configura layout principale
    mainLayout->addWidget(sideMenu);
    mainLayout->addWidget(centerWidget, 1); // Il secondo parametro fa espandere

    setCentralWidget(centralWidget);
}

void MainWindow::setupStyles() {
    // Stile per il titolo principale
    mainTitle->setStyleSheet(R"(
        QLabel {
            font-size: 48px;
            font-weight: bold;
            color: #2c3e50;
            margin-top: 100px;
        }
    )");

    // Stile per il menu laterale
    sideMenu->setStyleSheet(R"(
        QListWidget {
            background-color: #34495e;
            color: white;
            font-size: 16px;
            border: none;
        }
        QListWidget::item {
            padding: 15px;
            border-bottom: 1px solid #2c3e50;
        }
        QListWidget::item:hover {
            background-color: #2c3e50;
        }
        QListWidget::item:selected {
            background-color: #3498db;
        }
    )");

    // Stile per la finestra
    setStyleSheet(R"(
        QMainWindow {
            background-color: #ecf0f1;
        }
    )");
}
