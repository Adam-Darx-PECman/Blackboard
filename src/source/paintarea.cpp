#include "paintarea.h"

PaintArea::PaintArea(QWidget *parent) : QWidget(parent)
{
    setPalette(QPalette(Qt::black));
    setAutoFillBackground(true);
    setMinimumSize(950,800);
    w_begs = new QList<QPoint>;
    w_ends = new QList<QPoint>;
    s_begs = new QList<QPoint>;
    s_ends = new QList<QPoint>;
    dots = new QList<QPoint>;
    labels = new QList<std::string>;
    l_points = new QList<QPointF>;

    statusOfMem = new QList<Status>;
    mem = new QStack<QPoint>;
}

void PaintArea::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    QPen pen;
    pen.setWidth(3);
    pen.setColor(Qt::white);
    auto font = QFont("Consolas",13,75);
    font.setItalic(true);
    p.setFont(font);
    p.setPen(pen);
    for (auto i = 0;i<s_begs->size();i++)
        drawLine(p,s_begs->at(i),s_ends->at(i));
    for (auto i = 0;i<w_begs->size();i++)
        drawWave(p,w_begs->at(i),w_ends->at(i));
    for (auto i = dots->begin();i!=dots->end();i++)
        drawDot(p,*i);
    for (auto i = 0;i < labels->size();i++)
        p.drawText(l_points->at(i),QString().fromStdString(labels->at(i)));

}

void PaintArea::drawLine(QPainter &p, const QPoint &beg, const QPoint &end)
{
    QLineF l(beg,end);
    auto v = l.unitVector();
    v.setLength(8);
    v.translate(QPointF(l.dx()/2,l.dy()/2));

    auto n = v.normalVector();
    n.setLength(n.length() * 0.5);
    auto n2 = n.normalVector().normalVector();
    auto p1 = v.p2();
    auto p2 = n.p2();
    auto p3 = n2.p2();
    QPointF points[3] = {p1, p2, p3};
    p.drawPolygon(points,3);
    p.drawLine(l);
}

void PaintArea::drawWave(QPainter &p, const QPoint &beg, const QPoint &end)
{
    QPoint p1 = beg;
    for (double x = 0; x <= distance(beg,end);x+=0.05) {
        double angle = double(x)/((0.5)*(2*3.1415926));
        double y = 10*sin(angle);
        if(( (end.y()-beg.y())>0 && (end.x()-beg.x())>0 ) || ( (end.y()-beg.y())<0 && (end.x()-beg.x())>0 )){
            double rad = (end.y()-beg.y())/(end.x()-beg.x());
            double theta = atan(rad);
            QPoint p2(cos(theta)*x-sin(theta)*y+beg.x(),sin(theta)*x+cos(theta)*y+beg.y());
            p.drawLine(p1,p2);
            p1=p2;
        } else if(( (end.y()-beg.y())<0 && (end.x()-beg.x())<0 ) || ( (end.y()-beg.y())>0 && (end.x()-beg.x())<0 )){
            double rad = (end.y()-beg.y())/(end.x()-beg.x());
            double theta = atan(rad);
            QPoint p2(-(cos(theta)*x-sin(theta)*y)+beg.x(),-(sin(theta)*x+cos(theta)*y)+beg.y());
            p.drawLine(p1,p2);
            p1=p2;
        } else {
            QPoint p2(-y+beg.x(),x+beg.y());
            p.drawLine(p1,p2);
            p1=p2;
        }
    }
}

void PaintArea::drawDot(QPainter &p, const QPoint &pos)
{
    p.drawEllipse(QRect(pos,QSize(4,4)));
}


void PaintArea::mousePressEvent(QMouseEvent *e)
{
    beg = e->pos();
    switch(gadget)
    {
        case wave:w_begs->append(beg);statusOfMem->append(Wave);break;
        case line:s_begs->append(beg);statusOfMem->append(Line);break;
        case dot:dots->append(beg);statusOfMem->append(Dot);update();break;
        default:break;
    }
    startDragging = true;
}

