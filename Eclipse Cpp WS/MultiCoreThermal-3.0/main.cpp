/*
 * main.cpp
 *
 * @brief ģ����оƬ�ϣ����ģ����������µ��¶ȣ���ѹ�����ߡ�
 * ��ԭ�������+2.5��,��϶���3.0��
 * ԭ�����ԣ��������������ȶ�����ֵ����������м����ֵ���������⵼��������̷���������ļ��ϴ�
 * 2.5���ԣ��������������ȶ�ֵ�ļ��㣬�Լ�@Vbase�������ߵĵ���ֵ�Ƚ�
 * 3.0�����ԣ������ܶȳ�Ϊ�¶ȵĺ�������������ÿ�������¶�ֵ��������©�������ģ����Leakage.h����Ӧ��Vbase��Vcore��������ߣ�
 * ���ĵı仯���̱�д����Pcore.txt�У������Ѿ��ǹ��ģ������ǹ����ܶ�
 * ������֮����ÿ�����ϵ����AcoreĿǰ��A/CoreNumber������չΪ��ͬblock�������ͬ����Ҫ��Ϊһ���������Acore
 *
 * @date Created on: 2012-4-9
 * @author pc5
 *
 */

#include <vector>
#include <iostream>
#include <ostream>
#include <istream>
#include <iomanip>
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <fstream>

#include "Data.h"
#include "Leakage.h"

