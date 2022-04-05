#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <math.h>
#include <QDebug>
#include "function.h"
#include "outputgraph.h"
#include <QMessageBox>

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);
    leftX = -100; rightX = 100;
    leftY = -100; rightY = 100;
    empty = true;
    //drawGraph();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::getData() {
    //pictHeight = height();
    //pictWidth = width();
    leftX = ui->inputLeftX->text().toDouble();
    rightX = ui->inputRightX->text().toDouble();
    f.set_fun((ui->inputLeftX_4->text()).toStdString());
    leftY = f.min(leftX, rightX);
    rightY = f.max(leftX, rightX);
    //step = (rightX - leftX) / pictWidth;
}

void MainWindow::on_exit_clicked() {
    this->close();
}

void MainWindow::on_clear_clicked() {
    empty = true;
    if (ui->field->activeSubWindow() != nullptr) {
        outputGraph* wdg = static_cast<outputGraph*>(ui->field->activeSubWindow()->widget());
        wdg->getFun(fun());
        wdg->update();
    }
    //drawGraph();
}

void MainWindow::on_draw_clicked() {
    getData();
    if (f.is_err()) {
        QMessageBox::critical(this, tr("Function"),
                                       tr("Function isn't correct."),
                                       QMessageBox::Ok,
                                       QMessageBox::Ok);
        return;
    }
    if (ui->field->activeSubWindow() != nullptr) {
        outputGraph* wdg = static_cast<outputGraph*>(ui->field->activeSubWindow()->widget());
        wdg->getXY(leftX, rightX, leftY, rightY);
        wdg->getFun(f);
        wdg->recountPixels();
        wdg->update();
    }
    //drawGraph(1);
}

void MainWindow::on_create_subwindow_clicked() {
    auto wdg = new outputGraph();
    auto subwnd = ui->field->addSubWindow(wdg);
    if (subwnd_to_wdg.empty()) {
        subwnd->showMaximized();
    } else {
        subwnd->showNormal();
    }
    subwnd_to_wdg[subwnd] = wdg;
    connect(subwnd, &QMdiSubWindow::destroyed, this, &MainWindow::on_delete_subwindow_clicked);
}

void MainWindow::on_delete_subwindow_clicked() {
    auto subwnd = ui->field->activeSubWindow();
    ui->field->closeActiveSubWindow();
    ui->field->removeSubWindow(subwnd_to_wdg[subwnd]);
    delete subwnd_to_wdg[subwnd];
    subwnd_to_wdg.remove(subwnd);
}

