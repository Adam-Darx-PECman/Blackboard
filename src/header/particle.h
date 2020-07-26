#ifndef PARTICLE_H
#define PARTICLE_H

#include <string>
#include <QPoint>

class Particle
{
public:
    Particle(std::string,QPoint,QPoint);
    std::string name;
    QPoint beg;
    QPoint end;
};

#endif // PARTICLE_H
