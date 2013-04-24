/*
 * Example.cpp
 *
 *  Created on: 2012-5-2
 *      Author: pc5
 */
#include <iostream>
#include "Layer.h"
#include "DieLayer.h"
#include "TIM.h"
#include "HeatSpreader.h"
#include "HeatSink.h"
int main()
{
	DieLayer dl;
	TIM tim;
	HeatSpreader hsr;
	HeatSink hsk;
	double* RC=new double[4];
	RC[0]=dl.GetRC_Constant();
	std::cout<<"Die: "<<RC[0]<<"\n";
	RC[1]=tim.GetRC_Constant();
	std::cout<<"TIM: "<<RC[1]<<"\n";
	RC[2]=hsr.GetRC_Constant();
	std::cout<<"HeatSpreader: "<<RC[2]<<"\n";
	RC[3]=hsk.GetRC_Constant();
	std::cout<<"HeatSink: "<<RC[3]<<"\n";

	return 0;
}
