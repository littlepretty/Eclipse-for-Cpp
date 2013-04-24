/*
 * TIM.cpp
 *
 *  Created on: 2012-5-2
 *      Author: pc5
 */

#include "TIM.h"

TIM::TIM()
{
	g_ratio=4;   //             W/(mK)
	c_ratio=4.0e6; 		//			J/£¨kg*K£©
	//m_ratio=2.32e3;		//   		kg/m3
	L_=0.01;     //                 m=10mm
	W_=0.01;
	H_=20.0e-6;
	Area=L_*W_;
	Req=H_/g_ratio/Area;
	Ceq=c_ratio*L_*W_*H_;
}

TIM::~TIM() {
	// TODO Auto-generated destructor stub
}
