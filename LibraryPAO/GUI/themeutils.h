#ifndef THEMEUTILS_H
#define THEMEUTILS_H

#include <QApplication>
#include <QPalette>

inline bool isDarkTheme() {
    QColor bgColor = qApp->palette().color(QPalette::Window);
    int brightness = (bgColor.red() * 299 + bgColor.green() * 587 + bgColor.blue() * 114) / 1000;
    return brightness < 128;
}

#endif // THEMEUTILS_H
