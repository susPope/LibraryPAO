#ifndef SEARCHWIDGET_H
#define SEARCHWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QComboBox>

class SearchWidget : public QWidget {
    Q_OBJECT

public:
    explicit SearchWidget(QWidget* parent = nullptr);

    QString getTestoRicerca() const;
    QString getCriterioRicerca() const;

signals:
    void ricercaAvviata(const QString& testo, const QString& criterio);


private slots:
    void onTextEdited(const QString& testo);
    void onCercaClicked();

private:
    QLineEdit* searchEdit;
    QPushButton* searchButton;
    QComboBox* criterioCombo;
};

#endif // SEARCHWIDGET_H
