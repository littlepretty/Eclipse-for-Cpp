/*
 * RCWidget.h
 *
 *  Created on: 2012-4-4
 *      Author: yjq
 */

#ifndef RCWIDGET_H_
#define RCWIDGET_H_

#include "RCIPtr.h"
#include "RCObject.h"
#include "Widget.h"

class RCWidget
{
public:
	RCWidget(int size):value(new Widget(size)){};
	void doThis(){value->doThis();};
	int showThat() const{return value->showThat();};
private:
	RCIPtr<Widget> value;
};

#endif /* RCWIDGET_H_ */
