#ifndef PAINTAREA_H
#define PAINTAREA_H

#include <cmath>
#include <QWidget>
#include <QPen>
#include <QMouseEvent>
#include <QList>
#include <QStack>
#include <QPainter>
#include <QLineF>
#include "particle.h"

class PaintArea : public QWidget
{
    Q_OBJECT
public:
    enum Gadget{line, curvedLine, curvedWave, wave, dot};
    enum Status{Line, CurvedLine, CurvedWave, Wave, Dot};
    explicit PaintArea(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent*);
    void mouseMoveEvent(QMouseEvent*);
    void mousePressEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);

    void setGadget(Gadget);
    void clean();

    double direction(const QPoint &,double);
    double distance(const QPoint &, const QPoint &);
signals:
    void compute(QList<QPoint>*, QList<QPoint>*, QList<QPoint>*, QList<QPoint>*, QList<QPoint>*);
public slots:
    void on_compute();
    void mark(QList<QList<Particle>>);
    void undo();
    void redo();
private:
    Gadget gadget;
    QPoint beg;
    QPoint end;
    QList<QPoint>* l_begs;
    QList<QPoint>* l_ends;
    QList<QPoint>* w_begs;
    QList<QPoint>* w_ends;
    QList<QPoint>* dots;
    QList<std::string>* labels;
    QList<QPointF>* l_points;

    QList<QPoint>* cl_begs;
    QList<QPoint>* cl_ends;
    QList<QPoint>* cw_begs;
    QList<QPoint>* cw_ends;

    QList<Status>* statusOfMem;
    int offset = 0;
    QStack<QPoint>* mem;

    bool startDragging;

    void drawWave(QPainter &, const QPoint &, const QPoint &);
    void drawLine(QPainter &, const QPoint &, const QPoint &);
    void drawDot(QPainter &, const QPoint &);

    void drawCurvedLine(QPainter &, const QPoint &, const QPoint &);
    void drawCurvedWave(QPainter &, const QPoint &, const QPoint &);
    int sign(double);
};

#endif // PAINTAREA_H
