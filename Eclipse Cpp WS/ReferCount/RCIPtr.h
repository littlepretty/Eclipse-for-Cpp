/*
 * RCIPtr.h
 *
 *  Created on: 2012-4-4
 *      Author: yjq
 */

#ifndef RCIPTR_H_
#define RCIPTR_H_
#include "RCObject.h"

template <class T>
class RCIPtr
{
public:
	RCIPtr(T* realPtr=0);
	RCIPtr(const RCIPtr& rhs);
	~RCIPtr();

	RCIPtr& operator=(const RCIPtr& rhs);

	const T* operator -> () const;
	T* operator -> ();
	const T& operator * () const;
	T& operator * ();

private:
	struct CounterHolder: public RCObject
	{
		~CounterHolder(){delete pointee;}
		T *pointee;
	};
	CounterHolder *counter;
	void init();
	void makeCopy();
};


#endif /* RCIPTR_H_ */