int main()
{
	char powerfile[]="Power.txt";
	GetPower(powerfile);
	/*
	 * �������еı���
	 */
	std::vector<double> Vcore;
	std::vector<double> Icore;
	std::vector<double> Rcore;
	std::vector<double> ecore;//�˵������еĵ���Դ����
	std::vector<double> rcore;//�˵������е����貿��
	std::vector<double> Ipoint;

	/*
	 * ���������еı���
	 */
	double Vbase;
	double Ibase;
	double Rbase((r_conduct+r_fan)/A);//��ʼ�����׵����裬/A��ʾ���ܶȻ���
	double ebase(0);
	double rbase(h/(2*(c_conduct*A)));//��ʼ�����׵������е����貿��=h/2c��*A��ʾ���ܶȻ���

	/*
	 * ��ʼ�������е�@Vbase��@ebase
	 */
	for(int i=0;i<CoreNumber;i++)
	{
		//CorePower.push_back(SumPower/CoreNumber);//��ʼ�����еĹ���
		Icore.push_back(0);//��ʼ�������еĵ���Ϊ0
		Rcore.push_back(r_bulk/Acore);//��ʼ�����е����裬/Acore��ʾ���ܶȻ���
	}
	Vbase=0;//Isum*Rbase+Tamb;//��ʼ�� @Vbase
	ebase=0;//Vbase/rbase;//��ʼ�����׵������еĵ���Դ����


	/*
	 * ��ʼ�����е�@Vcore��@ecore
	 */
	for(int i=0;i<CoreNumber;i++)
	{
		//Vcore.push_back(CorePower.at(i)*Rcore.at(i)+Vbase);//��ʼ��@Vcore
		Vcore.push_back(0);
		rcore.push_back(h/(2*(c_bulk*Acore)));//��ʼ��@rcore=h/2c��*Acore��ʾ���ܶȻ���
		ecore.push_back(0);
		Ipoint.push_back(0);
	}

	/*
	 * ������Ч��·�У�����������������貿�ֵĵ�Чֵ
	 */
	std::vector<double> r;
	for(int i=0;i<CoreNumber;i++)
	{
		r.push_back(Rcore.at(i)+rcore.at(i));//�����Чֵ
	}
	double rtmp=r.at(0);//��������@Vbase����ʱ������������@rcore�Ĳ���ֵ
	double Itmp(0);
	/*
	 * ����Vbaseʱ�õ������貢����
	 */
	for(int i=0;i<CoreNumber-1;i++)
	{
		rtmp=Pararlle(rtmp,r.at(i+1));//���㲢��ֵ
	}
	rtmp=Pararlle(rtmp,Pararlle(Rbase,rbase));//��@Rbase����


	std::ofstream Vout("Vcore.txt");
	std::ofstream Pout("Pcore.txt");
	std::ofstream Bout("Base.txt");
	if(not Vout)
		std::perror("Vcore.txt");
	if(not Bout)
		std::perror("Residual.txt");
	if(not Pout)
		std::perror("Residual.txt");

	Vout.precision(SetPrecision);
	Bout.precision(SetPrecision);
	Pout.precision(SetPrecision);

	for(int i=0;i<CoreNumber;i++)
		Pout<<std::setw(SetWidth)<<std::right<<"Pcore("<<i+1<<")";
	Pout<<"\n";

	for(int i=0;i<CoreNumber;i++)
		Vout<<std::setw(SetWidth)<<std::right<<"Vcore("<<i+1<<")";
	Vout<<std::endl;

	Bout<<std::setw(SetWidth)<<"VBase";
	Bout<<std::setw(SetWidth)<<"Charge"<<"\n";


	std::vector<double> Vref;
	std::vector<double> newPower;
	for(int i=0;i<CoreNumber;i++)
	{
		Vref.push_back(Vcore.at(i));
		newPower.push_back(CorePower.at(i));
	}
	double eps(1);
	double compareVbase(0);

	do
	{
		for(std::vector<double>::iterator i=Vcore.begin();i!=Vcore.end();i++)
			Vout<<std::setw(SetWidth)<<*i;
		Vout<<std::endl;

		for(std::vector<double>::iterator i=newPower.begin();i!=newPower.end();i++)
			Pout<<std::setw(SetWidth)<<*i;
		Pout<<"\n";

		Bout<<std::setw(SetWidth)<<Vbase;
		Bout<<std::setw(SetWidth)<<compareVbase<<"\n";

		for(int i=0;i<CoreNumber;i++)
		{
			Vref.at(i)=Vcore.at(i);
		}
		/*
		* ˢ��@Vbase�Լ�@ebase
		*/
		Itmp=0;//��������@Vbase����ʱ�����������к��ڵ����ĺ�
		for(int j=0;j<CoreNumber;j++)
		{
			newPower.at(j)=CorePower.at(j)+LeakagePower(Vcore.at(j));//ˢ��ʵ�ʵĹ���ֵ=Դ����+©��������
			Ipoint.at(j)=((newPower.at(j)+ecore.at(j))*rcore.at(j)/r.at(j));
			Itmp+=Ipoint.at(j);//����@Itmp����Ҫÿ��ˢ��
		}

		Itmp+=ebase;

		//out<<std::setw(SetWidth)<<Itmp<<"\t";

		/*
		* ˢ��@Vcore��@Vbase��ֵ
		*/
		Vbase=rtmp*Itmp;

		double SumNewPower=0;//�������к˵�ʵ�ʹ�����ֵ�����Լ����ȶ�Vcore�Լ���������е�Vbase
		for(int i=0;i<CoreNumber;i++)
		{
			SumNewPower+=newPower.at(i);
		}
		compareVbase=ChargeLine(SumNewPower,Rbase,c_conduct*A,IterNumber++);//Cbase=c_conduct*A!!!
		for(int i=0;i<CoreNumber;i++)
		{
			Vcore.at(i)=Vbase+(Ipoint.at(i)-Vbase/r.at(i))*Rcore.at(i);
		}

		/*
		* ˢ��@Ibase��@ebase��ֵ
		*/
		Ibase=0;
		for(int i=0;i<CoreNumber;i++)
		{
			Ibase+=(Vcore.at(i)-Vbase)/Rcore.at(i);
		}
		Ibase+=-Vbase/Rbase;
		ebase=Vbase/rbase+Ibase;//����@ebase����Ҫÿ��ˢ��

		/*
		* ˢ��@Icore��@ecore��ֵ
		*/
		for(int i=0;i<CoreNumber;i++)
		{
			Icore.at(i)=Vcore.at(i)/rcore.at(i)-ecore.at(i);
			ecore.at(i)=Vcore.at(i)/rcore.at(i)+Icore.at(i);
		}

		/*
		 * ����@eps��@Icore��ǰ�������жϵ������������
		 */
		eps=0;
		for(int i=0;i<CoreNumber;i++)
		{
			Vref.at(i)=std::fabs(Vref.at(i)-Vcore.at(i));//*(Vref.at(i)-Vcore.at(i));
			eps=std::max(Vref.at(i),eps);
		}

	}while(std::fabs(eps)>0.00000001);

	std::vector<double> Vstable=StableVcore(newPower,Rcore,Rbase);//ע�⣺���µ��ȶ���Ĺ�������
	char charVstable[]="Vstable";
	DisplayVector(Vstable,charVstable);

	Vout.close();
	Bout.close();
	Pout.close();
	return 0;

}
