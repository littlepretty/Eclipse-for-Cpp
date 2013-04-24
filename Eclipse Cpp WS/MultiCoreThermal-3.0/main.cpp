/*
 * main.cpp
 *
 * @brief 模拟多核芯片上，功耗（电流）导致的温度（电压）升高。
 * 是原版基础上+2.5版,结合而得3.0版
 * 原版特性：不仅给出最后的稳定迭代值，而且输出中间过程值————这导致输出过程繁琐，输出文件较大
 * 2.5特性：加入了理论上稳定值的计算，以及@Vbase与充电曲线的迭代值比较
 * 3.0新特性：功率密度成为温度的函数，即加上了每步迭代温度值所产生的漏电流功耗，详见Leakage.h；相应的Vbase、Vcore都有所提高；
 * 功耗的变化过程被写入了Pcore.txt中，并且已经是功耗，而不是功耗密度
 * 待扩充之处：每个核上的面积Acore目前是A/CoreNumber，可扩展为不同block的面积不同，需要改为一个面积向量Acore
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
	 * 声明核中的变量
	 */
	std::vector<double> Vcore;
	std::vector<double> Icore;
	std::vector<double> Rcore;
	std::vector<double> ecore;//核的热容中的电流源部分
	std::vector<double> rcore;//核的热容中的热阻部分
	std::vector<double> Ipoint;

	/*
	 * 声明基底中的变量
	 */
	double Vbase;
	double Ibase;
	double Rbase((r_conduct+r_fan)/A);//初始化基底的热阻，/A表示从密度换算
	double ebase(0);
	double rbase(h/(2*(c_conduct*A)));//初始化基底的热容中的热阻部分=h/2c，*A表示从密度换算

	/*
	 * 初始化基底中的@Vbase和@ebase
	 */
	for(int i=0;i<CoreNumber;i++)
	{
		//CorePower.push_back(SumPower/CoreNumber);//初始化核中的功率
		Icore.push_back(0);//初始化热容中的电流为0
		Rcore.push_back(r_bulk/Acore);//初始化核中的热阻，/Acore表示从密度换算
	}
	Vbase=0;//Isum*Rbase+Tamb;//初始化 @Vbase
	ebase=0;//Vbase/rbase;//初始化基底的热容中的电流源部分


	/*
	 * 初始化核中的@Vcore和@ecore
	 */
	for(int i=0;i<CoreNumber;i++)
	{
		//Vcore.push_back(CorePower.at(i)*Rcore.at(i)+Vbase);//初始化@Vcore
		Vcore.push_back(0);
		rcore.push_back(h/(2*(c_bulk*Acore)));//初始化@rcore=h/2c，*Acore表示从密度换算
		ecore.push_back(0);
		Ipoint.push_back(0);
	}

	/*
	 * 二级等效电路中，计算热阻和热容热阻部分的等效值
	 */
	std::vector<double> r;
	for(int i=0;i<CoreNumber;i++)
	{
		r.push_back(Rcore.at(i)+rcore.at(i));//计算等效值
	}
	double rtmp=r.at(0);//用来计算@Vbase的临时变量，是所有@rcore的并联值
	double Itmp(0);
	/*
	 * 计算Vbase时用到的热阻并联和
	 */
	for(int i=0;i<CoreNumber-1;i++)
	{
		rtmp=Pararlle(rtmp,r.at(i+1));//计算并联值
	}
	rtmp=Pararlle(rtmp,Pararlle(Rbase,rbase));//与@Rbase并联


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
		* 刷新@Vbase以及@ebase
		*/
		Itmp=0;//用来计算@Vbase的临时变量，是所有核内电流的和
		for(int j=0;j<CoreNumber;j++)
		{
			newPower.at(j)=CorePower.at(j)+LeakagePower(Vcore.at(j));//刷新实际的功耗值=源功耗+漏电流功耗
			Ipoint.at(j)=((newPower.at(j)+ecore.at(j))*rcore.at(j)/r.at(j));
			Itmp+=Ipoint.at(j);//计算@Itmp，需要每次刷新
		}

		Itmp+=ebase;

		//out<<std::setw(SetWidth)<<Itmp<<"\t";

		/*
		* 刷新@Vcore，@Vbase的值
		*/
		Vbase=rtmp*Itmp;

		double SumNewPower=0;//计算所有核的实际功耗总值，用以计算稳定Vcore以及充电曲线中的Vbase
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
		* 刷新@Ibase，@ebase的值
		*/
		Ibase=0;
		for(int i=0;i<CoreNumber;i++)
		{
			Ibase+=(Vcore.at(i)-Vbase)/Rcore.at(i);
		}
		Ibase+=-Vbase/Rbase;
		ebase=Vbase/rbase+Ibase;//计算@ebase，需要每次刷新

		/*
		* 刷新@Icore，@ecore的值
		*/
		for(int i=0;i<CoreNumber;i++)
		{
			Icore.at(i)=Vcore.at(i)/rcore.at(i)-ecore.at(i);
			ecore.at(i)=Vcore.at(i)/rcore.at(i)+Icore.at(i);
		}

		/*
		 * 根据@eps（@Icore的前后迭代差）判断迭代的收敛情况
		 */
		eps=0;
		for(int i=0;i<CoreNumber;i++)
		{
			Vref.at(i)=std::fabs(Vref.at(i)-Vcore.at(i));//*(Vref.at(i)-Vcore.at(i));
			eps=std::max(Vref.at(i),eps);
		}

	}while(std::fabs(eps)>0.00000001);

	std::vector<double> Vstable=StableVcore(newPower,Rcore,Rbase);//注意：是新的稳定后的功耗向量
	char charVstable[]="Vstable";
	DisplayVector(Vstable,charVstable);

	Vout.close();
	Bout.close();
	Pout.close();
	return 0;

}
