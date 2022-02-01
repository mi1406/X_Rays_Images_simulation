#include "particle.h"
#include <cmath>
cParticle::cParticle(){}

cParticle::cParticle(const cParticle &newParticle)
{
    for(unsigned short i = 0; i < DIMENSIONS; i++){
        this->pos[i] = newParticle.pos[i];
        this->dir[i] = newParticle.dir[i];
    }
}

cParticle::~cParticle(){}

void cParticle::setPosition(double x, double y, double z)
{
    this->pos[0] = x;
    this->pos[1] = y;
    this->pos[2] = z;
}

void cParticle::setDirection(double x, double y, double z)
{
    this->dir[0] = x;
    this->dir[1] = y;
    this->dir[2] = z;
}

void cParticle::setPosition(double *newPos)
{
    this->pos[0] = newPos[0];
    this->pos[1] = newPos[1];
    this->pos[2] = newPos[2];
}

void cParticle::setDirection(double *newDir)
{
    this->dir[0] = newDir[0];
    this->dir[1] = newDir[1];
    this->dir[2] = newDir[2];
}

void cParticle::getPosition(double *x, double *y, double *z) const
{
    *x = this->pos[0];
    *y = this->pos[1];
    *z = this->pos[2];
}

void cParticle::getDirection(double *x, double *y, double *z) const
{
    *x = this->dir[0];
    *y = this->dir[1];
    *z = this->dir[2];
}

void cParticle::getPosition(double *newPos) const
{
    newPos[0] = this->pos[0];
    newPos[1] = this->pos[1];
    newPos[2] = this->pos[2];
}

void cParticle::getDirection(double *newDir) const
{
    newDir[0] = this->dir[0];
    newDir[1] = this->dir[1];
    newDir[2] = this->dir[2];
}

double cParticle::minDist2Origin() const
{
    /*
    double lambda;
    double d = 0;
    double x,y,z;
    double sumPos = 0;
    double sumDirSqr = 0;

    for(unsigned short i = 0; i < DIMENSIONS; i++){
        sumPos += pos[i] * dis[i];
        sumDirSqr += dir[i] * dir[i];
    }

    lambda =  (d - sumPos)/sumDirSqr;
    x = dir[0]* lambda + pos[0];
    y = dir[1]* lambda + pos[1];
    z = dir[2]* lambda + pos[2];
    */
    return minDist2Point(0,0,0);//sqrt(x * x + y * y + z * z);
}

double cParticle::minDist2Point(double x, double y, double z) const
{
    double lambda;
    double x1,y1,z1, xDiff, yDiff, zDiff;
    double sumPos = 0;
    double sumDirSqr = 0;
    sumPos += (pos[0] - x) * dir[0];
    sumPos += (pos[1] - y) * dir[1];
    sumPos += (pos[2] - z) * dir[2];
    for(unsigned short i = 0; i < DIMENSIONS; i++){
        sumDirSqr += dir[i] * dir[i];
    }

    lambda =   -sumPos/sumDirSqr;
    x1 = dir[0] * lambda + pos[0];
    y1 = dir[1] * lambda + pos[1];
    z1 = dir[2] * lambda + pos[2];

    xDiff = x - x1;
    yDiff = y - y1;
    zDiff = z - z1;

    return sqrt(xDiff * xDiff + yDiff * yDiff + zDiff * zDiff);
}


double cParticle::minDist2Point(double *point) const
{
    return minDist2Point(point[0], point[1], point[2]);
}

