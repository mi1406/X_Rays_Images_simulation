#pragma once

#include <vector>
#include <string>

class cSpectrum
{
private:
	std::vector<double> data;
public:
    cSpectrum() {}
	cSpectrum(const cSpectrum &);
    ~cSpectrum() {}
	size_t size();
	void resize(size_t);
	double& operator[](size_t);
	cSpectrum& operator=(double);
	cSpectrum& operator=(const std::vector<double> &);
	cSpectrum& operator=(const cSpectrum&);

	cSpectrum& operator+=(const cSpectrum&);
	cSpectrum& operator*=(const cSpectrum&);
	cSpectrum& operator*=(double);
	cSpectrum& exp();
	double sum();

	void readSpectrum(const std::string& filename, double tubeVoltage, double& minEnergy, std::string& spectrumName);
	void writeScv(const std::string& filename, double minEnergy, double tubeVoltage);
};
