/*
 * Test.cpp
 *
 *  Created on: 2012-5-3
 *      Author: pc5
 */
#include "ComplexTuning.h"
#include "iostream"
int main()
{
	ComplexTuning ct(2,2);
	ct.ReadInfo();
	ct.Solution();
	ct.Output();


	/*
	for(int i=0;i<10;i++)
	{
		std::cout<<ct.GenRand()<<" ";
	}
	*/

}
