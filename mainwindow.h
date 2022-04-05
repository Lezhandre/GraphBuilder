#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
//#include <QVector>
#include "function.h"
#include <QMdiArea>
#include <QMdiSubWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    //void drawGraph(bool notEmpty = false);
    void getData();
    
private slots:
    void on_exit_clicked();
    void on_clear_clicked();
    void on_draw_clicked();
    void on_create_subwindow_clicked();
    void on_delete_subwindow_clicked();

private:
    Ui::MainWindow *ui;
    double leftX, rightX;
    fun f;
    double leftY, rightY;
    bool empty;
    QMap<QMdiSubWindow*, QWidget*> subwnd_to_wdg;
    //QVector <QWidget*> subwindows;
};

#endif // MAINWINDOW_H
