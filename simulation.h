#pragma once
#include "lib/spectrum.h"
#include "lib/MedImage.h"

class cSimulation
{
    private:
        double tubePos[3];
        double tubeVoltage;
        double spherePos[3];
        double sphereRadius;
        double pixelSize; // m
        cSpectrum energy;
        cSpectrum xRayTube;
        cSpectrum attCoeff;
        double currentTimeProduct = 10; //mAs
    public:
        cSimulation();
        ~cSimulation();
        void setCurrentTimeProduct(double newCurrentTimeProduct);
        void prepare();
        void simulate(cMedImage<double> &cmi);
        void setTubePosition();
        double getTubeVoltage() const;
        void setTubeVoltage(double newTubeVoltage);
        double getSphereRadius() const;
        void setSphereRadius(double newSphereRadius);
        double getPixelSize() const;
        void setPixelSize(double newPixelSize);
        void getTubePos(double *tubePos);
        void setTubePos(double x, double y, double z);
        void getSpherePos(double *tubePos);
        void setSpherePos(double x, double y, double z);
};
