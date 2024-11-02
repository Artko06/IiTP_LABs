#ifndef LETTERSONWINDOW_H
#define LETTERSONWINDOW_H

#include <QMainWindow>
#include <QPointF>

class LettersOnWindow
{
public:
    static void setLetters(QMainWindow *window, QPointF posScene);
};

#endif // LETTERSONWINDOW_H
