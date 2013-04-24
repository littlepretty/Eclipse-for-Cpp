/*
 * @mainpage main.cpp
 *
 * @brief �����Աȵļ�ģ�ͣ����ϵ����ݶ�ʡ�ԣ����Բ�����Icore��ecore��rcore
 *
 * ���ù�ϵ��
 * @link Leakage.h
 * @link Data.h
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
	std::vector<double> Rcore;
	//std::vector<double> ecore;//�˵������еĵ���Դ����
	//std::vector<double> rcore;//�˵������е����貿��
	//std::vector<double> Ipoint;

	/*
	 * ���������еı���
	 */
	double Vbase(Tamb);
	double Ibase;
	double Rbase((r_conduct+r_fan)/A);//��ʼ�����׵����裬/@param A��ʾ���ܶȻ���
	double ebase(0);
	double rbase(h/(2*(c_conduct*A)));//��ʼ�����׵������е����貿��=@param h/2@param c��*@param A��ʾ���ܶȻ���

	/*
	 * ��ʼ�������е�@param Vbase��@param ebase
	 */
	for(int i=0;i<CoreNumber;i++)
	{
		Rcore.push_back(r_bulk/Acore.at(i));//��ʼ�����е����裬/@param Acore��ʾ���ܶȻ���
	}

	/*
	 * ��ʼ�����е�@param Vcore��@param ecore
	 */
	for(int i=0;i<CoreNumber;i++)
	{
		Vcore.push_back(Tamb);
		//rcore.push_back(h/(2*(c_bulk*Acore.at(i))));//��ʼ��@param rcore=@param h/2@param c��*@param Acore��ʾ���ܶȻ���
		//ecore.push_back(0);
	}

	/*
	 * ������Ч��·�У�����������������貿�ֵĵ�Чֵ
	 */
	std::vector<double> r;
	for(int i=0;i<CoreNumber;i++)
	{
		r.push_back(Rcore.at(i));//�����Чֵ
	}
	double rtmp=r.at(0);//��������@param Vbase����ʱ������������@param rcore�Ĳ���ֵ
	double Itmp(0);
	/*
	 * ����@param Vbaseʱ�õ������貢����
	 */
	for(int i=0;i<CoreNumber-1;i++)
	{
		rtmp=Pararlle(rtmp,r.at(i+1));//���㲢��ֵ
	}
	rtmp=Pararlle(rtmp,Pararlle(Rbase,rbase));//��@param Rbase����


	/*
	 * @brief ������ļ�������Ӧ�Ĵ�ӡ��ͷ
	 * ��Ҫ��������ݣ�@param Vore��@param Pcore��@param Vbase��@param compareVbase
	 */
	std::ofstream Vout("Vcore.txt");
	std::ofstream Pout("Pcore.txt");
	std::ofstream Bout("Base.txt");
	if(not Vout)
		std::perror("Vcore.txt");
	if(not Bout)
		std::perror("Pcore.txt");
	if(not Pout)
		std::perror("Base.txt");

	Vout.precision(SetPrecision);
	Bout.precision(SetPrecision);
	Pout.precision(SetPrecision);

	for(int i=0;i<CoreNumber;i++)
		Pout<<std::setw(SetWidth)<<std::right<<"Pcore("<<i+1<<")";
	Pout<<"\n";

	for(int i=0;i<CoreNumber;i++)
		Vout<<std::setw(SetWidth)<<std::right<<"Vcore("<<i+1<<")";
	Vout<<std::endl;

	Bout<<std::setw(SetWidth)<<"R(bc)Cb="<<Pararlle(Rbase,Rcore[0])*c_conduct<<"\n";
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
			Vout<<std::setw(SetWidth)<<*i+amb<<"\t";
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
		* ˢ��@param Vbase�Լ�@param ebase
		*/
		Itmp=0;//��������@param Vbase����ʱ�����������к��ڵ����ĺ�
		for(int j=0;j<CoreNumber;j++)
		{
			newPower.at(j)=CorePower.at(j)+LeakagePower(Vcore.at(j));//ˢ��ʵ�ʵĹ���ֵ(�ܶ�)=Դ����+©��������
			//Ipoint.at(j)=(newPower.at(j)*rcore.at(j)/r.at(j));
			Itmp+=newPower.at(j);//����@param Itmp����Ҫÿ��ˢ��
		}

		Itmp+=ebase;

		/*
		* ˢ��@param Vcore��@param Vbase��ֵ
		*/
		Vbase=rtmp*Itmp;

		double SumNewPower=0;//�������к˵�ʵ�ʹ�����ֵ�����Լ����ȶ�@param Vcore�Լ���������е�@param Vbase
		for(int i=0;i<CoreNumber;i++)
		{
			SumNewPower+=newPower.at(i);
		}
		compareVbase=ChargeLine(SumNewPower,Rbase,c_conduct*A,IterNumber++);//ע�⣺@param Cbase=@param c_conduct*@param A
		for(int i=0;i<CoreNumber;i++)
		{
			Vcore.at(i)=Vbase+newPower.at(i)*Rcore.at(i);
		}

		/*
		* ˢ��@param Ibase��@param ebase��ֵ
		*/
		Ibase=0;
		for(int i=0;i<CoreNumber;i++)
		{
			Ibase+=(Vcore.at(i)-Vbase)/Rcore.at(i);
		}
		Ibase+=-Vbase/Rbase;
		ebase=Vbase/rbase+Ibase;//����@param ebase����Ҫÿ��ˢ��

		/*
		 * ����@param eps��@param Icore��ǰ�������жϵ������������
		 */
		eps=0;
		for(int i=0;i<CoreNumber;i++)
		{
			Vref.at(i)=std::fabs(Vref.at(i)-Vcore.at(i));
			eps=std::max(Vref.at(i),eps);
		}

	}while(std::fabs(eps)>0.000001);

	std::vector<double> Vstable=StableVcore(newPower,Rcore,Rbase);//@param Vstable�����������ȶ��¶� ע�⣺�������õ����µ��ȶ���Ĺ�������
	char charVstable[]="Vstable";
	DisplayVector(Vstable,charVstable);//����ȶ��¶�ֵ�����Աȶ��ѣ���Vstable.txt��

	Vout.close();
	Bout.close();
	Pout.close();
	return 0;

}
