/*
 * @brief Leakage.h中定义了计算漏电流功耗所需要的参数以及方法
 *
 * @date Created on: 2012-4-17
 * @author pc5
 */

#ifndef LEAKAGE_H_
#define LEAKAGE_H_

#include <cmath>
/*
 * @brief 计算中需要的参数
 */
const int NumberTransistor(1000000);
const double paraA=0.000000000011432;
const double paraB=0.00000000000010126;
const double alf=466.4029;
const double beta=-1224.74083;
const double gama=6.28153;
const double kesi=6.9094;
const double Voltage=1.2;
const double Is=120;

double ToAbsolute(double cel);

/*
 * @brief 计算漏电流功耗
 * @param Temperauture 温度
 * @return 漏电流功耗
 */
double LeakagePower(double Temperature)
{
	double Absolute=ToAbsolute(Temperature);
	double part1=paraA*Absolute*Absolute*std::exp((alf*Voltage+beta)/Absolute);
	double part2=paraB*std::exp(gama*Voltage+kesi);
	double Ileak=Is*(part1+part2);
	double Pleak=NumberTransistor*Ileak*Voltage;
	return Pleak;
}

/*
 * @brief 摄氏温度转换为绝对温度+环境温度
 * @param cel 摄氏温度
 * @return 绝对温度
 */
double ToAbsolute(double cel)
{
	double amb=25.0;
	double abs=273.15;
	abs=abs+cel+amb;
	return abs;
}

#endif /* LEAKAGE_H_ */
