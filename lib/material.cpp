/*
 * Filename: material.cpp
 * Author: Mario Ivanov
 * Version: 1.00
 * Date: 27.12.2021
 * Description: OOP class for calculating attenuation coeficient of a composite material
 */

#include "material.h"
#include "math.h"

void cMaterial::clear()
{
    this->name = "new material";
    Z.clear();
    fraction.clear();
    density = 1;
}

cMaterial::cMaterial()
{
    clear();
}

cMaterial::cMaterial(const cMaterial &material)
{
    this->Z = material.Z;
    this->fraction = material.fraction;
    this->density = material.density;
    this->name = material.name;
}

cMaterial::~cMaterial()
{
}

void cMaterial::setName(const std::string &newName)
{
    if(newName.empty())
        THROW("Can't set name to NULL");
    this->name = newName;
}

const std::string &cMaterial::getName()
{
    return this->name;
}

void cMaterial::addElement(unsigned newZ, double newFraction)
{
    Z.emplace_back(newZ);
    fraction.emplace_back(newFraction);
}

unsigned cMaterial::getNoOfElements()
{
    return Z.size();
}

unsigned cMaterial::getAtomicNumber(unsigned i)
{
    if(i > Z.size() && i < 1)
        THROW("Index out of bounds");
    return Z[i - 1];
}

double cMaterial::getFraction(unsigned i)
{
    if(i > fraction.size() && i < 1)
        THROW("Index out of bounds");
    return fraction[i - 1];
}

void cMaterial::setDensity(double newDensity)
{
    this->density = newDensity;
}

double cMaterial::getDensity()
{
    return this->density;
}

double cMaterial::getAttCoeff(double energy)
{
    cAtomicData::prepare();
    double den = 0;
    double num = 0;

    for(unsigned i = 0; i < Z.size(); i++){
        double atomicWeight = cAtomicData::getStdAtomicWeight((unsigned)Z[i]);
        double totalCrossSection = cAtomicData::getTotalCrossSection((unsigned)Z[i], energy);
        num += totalCrossSection * fraction[i];
        den += atomicWeight * fraction[i];
    }
    double cubicCMtoCubicMeter = 1e6;
    double attCoeff = num/den * (density * cubicCMtoCubicMeter) * Na;
    double barnToMeter = 1e-28;
    return attCoeff * barnToMeter;
}

void cMaterial::getAttSpec(std::vector<double> &spec, double minEnergy, double tubeVoltage, unsigned energySteps)
{
    double inc = (tubeVoltage - minEnergy) / energySteps;
    spec.resize(energySteps);
    for(unsigned i = 0; i < energySteps; i++){
        spec[i] = getAttCoeff(minEnergy);
        minEnergy += inc;
    }
}

double cMaterial::getMeanFreePath(double energy)
{
    return 1.0/getAttCoeff(energy);
}
