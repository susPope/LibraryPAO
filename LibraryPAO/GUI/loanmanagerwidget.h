#ifndef LOANMANAGERWIDGET_H
#define LOANMANAGERWIDGET_H

#include <QWidget>

class QListWidget;
class QPushButton;

class LoanManagerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LoanManagerWidget(QWidget *parent = nullptr);

private slots:
    void addLoan();
    void returnLoan();

private:
    QListWidget *loanList;
    QPushButton *addButton;
    QPushButton *returnButton;
};

#endif // LOANMANAGERWIDGET_H
