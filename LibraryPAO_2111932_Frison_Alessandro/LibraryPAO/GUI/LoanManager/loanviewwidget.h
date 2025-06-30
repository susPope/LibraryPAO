#ifndef LOANVIEWWIDGET_H
#define LOANVIEWWIDGET_H

#include "Project/media.h"

#include <QLabel>

class LoanViewWidget : public QWidget {
    Q_OBJECT

public:
    explicit LoanViewWidget(Media* media, QWidget* parent = nullptr);
    void aggiorna();

private:
    Media* media;
    QLabel* titoloLabel;
    QLabel* tipoLabel;
    QLabel* dettagliLabel;
    QLabel* prestitoLabel;
};

#endif // LOANVIEWWIDGET_H
