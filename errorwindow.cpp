#include "errorwindow.h"
#include "mainwindow.h"
#include "ui_errorwindow.h"

ErrorWindow::ErrorWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ErrorWindow)
{
    ui->setupUi(this);
}

ErrorWindow::~ErrorWindow()
{
    delete ui;
}

void ErrorWindow::on_pushButton_clicked()
{
    MainWindow mainwindow;
    mainwindow.erase();
    this->close();
}
