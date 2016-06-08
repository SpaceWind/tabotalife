#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "viewerdesc.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    library = new ViewerLibrary("");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_3_clicked()
{

}

void MainWindow::on_pushButton_2_clicked()
{
    viewer = ViewerDesc::generate(library);
    ui->lineEdit_7->setText(QString::number(viewer.age));
    ui->lineEdit_8->setText(QString::number(viewer.isMale));
    ui->lineEdit_9->setText(QString::number(viewer.alphaLevel));
    ui->lineEdit_10->setText(QString::number(viewer.casuality));
}
