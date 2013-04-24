/*
 * Leakage.h
 *
 *  Created on: 2012-4-17
 *      Author: pc5
 */

#ifndef LEAKAGE_H_
#define LEAKAGE_H_

#include <cmath>
const int NumberTransistor(1000000);
const double paraA=0.000000000011432;
const double paraB=0.00000000000010126;
const double alf=466.4029;
const double beta=-1224.74083;
const double gama=6.28153;
const double kesi=6.9094;
const double Voltage=1.2;
const double Is=1040;

double ToAbsolute(double cel);

double LeakagePower(double Temperature)
{
	double Absolute=ToAbsolute(Temperature);
	double part1=paraA*Absolute*Absolute*std::exp((alf*Voltage+beta)/Absolute);
	double part2=paraB*std::exp(gama*Voltage+kesi);
	double Ileak=Is*(part1+part2);
	double Pleak=NumberTransistor*Ileak*Voltage;
	return Pleak;
}

double ToAbsolute(double cel)
{
	double amb=25.0;
	double abs=273.15;
	abs=abs+cel+amb;
	return abs;
}

#endif /* LEAKAGE_H_ */
