#include "simulation.h"
#include "lib/material.h"
#include "lib/particle.h"
#include <vector>
#include <cmath>
#include "lib/random.h"

using namespace std;

double cSimulation::getTubeVoltage() const
{
    return tubeVoltage;
}

void cSimulation::setTubeVoltage(double newTubeVoltage)
{
    tubeVoltage = newTubeVoltage;
}

double cSimulation::getSphereRadius() const
{
    return sphereRadius;
}

void cSimulation::setSphereRadius(double newSphereRadius)
{
    sphereRadius = newSphereRadius;
}

double cSimulation::getPixelSize() const
{
    return pixelSize;
}

void cSimulation::setPixelSize(double newPixelSize)
{
    pixelSize = newPixelSize;
}

void cSimulation::getTubePos(double *tubePos)
{
    tubePos[0] = this->tubePos[0];
    tubePos[1] = this->tubePos[1];
    tubePos[2] = this->tubePos[2];
}

void cSimulation::setTubePos(double x, double y, double z)
{
    tubePos[0] = x;
    tubePos[1] = y;
    tubePos[2] = z;
}

void cSimulation::getSpherePos(double *spherePos)
{
    spherePos[0] = this->spherePos[0];
    spherePos[1] = this->spherePos[1];
    spherePos[2] = this->spherePos[2];
}

void cSimulation::setSpherePos(double x, double y, double z)
{
    spherePos[0] = x;
    spherePos[1] = y;
    spherePos[2] = z;
}

cSimulation::cSimulation()
{
    tubePos[0] = 0;//m
    tubePos[1] = 0;
    tubePos[2] = 1.2;
    tubeVoltage = 75; //kev
    spherePos[0] = 0;
    spherePos[1] = 0; //m
    spherePos[2] = 0.2;
    sphereRadius = 15e-2; //m
    pixelSize = 0.01; //m
}

cSimulation::~cSimulation()
{

}

void cSimulation::setCurrentTimeProduct(double newCurrentTimeProduct)
{
    this->currentTimeProduct = newCurrentTimeProduct;
}

void cSimulation::prepare()
{
    double minEnergy = 10;
    std::string fileName = "/home/m/qtWorkSpace/xray_sim_simulation/SRO33100ROT350.dat";
    std::string name = "SRO33100-ROT350";
    xRayTube.readSpectrum(fileName, tubeVoltage, minEnergy, name);  //lambdaK
    double x = (tubeVoltage - minEnergy)/100;
    double energy_tmp = minEnergy - x;
    for(unsigned i = 0; i < xRayTube.size(); i++){
        energy_tmp += x;
        xRayTube[i] *= energy_tmp;
    }
    cMaterial cm;
    cm.setName("Pure Water");
    cm.addElement(1, 2);
    cm.addElement(8, 1);
    vector<double> spec;
    cm.getAttSpec(spec, minEnergy, tubeVoltage, xRayTube.size());
    attCoeff = spec;
    energy.resize(xRayTube.size());
    for(unsigned i = 0; i < energy.size(); i++)
        energy[i] = minEnergy + i * (tubeVoltage - minEnergy)/energy.size(); // Ek?

    double sid = sqrt(tubePos[0] * tubePos[0]  + tubePos[1] * tubePos[1] + tubePos[2] * tubePos[2]);
    double solidAngleF = (pixelSize * pixelSize / (sid * sid)) * 1e6;
    double correctionFactor = solidAngleF;
    correctionFactor *= currentTimeProduct;
    correctionFactor *= (tubeVoltage - minEnergy)/xRayTube.size();
    xRayTube *= correctionFactor;
}

void cSimulation::simulate(cMedImage<double> &cmi)
{
    unsigned rows = cmi.nRow;
    unsigned col = cmi.nCol;
    double stdev = 0;
    cRandKiss kiss((uint32_t)time(NULL));
    cRandNormal normal(kiss);
    normal.prepare();
    double distMin = 10000;
    cParticle particle;
    particle.setPosition(tubePos[0], tubePos[1], tubePos[2]);
    int xPixel, yPixel;
    for(unsigned i = 0; i < rows; i++){
        for(unsigned j = 0; j < col; j++){
            if(rows == col){
                int midYX = rows / 2;
                xPixel = i - midYX;
                yPixel = j - midYX;
            }else{
                int midX = rows/2;
                int midY = col/2;
                xPixel = midX - i;
                yPixel = midY - j;
            }

            double x = xPixel * pixelSize - tubePos[0]; //m
            double y = yPixel * pixelSize - tubePos[1]; //m
            double z = -tubePos[2]; //m
            double l = sqrt(x * x + y * y + z * z);//m
            double dir[3] = { x/l, y/l, z/l };
            particle.setDirection(dir);
            double dist  = particle.minDist2Point(spherePos[0], spherePos[1], spherePos[2]); //m

            double t = 0;

            if (dist >= sphereRadius){
                t = 0;
            }else{
                t = 2*sqrt((sphereRadius * sphereRadius) - (dist * dist));
            }

            cSpectrum spec;
            spec = attCoeff;
            spec *= -t;
            spec.exp();
            spec *= xRayTube;

            double expectedE = spec.sum();
            spec *= energy;
            stdev = sqrt(spec.sum());
            cmi[i][j] = log(expectedE + stdev*normal.rand());
        }
    }
    cout << distMin << endl;
}
