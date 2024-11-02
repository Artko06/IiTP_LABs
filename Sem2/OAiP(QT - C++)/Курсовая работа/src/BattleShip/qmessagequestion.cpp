#include "qmessagequestion.h"
#include "qabstractbutton.h"

QMessageBox *QMessageQuestion::setQuestion(QString setTitle, QString setText)
{
    auto answer = new QMessageBox();
    answer->setWindowTitle(setTitle);
    answer->setText(setText);
    answer->setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    answer->button(QMessageBox::Yes)->setText(QMessageBox::tr("Да"));
    answer->button(QMessageBox::No)->setText(QMessageBox::tr("Нет"));
    answer->setDefaultButton(QMessageBox::No);
    answer->setWindowFlags(answer->windowFlags() & ~Qt::WindowCloseButtonHint);
    return answer;
}
