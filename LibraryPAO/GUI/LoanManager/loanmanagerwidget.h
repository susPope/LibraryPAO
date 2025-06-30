#ifndef LOANMANAGERWIDGET_H
#define LOANMANAGERWIDGET_H

#include "Project/media.h"
#include "GUI/SearchBar/searchwidget.h"

#include <QListWidgetItem>

class QListWidget;
class QPushButton;

class LoanManagerWidget : public QWidget {
    Q_OBJECT

public:
    explicit LoanManagerWidget(QWidget *parent = nullptr);
    void aggiornaRicerca();

private slots:
    void addLoan();
    void returnLoan();
    void onRicercaAvviata(const QString& testo, const QString& criterio);

private:
    QListWidget *loanList;
    QPushButton *addButton;
    QPushButton *returnButton;
    SearchWidget *searchWidget;
    QComboBox* disponibilitaComboBox;

    Media* getMediaFromItem(QListWidgetItem* item);
};

#endif // LOANMANAGERWIDGET_H
