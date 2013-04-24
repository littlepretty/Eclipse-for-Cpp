/*
 * main.cpp
 *
 * @brief 模拟多核芯片上，功耗（电流）导致的温度（电压）升高。
 * 是原版基础上的2.5版
 * 原版特性：不仅给出最后的稳定迭代值，而且输出中间过程值————这导致输出过程繁琐，输出文件较大
 * 2.5特性：加入了理论上稳定值的计算，以及@Vbase与充电曲线的迭代值比较
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
		 * 输出显示
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
		compareVbase=ChargeLine(SumPower,Rbase,c_conduct,IterNumber++);
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
