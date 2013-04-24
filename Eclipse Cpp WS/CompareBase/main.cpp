/*
 * main.cpp
 *
 * @brief 对比等效计算的Vbase和充电曲线之间的误差
 * 忽略所有核上的热容、热阻，功耗直接加到基地上
 * 计算中，热容热阻都采用单位面积值，并修正了其他三版中Data.h中的热容热阻（c_bulk&c_conduct）值为单位面积的热容热阻值
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
	 * 声明基底中的变量
	 */
	double Vbase(0);//初始化 @Vbase
	double Ibase(0);
	double Rbase(r_conduct+r_fan);//初始化基底的热阻
	double ebase(0);//初始化基底的热容中的电流源部分
	double rbase(h/(2*c_conduct));//初始化基底的热容中的热阻部分=h/2c

	double rtmp=Pararlle(Rbase,rbase);//计算Vbase时用到的热阻并联和
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
		* 刷新@Vbase以及@ebase
		*/

		Itmp=SumPower+ebase;//计算@Itmp，需要每次刷新

		Vbase=rtmp*Itmp;//刷新@Vcore，@Vbase的值
		compareVbase=ChargeLine(SumPower,Rbase,c_conduct,IterNumber++);

		Ibase=SumPower-Vbase/Rbase;//刷新@Ibase，@ebase的值

		ebase=Vbase/rbase+Ibase;//计算@ebase，需要每次刷新


		out<<std::setw(SetWidth)<<std::setprecision(SetPrecision)<<Vbase<<"\t";
		//out<<std::setw(SetWidth)<<std::setprecision(SetPrecision)<<Ibase<<"\t";
		//out<<std::setw(SetWidth)<<std::setprecision(SetPrecision)<<ebase<<"\t";
		out<<std::setw(SetWidth)<<std::setprecision(SetPrecision)<<compareVbase<<"\n";

		eps=std::fabs(Vref-Vbase);//根据@eps（@Vbase的前后迭代差）判断迭代的收敛情况
		--ForceStop;

	}while(eps>0.0000005);

	out.close();
	return 0;
}
