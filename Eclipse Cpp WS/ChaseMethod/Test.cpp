/*
 * Test.cpp
 *
 *  Created on: 2012-4-27
 *      Author: pc5
 */
#include "ChaseAfter.h"
#include "DMConjGrad.h"
int main()
{
	ChaseAfter *ca=new ChaseAfter();
	(*ca)();

	DMConjGrad *dmcg=new DMConjGrad();
	dmcg->ReadInMatrix();
	(*dmcg)();
	dmcg->GiveResult();
	return 0;
}
