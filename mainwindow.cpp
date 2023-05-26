#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QColorDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_guzik_clicked()
{

    QColor kolorold=ui->widget->getColor();
    QColor kolor=QColorDialog::getColor(kolorold,this);
    QString styl;
    if(kolor.isValid())
    {
        styl=QString("background-color: %1;border:2px solid white").arg(kolor.name());
        ui->guzik->setStyleSheet(styl);
        ui->widget->setColor(kolor);
        ui->widget->colorChange();
    }
}

