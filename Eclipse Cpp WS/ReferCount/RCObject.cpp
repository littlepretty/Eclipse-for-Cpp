/*
 * RCObject.cpp
 *
 *  Created on: 2012-4-4
 *      Author: yjq
 */

#include "RCObject.h"

RCObject::RCObject():refCount(0),shareable(true)
{
}

RCObject::~RCObject(){}

RCObject::RCObject(const RCObject&):refCount(0),shareable(true){}

RCObject& RCObject::operator=(const RCObject&)
{return *this;}

void RCObject::addReference(){++refCount;}

void RCObject::removeReference()
{
	if(--refCount==0)
		delete this;
}

void RCObject::markUnshareable(){shareable=false;}

bool RCObject::isShareable() const
{return shareable;}

bool RCObject::isShared() const
{return refCount>1;}





