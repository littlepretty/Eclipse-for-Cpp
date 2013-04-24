/*
 * Widget.h
 *
 *  Created on: 2012-4-4
 *      Author: yjq
 */

#ifndef WIDGET_H_
#define WIDGET_H_

class Widget
{
public:
	Widget(int size);
	Widget(const Widget& rhs);
	~Widget();
	Widget& operator=(const Widget& rhs);
	void doThis();
	int showThat() const;
private:
	int secret;

};

#endif /* WIDGET_H_ */
