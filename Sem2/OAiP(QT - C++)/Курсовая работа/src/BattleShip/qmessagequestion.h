#ifndef QMESSAGEQUESTION_H
#define QMESSAGEQUESTION_H

#include <QMessageBox>

class QMessageQuestion
{
public:
    static QMessageBox *setQuestion(QString setTitle, QString setText);
};

#endif // QMESSAGEQUESTION_H
