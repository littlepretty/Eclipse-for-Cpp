/*
 * main.cpp
 *
 * @brief ģ����оƬ�ϣ����ģ����������µ��¶ȣ���ѹ�����ߡ�
 * ��ԭ������ϵ�2.5��
 * ԭ�����ԣ��������������ȶ�����ֵ����������м����ֵ���������⵼��������̷���������ļ��ϴ�
 * 2.5���ԣ��������������ȶ�ֵ�ļ��㣬�Լ�@Vbase�������ߵĵ���ֵ�Ƚ�
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
#include "Data.h"
#include <cmath>
#include <cstdio>
#include <fstream>

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
	double Rbase(r_conduct+r_fan);//��ʼ�����׵�����
	double ebase(0);
	double rbase(h/(2*c_conduct));//��ʼ�����׵������е����貿��=h/2c

	/*
	 * ��ʼ�������е�@Vbase��@ebase
	 */
	for(int i=0;i<CoreNumber;i++)
	{
		//CorePower.push_back(SumPower/CoreNumber);//��ʼ�����еĹ���
		Icore.push_back(0);//��ʼ�������еĵ���Ϊ0
		Rcore.push_back(r_bulk);//��ʼ�����е�����
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
		rcore.push_back(h/(2*c_bulk));//��ʼ��@rcore=h/2c
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
	//std::ofstream Iout("Icore.txt");
	//std::ofstream eout("ecore.txt");
	//std::ofstream Rout("Residual.txt");
	std::ofstream Bout("Base.txt");
	if(not Vout)
		std::perror("Vcore.txt");
	//if(not Iout)
	//	std::perror("Icore.txt");
	//if(not eout)
	//	std::perror("ecore.txt");
	//if(not Rout)
	//	std::perror("Residual.txt");
	if(not Bout)
		std::perror("Residual.txt");


	Vout.precision(SetPrecision);
	//Iout.precision(SetPrecision);
	//eout.precision(SetPrecision);
	//Rout.precision(SetPrecision);
	Bout.precision(SetPrecision);

	for(int i=0;i<CoreNumber;i++)
		Vout<<std::setw(SetWidth)<<std::right<<"Pcore("<<i+1<<")";
	Vout<<"\n";
	for(std::vector<double>::iterator i=CorePower.begin();i!=CorePower.end();i++)
		Vout<<std::setw(SetWidth)<<*i;
	Vout<<"\n";

	for(int i=0;i<CoreNumber;i++)
		Vout<<std::setw(SetWidth)<<std::right<<"Vcore("<<i+1<<")";
	Vout<<std::endl;

	/*
	for(int i=0;i<CoreNumber;i++)
		Iout<<std::setw(SetWidth)<<"Icore("<<i+1<<")";
	Iout<<std::endl;
	for(int i=0;i<CoreNumber;i++)
		eout<<std::setw(SetWidth)<<"ecore("<<i+1<<")";
	eout<<std::endl;
	*/
	//out<<std::setw(SetWidth)<<"rpoint";


	Bout<<std::setw(SetWidth)<<"VBase";
	Bout<<std::setw(SetWidth)<<"Charge"<<"\n";
	//Bout<<std::setw(SetWidth)<<std::fixed<<"Ibase";
	//Bout<<std::setw(SetWidth)<<"eBase"<<"\n";

	//Rout<<std::setw(SetWidth)<<std::right<<"eps";
	//Rout<<"\n";

	std::vector<double> Vref;
	for(int i=0;i<CoreNumber;i++)
	{
		Vref.push_back(Vcore.at(i));
	}
	double eps(1);
	double compareVbase(0);
	do
	{

		/*
		 * �����ʾ
		 */

		for(std::vector<double>::iterator i=Vcore.begin();i!=Vcore.end();i++)
			Vout<<std::setw(SetWidth)<<*i;
		Vout<<std::endl;

		/*
		for(std::vector<double>::iterator i=Icore.begin();i!=Icore.end();i++)
			Iout<<std::setw(SetWidth)<<*i;
		Iout<<std::endl;

		for(std::vector<double>::iterator i=ecore.begin();i!=ecore.end();i++)
			eout<<std::setw(SetWidth)<<*i;
		eout<<std::endl;
		 */

		Bout<<std::setw(SetWidth)<<Vbase;
		Bout<<std::setw(SetWidth)<<compareVbase<<"\n";

		//Bout<<std::setw(SetWidth)<<Ibase;
		//Bout<<std::setw(SetWidth)<<ebase<<std::endl;

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
			Ipoint.at(j)=((CorePower.at(j)+ecore.at(j))*rcore.at(j)/r.at(j));
			Itmp+=Ipoint.at(j);//����@Itmp����Ҫÿ��ˢ��
		}

		Itmp+=ebase;

		//out<<std::setw(SetWidth)<<Itmp<<"\t";

		/*
		* ˢ��@Vcore��@Vbase��ֵ
		*/
		Vbase=rtmp*Itmp;
		compareVbase=ChargeLine(SumPower,Rbase,c_conduct,IterNumber++);
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

		//Rout<<std::setw(SetWidth)<<eps<<std::endl;
		//for(std::vector<double>::iterator i=Vref.begin();i!=Vref.end();i++)
		//	std::cout<<std::setw(SetWidth)<<*i;

		//if(--IterNumber<0)
		//	break;
	}while(std::fabs(eps)>0.00000001);

	std::vector<double> Vstable(StableVcore(CorePower,Rcore,Rbase));
	char charVstable[]="Vstable";
	DisplayVector(Vstable,charVstable);

	Vout.close();
	//Iout.close();
	//eout.close();
	Bout.close();

	return 0;

}
