/*
 * Test.cpp
 *
 *  Created on: 2012-5-2
 *      Author: pc5
 */

#include "InequlityLinearProblem.h"

int main()
{
	InequlityLinearProblem ilp(4,4);
	ilp.ReadInfo();
	ilp.Solution();
	ilp.Output();
}
