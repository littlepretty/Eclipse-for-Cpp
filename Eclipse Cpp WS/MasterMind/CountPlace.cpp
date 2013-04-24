/*
 * CountPlace.cpp
 *
 *  Created on: 2012-4-9
 *      Author: pc5
 */

#include <map>
#include "M.h"

class CountPlace
{
public:
	CountPlace(M& cm,M& gm, int& pok):cm_(cm),gm_(gm),pok_(pok=0){};
	char operator()(char c,char g)
	{
		return ++cm_[c],++gm_[g],pok_+=c==g,g;
	}
private:
	M &cm_,&gm_;
	int& pok_;

};
