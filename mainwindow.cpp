#include "mainwindow.h"
#include <QObject>
#include "ui_form.h"
#include <QString>

MainWindow::MainWindow(QWidget *parent) : QWidget(parent), ui(new Ui::Form)
{
    ui->setupUi(this);
    color = new QColorDialog;
    connect(color,SIGNAL(currentColorChanged(const QColor &)),ui->openglWidget,SLOT(setColor(const QColor &)));
    connect(ui->openGLWidget,SIGNAL(sendX(float)),ui->openglWidget,SLOT(getX(float)));
    connect(ui->openGLWidget,SIGNAL(sendPoint(QVector2D)),ui->openglWidget,SLOT(getPoint(QVector2D)));
}

MainWindow::~MainWindow()
{

}


void MainWindow::on_yMax_editingFinished()
{
    ui->openglWidget->setYMax(ui->yMax->text().toFloat());
}

void MainWindow::on_yMin_editingFinished()
{
    ui->openglWidget->setYMin(ui->yMin->text().toFloat());
}

void MainWindow::on_xMin_editingFinished()
{
    ui->openglWidget->setXMin(ui->xMin->text().toFloat());
}

void MainWindow::on_xMax_editingFinished()
{
    ui->openglWidget->setXMax(ui->xMax->text().toFloat());
}

void MainWindow::on_pushButton_clicked()
{
    color->open();
}

void MainWindow::on_checkBox_stateChanged(int arg1)
{
    ui->openglWidget->myStateChagne(arg1);
}
