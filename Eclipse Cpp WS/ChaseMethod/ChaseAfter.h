/*
 * ChaseAfter.h
 *
 *  Created on: 2012-4-27
 *      Author: pc5
 */

#ifndef CHASEAFTER_H_
#define CHASEAFTER_H_
#include "DiagMatrix.h"
/*
 * 追赶法解三对角方程DM*x=f，函数对象
 */
class ChaseAfter {
public:
	ChaseAfter();
	virtual ~ChaseAfter();
	void operator ()();
private:
	DiagMatrix DM;
	double *f,*x;
};

#endif /* CHASEAFTER_H_ */
