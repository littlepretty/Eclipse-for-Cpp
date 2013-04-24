/*
 * Widget.cpp
 *
 *  Created on: 2012-4-4
 *      Author: yjq
 */

#include "Widget.h"

Widget::Widget(int size):secret(size)
{}

Widget::Widget(const Widget& rhs):secret(rhs.secret)
{}

Widget& Widget::operator =(const Widget& rhs)
{
	secret=rhs.secret;
}

Widget::~Widget()
{
	// TODO Auto-generated destructor stub
}

void Widget::doThis()
{
	++secret;
}

int Widget::showThat() const
{
	return secret;
}

