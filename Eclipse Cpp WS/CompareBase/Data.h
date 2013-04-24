/*
 * Data.h
 *
 * @brief ����оƬ�ĳߴ硢�ṹ�Լ����衢���ݵ��Ȳ������ṩ�ˣ���ȡ�����ļ��ĺ�������ʾ�����������ĺ��������㲢������ĺ���
 * @date 2012-4-9
 * @author pc5
 */


#include <vector>
#include <iomanip>
#include <fstream>
#include <cstring>
#include <cmath>
/* @brief
 * A��оƬ���
 * h_si h_cu ��ĵ׺�ͭɢ��Ƭ�ĺ��
 * gRatioΪ�ȵ��ʣ�W/��cm*K����
 * cRatioΪ�����ݣ�J/��kg*K����
 * mRatioΪ�ܶȣ�kg/cm3��
 * SumPowerΪ�ܹ����ܶȣ�W/cm2��
 */
const double A(4.35);
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
const double r_system=0.146*A;
const double r_fan=r_system-r_bulk-r_conduct;

//���Թ���RC���ʱ��
const double RCtime=(r_conduct+r_fan)*c_conduct;
const double h=0.001*RCtime;//�ָ�ʱ��Ƭ
const double Tamb=0;
std::vector<double> CorePower;

int CoreNumber;
int IterNumber(0);
int ForceStop(1000);
int SetWidth(12);
int SetPrecision(8);

/*
 * @brief
 * ��ʽ�������@param nameΪ���ֵ�����@param v
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
 * @brief ������������@param r1��@param r2�Ĳ���ֵ
 */
double Pararlle(double r1,double r2)
{
	if(std::fabs(r1)<0.00001)
		return r2;
	if(std::fabs(r2)<0.00001)
		return r1;
	if(std::fabs(r2)<0.00001 && std::fabs(r2)<0.00001)
		return 0;
	else
		return r1*r2/(r1+r2);
}

/*
 * @brief ���ļ�@param PowerFile�ж�ȡ���ʵ��й���Ϣ
 * 1. ����@param CoreNumber
 * 2. �ܹ���@param SumPower
 */
void GetPower(char* PowerFile)
{
	std::ifstream in;
	in.open(PowerFile);
	in>>CoreNumber>>SumPower;
	SumPower=SumPower/A;
	in.close();
}

/*
 * ���������ߣ����£���ʱ���ݱ�����е�ֵ
 */
double ChargeLine(double sumPower,double R_base,double C_base,int n)
{
	double Vtmp=sumPower*R_base;
	Vtmp=Vtmp*(1-std::exp((-h*n)/RCtime));
	return Vtmp;
}










