/*
 * HeatSink.cpp
 *
 *  Created on: 2012-5-2
 *      Author: pc5
 */

#include "HeatSink.h"

HeatSink::HeatSink() {
	g_ratio=400;   //             W/(mK)
	c_ratio=3.55e6; 		//			J/£¨m3*K£©
	m_ratio=8.92e3;		//   		kg/m3
	L_=0.04;              //       m=10mm
	W_=0.04;
	H_=0.0069;
	Area=L_*W_;
	Req=H_/(g_ratio*Area);
	Ceq=c_ratio*L_*W_*H_;
}

HeatSink::~HeatSink() {
	// TODO Auto-generated destructor stub
}
