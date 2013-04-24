/*
 * @mainpage main.cpp
 *
 * @brief 模拟多核芯片上，功耗（电流）导致的温度（电压）升高。
 * 是原版基础上+2.5版+3.0版,结合而得3.1版
 * 原版特性：不仅给出最后的稳定迭代值，而且输出中间过程值————这导致输出过程繁琐，输出文件较大
 * 2.5特性：加入了理论上稳定值的计算，以及@Vbase与充电曲线的迭代值比较
 * 3.0特性：功率密度成为温度的函数，即加上了每步迭代温度值所产生的漏电流功耗，详见Leakage.h；相应的Vbase、Vcore都有所提高；
 * 功耗的变化过程被写入了Pcore.txt中，并且已经是功耗，而不是功耗密度
 * 3.1版新特性：每个核上的面积Acore一个面积向量;程序从Power.txt中读入core的单位面积Aper，总面积A=Aper*CoreNumber
 * 然后根据之后的比例分配这个总面积（这么做主要是为了不再改面积）
 *
 * 引用关系：
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
	double Vbase(Tamb);
	double Ibase;
	double Rbase((r_conduct+r_fan)/A);//初始化基底的热阻，/@param A表示从密度换算
	double ebase(0);
	double rbase(h/(2*(c_conduct*A)));//初始化基底的热容中的热阻部分=@param h/2@param c，*@param A表示从密度换算

	/*
	 * 初始化基底中的@param Vbase和@param ebase
	 */
	for(int i=0;i<CoreNumber;i++)
	{
		Icore.push_back(0);//初始化热容中的电流为0
		Rcore.push_back(r_bulk/Acore.at(i));//初始化核中的热阻，/@param Acore表示从密度换算
	}

	/*
	 * 初始化核中的@param Vcore和@param ecore
	 */
	for(int i=0;i<CoreNumber;i++)
	{
		Vcore.push_back(Tamb);
		rcore.push_back(h/(2*(c_bulk*Acore.at(i))));//初始化@param rcore=@param h/2@param c，*@param Acore表示从密度换算
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
	double rtmp=r.at(0);//用来计算@param Vbase的临时变量，是所有@param rcore的并联值
	double Itmp(0);
	/*
	 * 计算@param Vbase时用到的热阻并联和
	 */
	for(int i=0;i<CoreNumber-1;i++)
	{
		rtmp=Pararlle(rtmp,r.at(i+1));//计算并联值
	}
	rtmp=Pararlle(rtmp,Pararlle(Rbase,rbase));//与@param Rbase并联


	/*
	 * @brief 打开输出文件，并相应的打印表头
	 * 主要的输出内容：@param Vore、@param Pcore、@param Vbase、@param compareVbase
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

	Vout<<std::setw(SetWidth)<<std::right<<"RC="<<r_bulk*c_bulk<<"\n";
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
			Vout<<std::setw(SetWidth)<<*i<<"\t";
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
		* 刷新@param Vbase以及@param ebase
		*/
		Itmp=0;//用来计算@param Vbase的临时变量，是所有核内电流的和
		for(int j=0;j<CoreNumber;j++)
		{
			newPower.at(j)=CorePower.at(j)+LeakagePower(Vcore.at(j));//刷新实际的功耗值(密度)=源功耗+漏电流功耗
			Ipoint.at(j)=((newPower.at(j)+ecore.at(j))*rcore.at(j)/r.at(j));
			Itmp+=Ipoint.at(j);//计算@param Itmp，需要每次刷新
		}

		Itmp+=ebase;

		/*
		* 刷新@param Vcore，@param Vbase的值
		*/
		Vbase=rtmp*Itmp;

		double SumNewPower=0;//计算所有核的实际功耗总值，用以计算稳定@param Vcore以及充电曲线中的@param Vbase
		for(int i=0;i<CoreNumber;i++)
		{
			SumNewPower+=newPower.at(i);
		}
		compareVbase=ChargeLine(SumNewPower,Rbase,c_conduct*A,IterNumber++);//注意：@param Cbase=@param c_conduct*@param A
		for(int i=0;i<CoreNumber;i++)
		{
			Vcore.at(i)=Vbase+(Ipoint.at(i)-Vbase/r.at(i))*Rcore.at(i);
		}

		/*
		* 刷新@param Ibase，@param ebase的值
		*/
		Ibase=0;
		for(int i=0;i<CoreNumber;i++)
		{
			Ibase+=(Vcore.at(i)-Vbase)/Rcore.at(i);
		}
		Ibase+=-Vbase/Rbase;
		ebase=Vbase/rbase+Ibase;//计算@param ebase，需要每次刷新

		/*
		* 刷新@param Icore，@param ecore的值
		*/
		for(int i=0;i<CoreNumber;i++)
		{
			Icore.at(i)=Vcore.at(i)/rcore.at(i)-ecore.at(i);
			ecore.at(i)=Vcore.at(i)/rcore.at(i)+Icore.at(i);
		}

		/*
		 * 根据@param eps（@param Icore的前后迭代差）判断迭代的收敛情况
		 */
		eps=0;
		for(int i=0;i<CoreNumber;i++)
		{
			Vref.at(i)=std::fabs(Vref.at(i)-Vcore.at(i));
			eps=std::max(Vref.at(i),eps);
		}

	}while(std::fabs(eps)>0.000001);

	std::vector<double> Vstable=StableVcore(newPower,Rcore,Rbase);//@param Vstable看作是理论稳定温度 注意：计算中用的是新的稳定后的功耗向量
	char charVstable[]="Vstable";
	DisplayVector(Vstable,charVstable);//输出稳定温度值用来对比而已（打开Vstable.txt）

	Vout.close();
	Bout.close();
	Pout.close();
	return 0;

}
