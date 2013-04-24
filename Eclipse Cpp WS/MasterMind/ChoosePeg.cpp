/*
 * ChoosePeg.cpp
 *
 *  Created on: 2012-4-9
 *      Author: pc5
 */

#include <string>
#include <cstdlib>
//using namespace std;

class ChoosePeg
{
public:
	ChoosePeg(const std::string& colors):colors_(colors){};
	char operator()() const
	{
		return colors_[std::rand()%colors_.size()];
	}

private:
	const std::string& colors_;
};
