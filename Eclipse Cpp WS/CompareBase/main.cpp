/*
 * main.cpp
 *
 * @brief �Աȵ�Ч�����Vbase�ͳ������֮������
 * �������к��ϵ����ݡ����裬����ֱ�Ӽӵ�������
 * �����У��������趼���õ�λ���ֵ��������������������Data.h�е��������裨c_bulk&c_conduct��ֵΪ��λ�������������ֵ
 * @date Created on: 2012-4-9
 * @author pc5
 *
 */
#include <iostream>
#include <algorithm>
#include <cmath>
#include "Data.h"
#include <iomanip>
#include <fstream>

int main()
{
	char powerfile[]="Power.txt";
	GetPower(powerfile);

	/*
	 * ���������еı���
	 */
	double Vbase(0);//��ʼ�� @Vbase
	double Ibase(0);
	double Rbase(r_conduct+r_fan);//��ʼ�����׵�����
	double ebase(0);//��ʼ�����׵������еĵ���Դ����
	double rbase(h/(2*c_conduct));//��ʼ�����׵������е����貿��=h/2c

	double rtmp=Pararlle(Rbase,rbase);//����Vbaseʱ�õ������貢����
	double Itmp(0);


	double Vref;
	double eps(1);
	double compareVbase;

	std::ofstream out("Trace.txt");
	if(not out)
		std::perror("Trace.txt failed!");

	do//begin loop!
	{
		Vref=Vbase;
		/*
		* ˢ��@Vbase�Լ�@ebase
		*/

		Itmp=SumPower+ebase;//����@Itmp����Ҫÿ��ˢ��

		Vbase=rtmp*Itmp;//ˢ��@Vcore��@Vbase��ֵ
		compareVbase=ChargeLine(SumPower,Rbase,c_conduct,IterNumber++);

		Ibase=SumPower-Vbase/Rbase;//ˢ��@Ibase��@ebase��ֵ

		ebase=Vbase/rbase+Ibase;//����@ebase����Ҫÿ��ˢ��


		out<<std::setw(SetWidth)<<std::setprecision(SetPrecision)<<Vbase<<"\t";
		//out<<std::setw(SetWidth)<<std::setprecision(SetPrecision)<<Ibase<<"\t";
		//out<<std::setw(SetWidth)<<std::setprecision(SetPrecision)<<ebase<<"\t";
		out<<std::setw(SetWidth)<<std::setprecision(SetPrecision)<<compareVbase<<"\n";

		eps=std::fabs(Vref-Vbase);//����@eps��@Vbase��ǰ�������жϵ������������
		--ForceStop;

	}while(eps>0.0000005);

	out.close();
	return 0;
}
