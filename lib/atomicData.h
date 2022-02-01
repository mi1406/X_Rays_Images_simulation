#pragma once
#include <vector>
#include <iostream>
#define THROW throw std::runtime_error
#define Na 6.022140867e23

class cAtomicData{
public:
    typedef struct{
        float x, y;
    }tPoint2d;

private:
    static std::vector<tPoint2d> tcs[100];
    static float A[100];
    static bool prepared;
public:
    cAtomicData();
    ~cAtomicData();
    void prepare();
    double getStdAtomicWeight(unsigned Z);
    double getTotalCrossSection(unsigned Z, double energy);
};
