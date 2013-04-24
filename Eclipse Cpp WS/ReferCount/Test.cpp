/*
 * Test.cpp
 *
 *  Created on: 2012-4-4
 *      Author: yjq
 */

#include "RCIPtr.h"
#include "RCObject.h"
#include "RCWidget.h"
#include "Widget.h"
#include <iostream>
int main()
{

	Widget w(54);
	int oldSecret=w.showThat();
	std::cout<<oldSecret<<std::endl;
	w.doThis();
	int newSecret=w.showThat();
	std::cout<<newSecret<<std::endl;


	/*
	RCWidget rcw(54);
	oldSecret=rcw.showThat();
	std::cout<<oldSecret<<std::endl;
	rcw.doThis();
	newSecret=rcw.showThat();
	std::cout<<newSecret<<std::endl;
	*/
	return 0;
}
