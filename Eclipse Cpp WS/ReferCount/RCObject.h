/*
 * RCObject.h
 *
 *  Created on: 2012-4-4
 *      Author: yjq
 */

#ifndef RCOBJECT_H_
#define RCOBJECT_H_

class RCObject
{
public:
	void addReference();
	void removeReference();
	void markUnshareable();
	bool isShareable() const;
	bool isShared() const;

protected:
	RCObject();
	RCObject(const RCObject& rhs);
	RCObject& operator=(const RCObject& rhs);
	virtual ~RCObject()=0;
private:
	int refCount;
	bool shareable;
};

#endif /* RCOBJECT_H_ */
