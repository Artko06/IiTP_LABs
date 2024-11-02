#include "lettersonwindow.h"

#include <QChar>
#include <QLabel>
#include "ConstField.h"
#include "ConstShips.h"

constexpr short firstRussianLetter = 1040;
constexpr short indentForTheLetter = 5;
constexpr short indentForTheNumber = 12;
constexpr short NUMBER_IS_10 = 9;

void LettersOnWindow::setLetters(QMainWindow *window, QPointF posScene)
{
    for (int i = 0; i < sizeBattleField / SIZE_ONE_SELL; ++i) {
        QLabel *letterLabel = new QLabel(QString(QChar(firstRussianLetter + i)), window);
        QLabel *numberLabel = new QLabel(QString::number(i + 1), window);

        letterLabel->move(i * SIZE_ONE_SELL + posScene.x() + indentForTheLetter,
                          posScene.y() - SIZE_ONE_SELL);
        (i == NUMBER_IS_10)
            ? numberLabel->move(posScene.x() - SIZE_ONE_SELL, i * SIZE_ONE_SELL + posScene.y())
            : numberLabel->move(posScene.x() - SIZE_ONE_SELL + indentForTheNumber,
                                i * SIZE_ONE_SELL + posScene.y());

        letterLabel->setStyleSheet("QLabel { color : white; font-size : 44px;}");
        numberLabel->setStyleSheet("QLabel { color : white; font-size : 44px;}");
        letterLabel->setFixedSize(SIZE_ONE_SELL, SIZE_ONE_SELL);
        numberLabel->setFixedSize(SIZE_ONE_SELL, SIZE_ONE_SELL);

        letterLabel->show();
        numberLabel->show();
    }
}
