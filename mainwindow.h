#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "openglwidget.h"
#include <QLabel>
#include <QColorDialog>

namespace Ui {
class Form;
}

class MainWindow : public QWidget
{
    Q_OBJECT

private:
    Ui::Form *ui;
    QColorDialog *color;
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void on_yMax_editingFinished();
    void on_yMin_editingFinished();
    void on_xMin_editingFinished();
    void on_xMax_editingFinished();
    void on_pushButton_clicked();
    void on_checkBox_stateChanged(int arg1);
};
#endif // MAINWINDOW_H
