#include "spectrum.h"
#include <iostream>
#include <fstream>    
#include <math.h>

#define THROW throw std::runtime_error

cSpectrum::cSpectrum(const cSpectrum& spec) {
    data = spec.data;
}

size_t cSpectrum::size() {
    return data.size();
}

void cSpectrum::resize(size_t newSize) {
    data.resize(newSize);
}

double& cSpectrum::operator[](size_t index) {
    if (index > data.size())
        throw(std::runtime_error("Index out of range in class cSpectrum"));
    return data[index];
}

cSpectrum& cSpectrum::operator=(double value) {
    for (unsigned i = 0; i < data.size(); i++)
        data[i] = value;
    return *this;
}

cSpectrum& cSpectrum::operator=(const std::vector<double>& spec) {
    data = spec;
    return *this;
}

cSpectrum& cSpectrum::operator=(const cSpectrum& spec) {
    if (!spec.data.empty())
        data = spec.data;
    return *this;
}

cSpectrum& cSpectrum::operator+=(const cSpectrum& summand) {
    if (data.size() != summand.data.size())
        throw(std::runtime_error("Cannot sum different length spectra"));
    for (unsigned i = 0; i < data.size(); i++)
        data[i] += summand.data[i];
    return *this;
}
cSpectrum& cSpectrum::operator*=(const cSpectrum& factor) {
    if (data.size() != factor.data.size())
        throw(std::runtime_error("Cannot multiply different length spectra"));
    for (unsigned i = 0; i < data.size(); i++)
        data[i] *= factor.data[i];
    return *this;
}
cSpectrum& cSpectrum::operator*=(double factor) {
    for (unsigned i = 0; i < data.size(); i++)
        data[i] *= factor;
    return *this;
}
cSpectrum& cSpectrum::exp() {
    for (unsigned i = 0; i < data.size(); i++)
        data[i] = std::exp(data[i]);
    return *this;
}
double cSpectrum::sum() {
    double sum = 0;
    for (unsigned i = 0; i < data.size(); i++)
        sum += data[i];
    return sum;
}

void cSpectrum::readSpectrum(const std::string& filename, double tubeVoltage, double& minEnergy, std::string& spectrumName) {
    std::ifstream inp(filename, std::ios::binary);


    if (!inp.is_open())
        THROW("FIle not found!");
    char fileName[14];
    inp.read(fileName, 14);
    std::string header = "x-ray spectrum";
    for (unsigned i = 0; i < header.length()- 1; i++) {
        if (fileName[i] != header[i])
            THROW("File name doesn't match");
    }
    uint32_t nameN;
    inp.read((char*)&nameN, sizeof(nameN));
    char* name = new char[nameN + 1];

    inp.read(name, nameN);
    name[nameN] = 0;
    std::cout << name << std::endl;
    std::cout << spectrumName << std::endl;
    if (name != spectrumName)
        THROW("Name mismatch");

    uint32_t numSpectns;
    inp.read((char*)&numSpectns, sizeof(numSpectns));

    bool found = false;
    for (uint32_t i = 0; i < numSpectns; i++) {
        float tubeVoltageRead;
        inp.read((char*)&tubeVoltageRead, sizeof(tubeVoltageRead));
        float minEnergyRead;
        inp.read((char*)&minEnergyRead, sizeof(minEnergyRead));

        if (round(minEnergyRead*10)/10 != round(minEnergy*10)/10)
            THROW("Mismatch between read and provided min energy");

        uint32_t tableLen;

        inp.read((char*)&tableLen, sizeof(tableLen));
        if (round(tubeVoltageRead * 10) / 10 != round(tubeVoltage * 10) / 10) {
            inp.seekg(tableLen * (uint64_t)sizeof(tableLen), inp.cur);
            continue;
        }
        found = true;

        data.resize(tableLen);
        for (unsigned i = 0; i < tableLen; i++) {
            float f;
            inp.read((char*)&f, sizeof(f));
            data[i] = (double)f;
        }
        inp.close();

        break;
    }
    if (!found)
        THROW("Tube voltage not found in file.");
    delete[] name;
}

void cSpectrum::writeScv(const std::string& filename, double minEnergy, double tubeVoltage)
{
    std::ofstream out;

    //open file
    out.open(filename);
    if (!out.is_open())
        THROW("File not found");
    if (data.empty())
        THROW("Dat needs t be provided by reading a spectra first!");
    out << "energy, intensity" << std::endl;
    for (unsigned i = 0; i < data.size(); i++)
        out << minEnergy + i * (tubeVoltage - minEnergy) / data.size() << ", " << data[i] << std::endl;
    out << tubeVoltage << ", " << 0 << std::endl;
}
