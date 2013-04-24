/*
 * main.cpp
 *
 *  Created on: 2012-3-25
 *      Author: pc5
 */

/** @file
 *  @brief run the eightqueen problem
 *  @author yjq
 *  @date 2012-3-25
 */
#include "EightQueen.h"
#include <iostream>
int main()
{
	EightQueen *eq=new EightQueen(8,0);
	eq->SafePut(0);

	//char ch;
	//std::cin>>ch;
	return 0;
}
