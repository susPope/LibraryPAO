#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QPushButton;
class QStackedWidget;

class MediaManagerWidget;
//class SearchWidget;
class LoanManagerWidget;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void showMediaManager();
    //void showSearchMedia();
    void showLoanManager();

private:
    QPushButton *gestioneBtn;
    //QPushButton *ricercaBtn;
    QPushButton *prestitiBtn;

    QStackedWidget *stackedWidget;
    MediaManagerWidget *mediaWidget;
    //SearchWidget *searchWidget;
    LoanManagerWidget *loanWidget;
};

#endif // MAINWINDOW_H
