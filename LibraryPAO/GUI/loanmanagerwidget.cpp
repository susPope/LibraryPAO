#include "loanmanagerwidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QListWidget>
#include <QPushButton>
#include <QMessageBox>

LoanManagerWidget::LoanManagerWidget(QWidget *parent)
    : QWidget(parent)
{
    loanList = new QListWidget(this);
    addButton = new QPushButton("Aggiungi Prestito", this);
    returnButton = new QPushButton("Restituisci", this);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(returnButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(loanList);
    mainLayout->addLayout(buttonLayout);

    setLayout(mainLayout);
    setWindowTitle("Gestione Prestiti");

    // Dati di esempio
    loanList->addItem("Libro: Il Signore degli Anelli - Mario Rossi");
    loanList->addItem("Film: Inception - Anna Bianchi");

    connect(addButton, SIGNAL(clicked()), this, SLOT(addLoan()));
    connect(returnButton, SIGNAL(clicked()), this, SLOT(returnLoan()));
}

void LoanManagerWidget::addLoan()
{
    // In una versione reale si aprirebbe una dialog per inserire dati
    QString nuovoPrestito = "Prestito: Nuovo media - Nuovo utente";
    loanList->addItem(nuovoPrestito);
}

void LoanManagerWidget::returnLoan()
{
    QListWidgetItem *item = loanList->takeItem(loanList->currentRow());
    if (item) {
        delete item;
    } else {
        QMessageBox::warning(this, "Restituzione", "Seleziona un prestito da restituire.");
    }
}
