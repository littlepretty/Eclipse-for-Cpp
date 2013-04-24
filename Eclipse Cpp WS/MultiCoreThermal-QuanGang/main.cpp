/*
 * main.cpp
 *
 * @brief
 * @date Created on: 2012-4-9
 * @author pc5
 *
 */
#include <iostream>
#include <algorithm>
#include <cmath>
#include "Data.h"
#include "Leakage.h"
#include <iomanip>
#include <fstream>

int main()
{
	char powerfile[]="Power.txt";
	GetPower(powerfile);

	/*
	 * ���������еı���
	 */
	double Vcore(0);//��ʼ�� @Vcore
	double Icore(0);
	double Rcore(2.25*r_bulk);//��ʼ�����׵�����
	double ecore(0);//��ʼ�����׵������еĵ���Դ����
	double rcore(h/(2*c_bulk));//��ʼ�����׵������е����貿��=h/2c

	double rtmp=Pararlle(Rcore,rcore);//����Vcoreʱ�õ������貢����
	double Itmp(0);


	double ref;
	double eps(1);
	//double compareVcore;

	std::ofstream out("Trace.txt");
	if(not out)
		std::perror("Trace.txt failed!");
	out<<std::setw(SetWidth)<<std::setprecision(SetPrecision)<<"RC(core)="<<c_bulk<<"\n";

	do//begin loop!
	{
		ref=Vcore;//Vcore;
		/*
		* ˢ��@Vcore�Լ�@ecore
		*/
		Itmp=SumPower+ecore+LeakagePower(Vcore);//����@Itmp����Ҫÿ��ˢ��

		Vcore=rtmp*Itmp;//ˢ��@Vcore��@Vcore��ֵ
		//compareVcore=ChargeLine(SumPower,Rcore,c_conduct,IterNumber++);

		Icore=SumPower+LeakagePower(Vcore)-Vcore/Rcore;//ˢ��@Icore��@ecore��ֵ

		ecore=Vcore/rcore+Icore;//����@ecore����Ҫÿ��ˢ��


		out<<std::setw(SetWidth)<<std::setprecision(SetPrecision)<<Vcore<<"\n";
		//out<<std::setw(SetWidth)<<std::setprecision(SetPrecision)<<Icore<<"\t";
		//out<<std::setw(SetWidth)<<std::setprecision(SetPrecision)<<ecore<<"\n";
		//out<<std::setw(SetWidth)<<std::setprecision(SetPrecision)<<compareVcore<<"\n";

		eps=std::fabs(ref-Vcore);//����@eps��@Vcore��ǰ�������жϵ������������
		--ForceStop;
		if(ForceStop<0)
			break;

	}while(eps>0.000000001);

	out.close();
	return 0;
}
