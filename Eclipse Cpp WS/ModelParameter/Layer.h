/*
 * Layer.h
 *
 *  Created on: 2012-5-2
 *      Author: pc5
 */

#ifndef LAYER_H_
#define LAYER_H_

class Layer {
public:
	Layer();
	virtual ~Layer();
	double GetRC_Constant(){return Req*Ceq;};
protected:
	double g_ratio;
	double c_ratio;
	double m_ratio;
	double L_,W_,H_;
	double Area;
	double Req,Ceq;
};

#endif /* LAYER_H_ */
