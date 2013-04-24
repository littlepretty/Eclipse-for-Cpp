/*
 * RCIPtr.cpp
 *
 *  Created on: 2012-4-4
 *      Author: yjq
 */

#include "RCIPtr.h"

template <class T>
void RCIPtr<T>::init()
{
	if(counter->isShareable()==false)
	{
		T *oldValue=counter->pointee;
		counter=new CounterHolder;
		counter->pointee=new T(*oldValue);
	}
	counter->addReference();
}

template <class T>
RCIPtr<T>::RCIPtr(T* realPtr):counter(new CounterHolder)
{
	counter->pointee=realPtr;
	init();
}

template <class T>
RCIPtr<T>::RCIPtr(const RCIPtr<T>& rhs):counter(rhs.counter)
{
	init();
}

template <class T>
RCIPtr<T>::~RCIPtr()
{
	counter->removeReference();
}

template <class T>
RCIPtr<T>& RCIPtr<T>::operator =(const RCIPtr<T>& rhs)
{
	if(counter!=rhs.counter)
	{
		counter->removeReference();
		counter=rhs.counter;
		init();
	}
	return *this;
}

template <class T>
void RCIPtr<T>::makeCopy()
{
	if(counter->isShareable())
	{
		T *oldValue=counter->pointee;
		counter->removeReference();
		counter=new CounterHolder;
		counter->pointee=new T(*oldValue);
		counter->addReference();
	}
}


template <class T>
const T* RCIPtr<T>::operator -> () const
{return counter->pointee;}

template <class T>
T* RCIPtr<T>::operator -> ()
{
	makeCopy();
	return counter->pointee;
}

template <class T>
const T& RCIPtr<T>::operator * () const
{return *(counter->pointee);}

template <class T>
T& RCIPtr<T>::operator * ()
{
	makeCopy();
	return *(counter->pointee);
}

