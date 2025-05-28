#ifndef SEARCHWIDGET_H
#define SEARCHWIDGET_H

#include <QWidget>

class QLineEdit;
class QPushButton;
class QListWidget;

class SearchWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SearchWidget(QWidget *parent = nullptr);

private slots:
    void performSearch();

private:
    QLineEdit *searchInput;
    QPushButton *searchButton;
    QListWidget *resultsList;
};

#endif // SEARCHWIDGET_H
