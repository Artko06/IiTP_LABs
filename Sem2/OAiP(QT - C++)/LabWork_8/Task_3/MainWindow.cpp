#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , map()
{
    ui->setupUi(this);
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (ui->spinInsert->hasFocus()
        && (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)) {
        if (ui->lineValue->text() != "") {
            ui->lineInsert->setText(
                (map.insert(std::make_pair(ui->spinInsert->value(), ui->lineValue->text())).second
                 == true)
                    ? "True"
                    : "False");
        }
    } else if (ui->spinErace->hasFocus()
               && (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)) {
        ui->lineErace->setText(((map.erace(ui->spinErace->value()) == true) ? "True" : "False"));
    }

    else if (ui->spinContains->hasFocus()
             && (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)) {
        ui->lineContains->setText((map.contains(ui->spinContains->value()) ? "True" : "False"));
    }

    else if (ui->spinOperator->hasFocus()
             && (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)) {
        QString temp = map[ui->spinOperator->value()];
        ui->lineOperator->setText((temp == "") ? "empty" : temp);
    }

    ui->spinSize->setValue(map.size());
    ui->lineOutput->clear();
    for (const auto &i : map) {
        temp = i;
        ui->lineOutput->setText(ui->lineOutput->text() + QString::number(temp.first) + ' ');
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_clearButton_clicked()
{
    map.clear();
    ui->spinSize->setValue(map.size());
    ui->lineOutput->clear();
}
