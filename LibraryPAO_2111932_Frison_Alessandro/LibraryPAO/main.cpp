#include "GUI/mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow w;
    w.setMinimumSize(800, 600);
    w.setMaximumSize(1920, 1080);
    w.resize(1080, 720);
    w.show();
    return app.exec();
}
