/*
 * DieLayer.cpp
 *
 *  Created on: 2012-5-2
 *      Author: pc5
 */

#include "DieLayer.h"

DieLayer::DieLayer()
{
	g_ratio=148.2;   //             W/(mK)
	c_ratio=1.75e6; 		//			J/£¨m3*K£©
	m_ratio=2.32e3;		//   		kg/m3
	L_=0.01;     //                 m=10mm
	W_=0.01;
	H_=0.00015;
	Area=L_*W_;
	Req=H_/g_ratio/Area;
	Ceq=c_ratio*L_*W_*H_;
}

DieLayer::~DieLayer() {
	// TODO Auto-generated destructor stub
}
