#include "atomicData.h"
#include <fstream>
#include <cstring>
#include <string>
#include <math.h>

using namespace std;
bool cAtomicData::prepared = false;
vector<cAtomicData::tPoint2d> cAtomicData::tcs[100];
float cAtomicData::A[100];
cAtomicData::cAtomicData(){
}
cAtomicData::~cAtomicData(){
}

void cAtomicData::prepare(){
    if(prepared)
        return;
    ifstream inFile("//home//m//qtWorkSpace//xrays//X-rays_sim_assignment_2//totalCrossSection.dat", ios::binary);
    if(!inFile.is_open()){
        THROW("File not found");
    }
    const string nameIdentifier = "atomic-data";
    char title[12];
    inFile.read(title, 11);
    title[11] = '\0';
    if(nameIdentifier.compare(title)){
        THROW("File is not atomic data");
    }
    uint32_t strLength;
    inFile.read((char*)&strLength, sizeof(uint32_t));
    char* str = new char[strLength];
    inFile.read(str, strLength);
    for(int j = 0; j < 100; j++){
        uint32_t atomicNumber;
        inFile.read((char*)&atomicNumber, sizeof(atomicNumber));
        float atomicWeight;
        inFile.read((char*)&atomicWeight, sizeof(atomicWeight));
        A[atomicNumber - 1] = atomicWeight;
        uint32_t numeberOfEntries;
        inFile.read((char*)&numeberOfEntries, sizeof(numeberOfEntries));
        tcs[j].resize(numeberOfEntries);
        for(uint32_t i = 0; i < numeberOfEntries; i++){
            float energy;
            inFile.read((char*)&energy, sizeof(float));
            tcs[j][i].x = energy; //MeV
            float crossSection;
            inFile.read((char*)&crossSection, sizeof(float));
            tcs[j][i].y = crossSection; //barn
        }
    }
    prepared = true;
}

double cAtomicData::getStdAtomicWeight(unsigned Z)
{
    if(Z > 100 || Z < 1)
        THROW("Index out of range");
    Z -= 1;
    return A[Z];
}

double cAtomicData::getTotalCrossSection(unsigned Z, double energy)
{
    if(Z > 100 || Z < 1)
        THROW("Index out of bound");
    Z -= 1;
    auto vec = tcs[Z];
    energy /= 1000;

    if(energy <= vec[0].x) return vec[0].y;
    if(energy >= vec.back().x) return vec.back().y;

    float min = 1000;
    float cur = 0;
    int index = 1000;
    for(unsigned i = 0; i < vec.size(); i++){
        cur = abs(vec[i].x - energy);
        if(cur < min){
            min = cur;
            index = i;
        }
    }

    double sigma = -1;
    unsigned k = 1000;
    if(vec[index].x <= energy){
        k = index + 1;
        sigma = vec[index].y*exp(log(vec[k].y/vec[index].y)*(log(energy/vec[index].x)/log(vec[k].x/vec[index].x)));
    }else if(vec[index].x > energy){
        k = index - 1;
        sigma = vec[k].y*exp(log(vec[index].y/vec[k].y)*(log(energy/vec[k].x)/log(vec[index].x/vec[k].x)));
    }

    return sigma;
}


