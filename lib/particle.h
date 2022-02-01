#pragma once
#define DIMENSIONS 3

class cParticle
{
private:
    double pos[DIMENSIONS];
    double dir[DIMENSIONS];
public:
    cParticle();
    cParticle(const cParticle& newParticle);
    ~cParticle();

    void setPosition(double x, double y, double z);
    void setDirection(double x, double y, double z);
    void setPosition(double* newPos);
    void setDirection(double* newDir);

    void getPosition(double *x, double *y, double *z) const;
    void getDirection(double *x, double *y, double *z) const;
    void getPosition(double* newPos) const;
    void getDirection(double* newDir) const;

    double minDist2Origin() const;
    double minDist2Point(double x, double y, double z) const;
    double minDist2Point(double* point) const;
};

