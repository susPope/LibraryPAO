#include "searchwidget.h"

SearchWidget::SearchWidget(QWidget* parent) : QWidget(parent) {
    criterioCombo = new QComboBox(this);
    criterioCombo->addItem("Titolo");
    criterioCombo->addItem("Autore o Regista");

    searchEdit = new QLineEdit(this);
    searchEdit->setPlaceholderText("Cerca...");
    searchEdit->setStyleSheet("QLineEdit { color: white; background-color: #333; } QLineEdit::placeholder { color: gray; }");

    searchButton = new QPushButton("🔍 Cerca", this);
    searchButton->setToolTip("Avvia la ricerca");

    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->addWidget(criterioCombo);
    layout->addWidget(searchEdit);
    layout->addWidget(searchButton);
    layout->setContentsMargins(0, 0, 0, 0);
    setLayout(layout);

    connect(searchEdit, &QLineEdit::textEdited, this, &SearchWidget::onTextEdited);
    connect(searchButton, &QPushButton::clicked, this, &SearchWidget::onCercaClicked);
}

QString SearchWidget::getTestoRicerca() const {
    return searchEdit->text().trimmed();
}

QString SearchWidget::getCriterioRicerca() const {
    return criterioCombo->currentText();  // o il nome corretto della QComboBox
}

void SearchWidget::onTextEdited(const QString& testo) {
    emit ricercaAvviata(testo.trimmed(), criterioCombo->currentText());
}

void SearchWidget::onCercaClicked() {
    emit ricercaAvviata(getTestoRicerca(), criterioCombo->currentText());
}
