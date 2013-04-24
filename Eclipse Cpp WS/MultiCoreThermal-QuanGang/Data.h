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

/* @brief ������������Ķ���
 * A��оƬ���
 * Aper��ÿ���˵����
 * h_si h_cu ��ĵ׺�ͭɢ��Ƭ�ĺ��
 * gRatioΪ�ȵ��ʣ�W/��cm*K����
 * cRatioΪ�����ݣ�J/��kg*K����
 * mRatioΪ�ܶȣ�kg/cm3��
 * SumPowerΪ�ܹ��ģ�W��
 * CoreNumberΪ������֮ǰ�汾���Ǵ�Power.txt�ļ����룬��һ���Ϊ����Data.h�ж���
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


//ͭ���Ȳ�����ݡ��������
const double r_conduct=h_cu/gRatio_cu;
const double c_conduct=cRatio_cu*mRatio_cu*h_cu;

//��ĵ׵����ݡ��������
const double r_bulk=h_si/gRatio_si;
const double c_bulk=cRatio_si*mRatio_si*h_si;

//ɢ��ϵͳ���������
const double r_system=0.146;
const double r_fan=r_system-r_bulk-r_conduct;

//���Թ���RC���ʱ��
const double RCtime=(r_bulk*r_conduct/(r_bulk+r_conduct))*(c_bulk+c_conduct);
const double h=0.0001*RCtime;//�ָ�ʱ��Ƭ
const double Tamb=0;
std::vector<double> CorePower;
std::vector<double> Acore;

int IterNumber(1);
int SetWidth(14);
int SetPrecision(8);

/*
 * @brief ��һ��vectorд�뵽�ļ���
 * @param v ��д����
 * @param name �ļ����֣���չ��Ϊ.txt
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
 * @brief ����@param r1��@param r2�Ĳ���ֵ
 * @return ����ֵ
 */
double Pararlle(double r1,double r2)
{
	return r1*r2/(r1+r2);
}

/*
 * @brief ���빦���ļ�������һЩ����
 * @param PowerFile �ļ���
 */
void GetPower(char* PowerFile)
{
	std::ifstream in;
	in.open(PowerFile);
	in>>CoreNumber;
	in>>SumPower;

	//���Ķ��ǹ����ܶȣ�������������������������
	//SumPower=SumPower/A;

	double tmp;
	for(int i=0;i<CoreNumber;i++)
	{
		in>>tmp;
		CorePower.push_back(SumPower*tmp);//��ʼ�����еĹ���
	}

	in>>Aper;
	A=Aper*CoreNumber;
	for(int i=0;i<CoreNumber;i++)
	{
		in>>tmp;
		Acore.push_back(A*tmp);//��ʼ�����еĹ���
	}

	in.close();

}

/*
 * @brief ���к˵������ȶ�ֵ
 * @param Power ����
 * @param R_base base������
 * @param R_core core������
 * @return V_tmp �ȶ��¶ȵ�����
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
 * @brief ��Base�������ȶ�ֵ
 * @param Power����
 * @param R_base base������
 * @param V_tmp base���ȶ�ֵ
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
 * @brief �ó�����߼���Ա�ֵ
 * @param sumPower����
 * @param R_base
 * @param C_base ȷ��ʱ�䳣��
 * @param n ʱ�䲽
 */
double ChargeLine(double sumPower,double R_base,double C_base,int n)
{
	double Vtmp=sumPower*R_base;
	double tao=R_base*C_base;
	Vtmp=Vtmp*(1-std::exp((-h*n)/tao));
	return Vtmp;
}



#endif /* DATA_H_ */
