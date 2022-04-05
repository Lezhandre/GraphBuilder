#include "outputgraph.h"
#include "ui_outputgraph.h"

outputGraph::outputGraph(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::outputGraph) {
    ui->setupUi(this);
    leftX = -100; rightX = 100;
    leftY = -100; rightY = 100;
    //setWindowFlags(Qt::Tool);
}

outputGraph::~outputGraph() {
    delete ui;
}

void outputGraph::recountPixels() {
    pictHeight = height();
    pictWidth = width();
    step = (rightX - leftX) / pictWidth;
    onePixelX = pictWidth / (rightX - leftX);
    onePixelY = pictHeight / (rightY - leftY);
    Ox = leftX; Oy = rightY;
}

void outputGraph::getXY(double leftX, double rightX, double leftY, double rightY) {
    this->leftX = leftX;
    this->rightX = rightX;
    this->leftY = leftY;
    this->rightY = rightY;
}

void outputGraph::getFun(fun f) {
    this->f = f;
}

void outputGraph::paintEvent(QPaintEvent *)
{
    //QPixmap graph(pictWidth, pictHeight);
    QPainter paint;
    paint.begin(this);
    paint.eraseRect(0, 0, pictWidth, pictHeight);
    if (-Ox >= 0)
        paint.drawLine((-Ox) * onePixelX, 0, (-Ox) * onePixelX, pictHeight);
    if (Oy >= 0)
        paint.drawLine(0, Oy * onePixelY, pictWidth, Oy * onePixelY);

    paint.setPen(QPen(Qt::black, 2));
    for (double i = 0; i <= rightX && Oy >= 0; i += 2.0)
        paint.drawPoint((i - Ox) * onePixelX, Oy * onePixelY);
    for (double i = 0; i >= leftX && Oy >= 0; i -= 2.0)
        paint.drawPoint((i - Ox) * onePixelX, Oy * onePixelY);
    for (double i = 0; i <= rightY && -Ox >= 0; i += 2.0)
        paint.drawPoint((-Ox) * onePixelX, (-i + Oy) * onePixelY);
    for (double i = 0; i >= leftY && -Ox >= 0; i -= 2.0)
        paint.drawPoint((-Ox) * onePixelX, (-i + Oy) * onePixelY);

    if (f.is_err()) {
        paint.end();
        return;
    }

    paint.setPen(QPen(Qt::green, 1, Qt::SolidLine));
    paint.setRenderHint(QPainter::Antialiasing, true);
    QPainterPath path;
    double i = leftX + step;
    path.moveTo((i - Ox) * onePixelX, (-f.fun_val(i) + Oy) * onePixelY);
    for (; i <= rightX; i += step) {
        path.lineTo((i - Ox) * onePixelX, (-f.fun_val(i) + Oy) * onePixelY);
    }
    paint.setPen(QPen(Qt::blue, 1, Qt::SolidLine));
    paint.drawPath(path);
    paint.end();
    //ui->outputGraph->setPixmap(graph);
    return;
}

void outputGraph::resizeEvent(QResizeEvent *) {
    recountPixels();
    update();
}
