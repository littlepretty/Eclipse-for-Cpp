/*
 * CountColor.cpp
 *
 *  Created on: 2012-4-9
 *      Author: pc5
 */
#include <map>
#include "M.h"
#include <algorithm>

class CountColor
{
public:
	CountColor(M& cm,M& gm,int& cok):cm_(cm),gm_(gm),cok_(cok=0){}
	void operator()(char c)
	{
		cok_+=std::min(cm_[c],gm_[c]);
	}

private:
	M &cm_,gm_;
	int& cok_;

};
