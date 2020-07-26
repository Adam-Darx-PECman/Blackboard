#ifndef PAINTAREA_H
#define PAINTAREA_H

#include <cmath>
#include <QWidget>
#include <QPen>
#include <QMouseEvent>
#include <QList>
#include <QMap>
#include "particle.h"

class PaintArea : public QWidget
{
    Q_OBJECT
public:
    enum Gadget{line, wave, dot};
    explicit PaintArea(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent*);
    void mouseMoveEvent(QMouseEvent*);
    void mousePressEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);
    void setGadget(Gadget);
    void clean();

    double distance(const QPoint &, const QPoint &);
signals:
    void compute(QList<QPoint>*, QList<QPoint>*, QList<QPoint>*, QList<QPoint>*, QList<QPoint>*);
public slots:
    void on_compute();
    void mark(QList<QList<Particle>>);
private:
    Gadget gadget;
    QPoint beg;
    QPoint end;
    QList<QPoint>* s_begs;
    QList<QPoint>* s_ends;
    QList<QPoint>* w_begs;
    QList<QPoint>* w_ends;
    QList<QPoint>* dots;
    QList<std::string>* labels;
    QList<QPointF>* l_points;
    bool flag;

    void drawWave(QPainter &, const QPoint &, const QPoint &);
    void drawLine(QPainter &, const QPoint &, const QPoint &);
    void drawDot(QPainter &, const QPoint &);
};

#endif // PAINTAREA_H
