#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>

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
    QLabel* titleLabel;
    QPushButton *gestioneBtn;
    QPushButton *prestitiBtn;

    QStackedWidget *stackedWidget;
    MediaManagerWidget *mediaWidget;
    LoanManagerWidget *loanWidget;
};

#endif // MAINWINDOW_H
