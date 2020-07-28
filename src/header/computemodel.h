#ifndef COMPUTEMODEL_H
#define COMPUTEMODEL_H

#include <QWidget>
#include <QTextBrowser>
#include <cmath>
#include "particle.h"

class ComputeModel : public QWidget
{
    Q_OBJECT
private:
    QTextBrowser* tb;
    QStringList* indices;

    int findMaximumIn(QList<QPoint>*, double angle);
    int findMinimumIn(QList<QPoint>*, double angle);
    bool contains(QList<QPoint>*,QPoint);
    double direction(QPoint, double angle);
    double distance(QPoint, QPoint);
public:
    double angle = 0;

    explicit ComputeModel(QWidget *parent = nullptr);
signals:
    void on_mark(QList<QList<Particle>>);
public slots:
    void on_compute(QList<QPoint>*, QList<QPoint>*, QList<QPoint>*, QList<QPoint>*, QList<QPoint>*);
    void paintEvent(QPaintEvent *);
    void setAngle(int);
};

#endif // COMPUTEMODEL_H
