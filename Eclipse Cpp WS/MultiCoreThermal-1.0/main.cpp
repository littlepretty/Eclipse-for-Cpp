/*
 * main.cpp
 *
 * @brief ģ����оƬ�ϣ����ģ����������µ��¶ȣ���ѹ�����ߡ�
 * 1.0�����ԣ�ֻ���������ȶ�����ֵ����������м����ֵ
 * @date Created on: 2012-4-9
 * @author pc5
 *
 */
#include <algorithm>
#include <cmath>
#include "Data.h"
#include <iomanip>
#include <fstream>

int main()
{
	char powerfile[]="Power.txt";
	//char distrfile[]="Distribution.txt";
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


	//begin loop!
	std::vector<double> Vref;
	for(int i=0;i<CoreNumber;i++)
	{
		Vref.push_back(Vcore.at(i));
	}
	double eps(1);
	do
	{
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

		//if(--IterNumber<0)
		//	break;
	}while(std::fabs(eps)>0.00000001);


	/*
	 * ��������д���ļ�
	 */
	char Vname[]="Vcore";
	DisplayVector(Vcore,Vname);

	std::ofstream Bout("Base.txt");
	if(not Bout)
		std::perror("Base.txt");
	Bout.precision(SetPrecision);
	Bout<<std::setw(SetWidth)<<"VBase: "<<Vbase<<"\n";
	Bout<<std::setw(SetWidth)<<"Ibase: "<<Ibase<<"\n";
	Bout<<std::setw(SetWidth)<<"eBase: "<<ebase<<"\n";
	Bout<<std::setw(SetWidth)<<std::right<<"eps: "<<eps<<"\n";
	Bout.close();

	return 0;
}
