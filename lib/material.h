/*
 * Filename: material.h
 * Author: Mario Ivanov
 * Version: 1.00
 * Date: 27.12.2021
 * Description: OOP class for calculating attenuation coeficient of a composite material
 */
#pragma once
#include "atomicData.h"
#include <vector>
#include <string>

class cMaterial : cAtomicData{
private:
    std::vector<unsigned> Z;
    std::vector<double> fraction;
    double density;
    std::string name;
    void clear();
public:
    cMaterial();  //tested main.cpp/testMaterial:41-49
    cMaterial(const cMaterial &material); //tested main.cpp/testMaterial:54-77
    ~cMaterial(); //nothing to test

    void setName(const std::string &newName); //tested main.cpp/testMaterial:90-108
    const std::string &getName(); //tested main.cpp/testMaterial:90-108
    void addElement(unsigned newZ, double newFraction); //tested main.cpp/testMaterial:90-108
    unsigned getNoOfElements(); //tested main.cpp/testMaterial:90-108
    unsigned getAtomicNumber(unsigned i); //tested main.cpp/testMaterial:90-108
    double getFraction(unsigned i); //tested main.cpp/testMaterial:90-108
    void setDensity(double newDensity); //tested main.cpp/testMaterial:90-108
    double getDensity(); //tested main.cpp/testMaterial:90-108
    double getAttCoeff(double energy); //tested main.cpp/testMaterial:115-121
    void getAttSpec(std::vector<double> &spec, double minEnergy, double tubeVoltage, unsigned energySteps); //tested main.cpp/testMaterial:128-158
    double getMeanFreePath(double energy); // tested with getAttCoef since the calculation is just 1/getAttCoeff()
    friend void testMaterial();
};
