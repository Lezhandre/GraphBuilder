#ifndef OUTPUTGRAPH_H
#define OUTPUTGRAPH_H

#include <QWidget>
#include <QtGui>
#include "function.h"

namespace Ui {
class outputGraph;
}

class outputGraph : public QWidget
{
    Q_OBJECT

public:
    explicit outputGraph(QWidget *parent = nullptr);
    ~outputGraph();
    void recountPixels();
    void getXY(double, double, double, double);
    void getFun(fun);

protected:
    void paintEvent(QPaintEvent *) override;
    void resizeEvent(QResizeEvent *) override;

private:
    Ui::outputGraph *ui;
    int pictWidth, pictHeight;
    double step;
    double onePixelX, onePixelY;
    double Ox, Oy;
    double leftY, rightY;
    double leftX, rightX;
    fun f;
};

#endif // OUTPUTGRAPH_H
