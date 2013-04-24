/*
 * HeatSpreader.cpp
 *
 *  Created on: 2012-5-2
 *      Author: pc5
 */

#include "HeatSpreader.h"

HeatSpreader::HeatSpreader() {
	g_ratio=400;   //             W/(mK)
	c_ratio=3.55e6; 		//			J/£¨kg*K£©
	m_ratio=8.92e3;		//   		kg/m3
	L_=0.03;     //                 m=10mm
	W_=0.03;
	H_=0.001;
	Area=L_*W_;
	Req=H_/(g_ratio*Area);
	Ceq=c_ratio*L_*W_*H_;
}

HeatSpreader::~HeatSpreader() {
	// TODO Auto-generated destructor stub
}
