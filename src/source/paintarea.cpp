#include "paintarea.h"

PaintArea::PaintArea(QWidget *parent) : QWidget(parent)
{
    setPalette(QPalette(Qt::black));
    setAutoFillBackground(true);
    setMinimumSize(950,800);
    w_begs = new QList<QPoint>;
    w_ends = new QList<QPoint>;
    cw_begs = new QList<QPoint>;
    cw_ends = new QList<QPoint>;
    l_begs = new QList<QPoint>;
    l_ends = new QList<QPoint>;
    cl_begs = new QList<QPoint>;
    cl_ends = new QList<QPoint>;
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
    for (auto i = 0;i<l_begs->size();i++)
        drawLine(p,l_begs->at(i),l_ends->at(i));

    for (auto i = 0;i<cl_begs->size();i++)
        drawCurvedLine(p,cl_begs->at(i),cl_ends->at(i));

    for (auto i = 0;i<w_begs->size();i++)
        drawWave(p,w_begs->at(i),w_ends->at(i));
    for (auto i = 0;i<cw_begs->size();i++)
        drawCurvedWave(p,cw_begs->at(i),cw_ends->at(i));

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

void PaintArea::drawCurvedWave(QPainter &p, const QPoint &beg, const QPoint &end)
{
    QPoint p1 = beg;
    QLineF l(beg,end);
    auto radius = l.length()/2;
    for (double x = 0; x <= distance(beg,end);x+=0.05) {
        double angle = double(x)/((0.5)*(2*3.1415926));
        double y = sign(direction(end,l.angle())-direction(beg,l.angle()))*(10*sin(angle)+sqrt(radius*radius-(x-radius)*(x-radius)));
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

void PaintArea::drawCurvedLine(QPainter & p, const QPoint & beg, const QPoint & end)
{
    QLineF l(beg, end);
    auto radius = l.length()/2.0;
    auto n = l.normalVector();
    auto u = l.unitVector();
    n.setLength(radius);
    QRect rect(l.center().x()-radius,l.center().y()-radius,l.length(),l.length());
    p.drawArc(rect,l.angle()*16,180*16);

    n.translate(QPoint(l.dx()/2,l.dy()/2));
    u.translate(QPoint(n.p2().x()-u.p1().x(),n.p2().y()-u.p1().y()));
    u.setLength(8);

    auto n1 = u.normalVector();
    n1.setLength(n1.length() * 0.5);
    auto n2 = n1.normalVector().normalVector();
    auto p1 = u.p2();
    auto p2 = n1.p2();
    auto p3 = n2.p2();
    QPointF points[3] = {p1, p2, p3};
    p.drawPolygon(points,3);
}

void PaintArea::mousePressEvent(QMouseEvent *e)
{
    beg = e->pos();
    switch(gadget)
    {
        case wave:w_begs->append(beg);statusOfMem->append(Wave);break;
        case curvedWave:cw_begs->append(beg);statusOfMem->append(CurvedWave);break;
        case line:l_begs->append(beg);statusOfMem->append(Line);break;
        case curvedLine:cl_begs->append(beg);statusOfMem->append(CurvedLine);break;
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
            case curvedWave:cw_ends->append(end);break;
            case line:l_ends->append(end);break;
            case curvedLine:cl_ends->append(end);break;
            case dot:dots->replace(dots->size()-1,end);break;
            default:break;
        }
        startDragging = false;
    } else {
        switch(gadget)
        {
            case wave:w_ends->replace(w_ends->size()-1,end);break;
            case curvedWave:cw_ends->replace(cw_ends->size()-1,end);break;
            case line:l_ends->replace(l_ends->size()-1,end);break;
            case curvedLine:cl_ends->replace(cl_ends->size()-1,end);break;
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
        case dot:{
            double radius = 15;
            for(auto p = dots->begin();p!=dots->end();p++) {
                for (auto i = l_begs->begin();i!=l_begs->end();i++)
                    if(distance(*i,*p) <= radius)
                    {
                        i->setX(p->x());
                        i->setY(p->y());
                    }
                for (auto i = l_ends->begin();i!=l_ends->end();i++)
                    if(distance(*i,*p) <= radius)
                    {
                        i->setX(p->x());
                        i->setY(p->y());
                    }
                for (auto i = cl_begs->begin();i!=cl_begs->end();i++)
                    if(distance(*i,*p) <= radius)
                    {
                        i->setX(p->x());
                        i->setY(p->y());
                    }
                for (auto i = cl_ends->begin();i!=cl_ends->end();i++)
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
                for (auto i = cw_begs->begin();i!=cw_begs->end();i++)
                    if(distance(*i,*p) <= radius)
                    {
                        i->setX(p->x());
                        i->setY(p->y());
                    }
                for (auto i = cw_ends->begin();i!=cw_ends->end();i++)
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
    auto tmp_1 = *l_begs;
    tmp_1.append(*cl_begs);
    auto tmp_2 = *l_ends;
    tmp_2.append(*cl_ends);
    auto tmp_3 = *w_begs;
    tmp_3.append(*cw_begs);
    auto tmp_4 = *w_ends;
    tmp_4.append(*cw_ends);
    emit compute(&tmp_1,&tmp_2,&tmp_3,&tmp_4,dots);
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
    cw_begs = new QList<QPoint>;
    cw_ends = new QList<QPoint>;
    l_begs = new QList<QPoint>;
    l_ends = new QList<QPoint>;
    cl_begs = new QList<QPoint>;
    cl_ends = new QList<QPoint>;
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
        if(l_begs->empty())
            return;
        mem->push(l_begs->last());
        l_begs->pop_back();
        mem->push(l_ends->last());
        l_ends->pop_back();
        break;
    case CurvedLine:
        if(cl_begs->empty())
            return;
        mem->push(cl_begs->last());
        cl_begs->pop_back();
        mem->push(cl_ends->last());
        cl_ends->pop_back();
        break;
    case Wave:
        if(w_begs->empty())
            return;
        mem->push(w_begs->last());
        w_begs->pop_back();
        mem->push(w_ends->last());
        w_ends->pop_back();
        break;
    case CurvedWave:
        if(cw_begs->empty())
            return;
        mem->push(cw_begs->last());
        cw_begs->pop_back();
        mem->push(cw_ends->last());
        cw_ends->pop_back();
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
        l_ends->append(mem->pop());
        l_begs->append(mem->pop());
        break;
    case CurvedLine:
        cl_ends->append(mem->pop());
        cl_begs->append(mem->pop());
        break;
    case Wave:
        w_ends->append(mem->pop());
        w_begs->append(mem->pop());
        break;
    case CurvedWave:
        cw_ends->append(mem->pop());
        cw_begs->append(mem->pop());
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

double PaintArea::direction(const QPoint &a, double angle)
{
    return cos(angle)*a.x()+sin(angle)*a.y();
}

int PaintArea::sign(double a)
{
    return int(a > 0) - int(a < 0);
}

