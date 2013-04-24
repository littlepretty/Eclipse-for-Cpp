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
	 * 声明单核中的变量
	 */
	double Vcore(0);//初始化 @Vcore
	double Icore(0);
	double Rcore(2.25*r_bulk);//初始化基底的热阻
	double ecore(0);//初始化基底的热容中的电流源部分
	double rcore(h/(2*c_bulk));//初始化基底的热容中的热阻部分=h/2c

	double rtmp=Pararlle(Rcore,rcore);//计算Vcore时用到的热阻并联和
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
		* 刷新@Vcore以及@ecore
		*/
		Itmp=SumPower+ecore+LeakagePower(Vcore);//计算@Itmp，需要每次刷新

		Vcore=rtmp*Itmp;//刷新@Vcore，@Vcore的值
		//compareVcore=ChargeLine(SumPower,Rcore,c_conduct,IterNumber++);

		Icore=SumPower+LeakagePower(Vcore)-Vcore/Rcore;//刷新@Icore，@ecore的值

		ecore=Vcore/rcore+Icore;//计算@ecore，需要每次刷新


		out<<std::setw(SetWidth)<<std::setprecision(SetPrecision)<<Vcore<<"\n";
		//out<<std::setw(SetWidth)<<std::setprecision(SetPrecision)<<Icore<<"\t";
		//out<<std::setw(SetWidth)<<std::setprecision(SetPrecision)<<ecore<<"\n";
		//out<<std::setw(SetWidth)<<std::setprecision(SetPrecision)<<compareVcore<<"\n";

		eps=std::fabs(ref-Vcore);//根据@eps（@Vcore的前后迭代差）判断迭代的收敛情况
		--ForceStop;
		if(ForceStop<0)
			break;

	}while(eps>0.000000001);

	out.close();
	return 0;
}
