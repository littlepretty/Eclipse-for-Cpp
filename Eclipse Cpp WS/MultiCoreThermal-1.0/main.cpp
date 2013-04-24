/*
 * main.cpp
 *
 * @brief 模拟多核芯片上，功耗（电流）导致的温度（电压）升高。
 * 1.0新特性：只给出最后的稳定迭代值，而不输出中间过程值
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
	double Rbase(r_conduct+r_fan);//初始化基底的热阻
	double ebase(0);
	double rbase(h/(2*c_conduct));//初始化基底的热容中的热阻部分=h/2c

	/*
	 * 初始化基底中的@Vbase和@ebase
	 */
	for(int i=0;i<CoreNumber;i++)
	{
		//CorePower.push_back(SumPower/CoreNumber);//初始化核中的功率
		Icore.push_back(0);//初始化热容中的电流为0
		Rcore.push_back(r_bulk);//初始化核中的热阻
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
		rcore.push_back(h/(2*c_bulk));//初始化@rcore=h/2c
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
		* 刷新@Vbase以及@ebase
		*/
		Itmp=0;//用来计算@Vbase的临时变量，是所有核内电流的和
		for(int j=0;j<CoreNumber;j++)
		{
			Ipoint.at(j)=((CorePower.at(j)+ecore.at(j))*rcore.at(j)/r.at(j));
			Itmp+=Ipoint.at(j);//计算@Itmp，需要每次刷新
		}

		Itmp+=ebase;

		//out<<std::setw(SetWidth)<<Itmp<<"\t";

		/*
		* 刷新@Vcore，@Vbase的值
		*/
		Vbase=rtmp*Itmp;
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

		//if(--IterNumber<0)
		//	break;
	}while(std::fabs(eps)>0.00000001);


	/*
	 * 将计算结果写入文件
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
