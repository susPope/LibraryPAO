#ifndef LOANMANAGERWIDGET_H
#define LOANMANAGERWIDGET_H

#include <QWidget>
#include <QListWidgetItem>
#include "Project/media.h"
#include "searchwidget.h"

class QListWidget;
class QPushButton;

class LoanManagerWidget : public QWidget {
    Q_OBJECT

public:
    explicit LoanManagerWidget(QWidget *parent = nullptr);

private slots:
    void addLoan();
    void returnLoan();
    void onRicercaAvviata(const QString& testo, const QString& criterio);
    void aggiornaRicerca();

private:
    QListWidget *loanList;
    QPushButton *addButton;
    QPushButton *returnButton;
    SearchWidget *searchWidget;
    QComboBox* disponibilitaComboBox;

    Media* getMediaFromItem(QListWidgetItem* item);
};

#endif // LOANMANAGERWIDGET_H