void PaintArea::mouseMoveEvent(QMouseEvent *e)
{
    end = e->pos();
    if(startDragging)
    {
        switch(gadget)
        {
            case wave:w_ends->append(end);break;
            case line:s_ends->append(end);break;
            case dot:dots->replace(dots->size()-1,end);break;
            default:break;
        }
        startDragging = false;
    } else {
        switch(gadget)
        {
            case wave:w_ends->replace(w_ends->size()-1,end);break;
            case line:s_ends->replace(s_ends->size()-1,end);break;
            case dot:dots->replace(dots->size()-1,end);break;
            default:break;
        }
    }
    update();
}

void PaintArea::mouseReleaseEvent(QMouseEvent*)
{
    //TODO: 此处添加代码
    switch (gadget) {
        case wave:break;
        case line:break;
        case dot:{
            double radius = 15;
            for(auto p = dots->begin();p!=dots->end();p++) {
                for (auto i = s_begs->begin();i!=s_begs->end();i++)
                    if(distance(*i,*p) <= radius)
                    {
                        i->setX(p->x());
                        i->setY(p->y());
                    }
                for (auto i = s_ends->begin();i!=s_ends->end();i++)
                    if(distance(*i,*p) <= radius)
                    {
                        i->setX(p->x());
                        i->setY(p->y());
                    }
                for (auto i = w_begs->begin();i!=w_begs->end();i++)
                    if(distance(*i,*p) <= radius)
                    {
                        i->setX(p->x());
                        i->setY(p->y());
                    }
                for (auto i = w_ends->begin();i!=w_ends->end();i++)
                    if(distance(*i,*p) <= radius)
                    {
                        i->setX(p->x());
                        i->setY(p->y());
                    }
            }
        }break;
        default:break;
    }
    update();
}

void PaintArea::on_compute(){
    emit compute(s_begs,s_ends,w_begs,w_ends,dots);
}

void PaintArea::mark(QList<QList<Particle>> l)
{
    l_points->clear();
    labels->clear();
    for(auto particles : l)
        for(auto particle : particles)
        {
            auto line = QLineF(particle.beg,particle.end);
            auto v = line.unitVector().normalVector();
            v.setLength(12);
            v.translate(line.dx()/2,line.dy()/2);
            l_points->append(v.p2());
            labels->append(particle.name);
        }
    update();
}

void PaintArea::setGadget(Gadget g)
{
    gadget = g;
}

void PaintArea::clean()
{
    w_begs = new QList<QPoint>;
    w_ends = new QList<QPoint>;
    s_begs = new QList<QPoint>;
    s_ends = new QList<QPoint>;
    dots = new QList<QPoint>;
    labels = new QList<std::string>;
    l_points = new QList<QPointF>;
    update();
}

void PaintArea::undo()
{
    switch(statusOfMem->at(statusOfMem->size()-(offset++)-1))
    {
    case Line:
        if(s_begs->empty())
            return;
        mem->push(s_begs->last());
        s_begs->pop_back();
        mem->push(s_ends->last());
        s_ends->pop_back();
        break;
    case Wave:
        if(w_begs->empty())
            return;
        mem->push(w_begs->last());
        w_begs->pop_back();
        mem->push(w_ends->last());
        w_ends->pop_back();
        break;
    case Dot:
        if(dots->empty())
            return;
        mem->push(dots->last());
        dots->pop_back();
        break;
    }
    update();
}

void PaintArea::redo()
{
    if(mem->empty())
        return;
    switch(statusOfMem->at(statusOfMem->size()-(--offset)-1))
    {
    case Line:
        s_ends->append(mem->pop());
        s_begs->append(mem->pop());
        break;
    case Wave:
        w_ends->append(mem->pop());
        w_begs->append(mem->pop());
        break;
    case Dot:
        dots->append(mem->pop());
        break;
    }
    update();
}

double PaintArea::distance(const QPoint& a, const QPoint& b)
{
    return sqrt((a.x()-b.x())*(a.x()-b.x())+(a.y()-b.y())*(a.y()-b.y()));
}

