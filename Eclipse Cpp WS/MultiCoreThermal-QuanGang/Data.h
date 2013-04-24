/*
 * Data.h
 *
 *  Created on: 2012-4-9
 *      Author: pc5
 */

#ifndef DATA_H_
#define DATA_H_

#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>
#include <cstring>
#include <iomanip>

/* @brief 各种物理参数的定义
 * A：芯片面积
 * Aper：每个核的面积
 * h_si h_cu 硅衬底和铜散热片的厚度
 * gRatio为热导率（W/（cm*K））
 * cRatio为比热容（J/（kg*K））
 * mRatio为密度（kg/cm3）
 * SumPower为总功耗（W）
 * CoreNumber为核数，之前版本都是从Power.txt文件读入，这一版改为了在Data.h中定义
 */
int CoreNumber;
int ForceStop(100000);
double A,Aper;
const double h_si(5e-2);
const double h_cu(1);

const double gRatio_si(1);
const double cRatio_si(171);
const double mRatio_si(2.32e-3);

const double gRatio_cu(4);
const double cRatio_cu(386);
const double mRatio_cu(8.92e-3);

double SumPower;


//铜导热层的热容、热阻参数
const double r_conduct=h_cu/gRatio_cu;
const double c_conduct=cRatio_cu*mRatio_cu*h_cu;

//硅衬底的热容、热阻参数
const double r_bulk=h_si/gRatio_si;
const double c_bulk=cRatio_si*mRatio_si*h_si;

//散热系统的热阻参数
const double r_system=0.146;
const double r_fan=r_system-r_bulk-r_conduct;

//粗略估计RC充电时间
const double RCtime=(r_bulk*r_conduct/(r_bulk+r_conduct))*(c_bulk+c_conduct);
const double h=0.0001*RCtime;//分隔时间片
const double Tamb=0;
std::vector<double> CorePower;
std::vector<double> Acore;

int IterNumber(1);
int SetWidth(14);
int SetPrecision(8);

/*
 * @brief 将一个vector写入到文件中
 * @param v 待写向量
 * @param name 文件名字，扩展名为.txt
 */
void DisplayVector(std::vector<double> v, char* name)
{
	char dis[20];
	strcpy(dis,name);
	strcat(dis,".txt");
	std::ofstream out(dis);

	if(not out)
		std::perror(dis);

	out.precision(SetPrecision);

	for(int i=0;i<CoreNumber;i++)
		out<<std::setw(SetWidth-3)<<std::right<<name<<"("<<i+1<<")";
	out<<std::endl;

	for(std::vector<double>::iterator i=v.begin();i!=v.end();i++)
		out<<std::setw(SetWidth)<<*i;
	out<<std::endl;

	out.close();
}

/*
 * @brief 计算@param r1与@param r2的并联值
 * @return 并联值
 */
double Pararlle(double r1,double r2)
{
	return r1*r2/(r1+r2);
}

/*
 * @brief 读入功率文件，配置一些参数
 * @param PowerFile 文件名
 */
void GetPower(char* PowerFile)
{
	std::ifstream in;
	in.open(PowerFile);
	in>>CoreNumber;
	in>>SumPower;

	//功耗而非功耗密度！！！！！！！！！！！！！
	//SumPower=SumPower/A;

	double tmp;
	for(int i=0;i<CoreNumber;i++)
	{
		in>>tmp;
		CorePower.push_back(SumPower*tmp);//初始化核中的功率
	}

	in>>Aper;
	A=Aper*CoreNumber;
	for(int i=0;i<CoreNumber;i++)
	{
		in>>tmp;
		Acore.push_back(A*tmp);//初始化核中的功率
	}

	in.close();

}

/*
 * @brief 所有核的理论稳定值
 * @param Power 功率
 * @param R_base base的热阻
 * @param R_core core的热阻
 * @return V_tmp 稳定温度的向量
 */
std::vector<double> StableVcore(std::vector<double> Power,std::vector<double> R_core,double R_base)
{
	std::vector<double> V_tmp;
	double sum=0;
	for(std::vector<double>::iterator i=Power.begin();i!=Power.end();++i)
	{
		sum+=*i;
	}
	double V=sum*R_base;

	for(int i=0;i<CoreNumber;++i)
	{
		V_tmp.push_back(V+R_core.at(i)*Power.at(i));
	}
	return V_tmp;
}

/*
 * @brief 求Base的理论稳定值
 * @param Power功率
 * @param R_base base的热阻
 * @param V_tmp base的稳定值
 */
double StableVbase(std::vector<double> Power,double R_base)
{
	double V_tmp;
	double sum=0;
	for(std::vector<double>::iterator i=Power.begin();i!=Power.end();++i)
	{
		sum+=*i;
	}
	V_tmp=sum*R_base;
	return V_tmp;
}

/*
 * @brief 用充电曲线计算对比值
 * @param sumPower功率
 * @param R_base
 * @param C_base 确定时间常数
 * @param n 时间步
 */
double ChargeLine(double sumPower,double R_base,double C_base,int n)
{
	double Vtmp=sumPower*R_base;
	double tao=R_base*C_base;
	Vtmp=Vtmp*(1-std::exp((-h*n)/tao));
	return Vtmp;
}



#endif /* DATA_H_ */
