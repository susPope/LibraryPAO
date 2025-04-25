#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Imposta stile globale
    app.setStyle("Fusion");

    // Crea e mostra finestra principale
    MainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}
