/*
 * Main.cpp
 *
 *  Created on: 2012-4-9
 *      Author: pc5
 */

#include "ChoosePeg.cpp"
#include "CountPlace.cpp"
#include "CountColor.cpp"
#include <string>
#include "M.h"
#include <ctime>
#include <iostream>
int main()
{
	const int number(4);
	const std::string color("RGB");
	std::string noconst(color);
	std::cout<<number<<" IS THE LENGTH OF YOUR GUESS\n"<<"ALL POSSIBLE COLORS ARE: ";
	for(unsigned int i=0;i<noconst.size();i++)
		std::cout<<noconst[i];
	std::cout<<"\nI WILL GIVE YOU SOME CLUE AFTER YOU GUESS ONCE"<<std::endl;
	std::cout<<"THE FIRST NUMBER IS HOW MANY YOUR GUESSED COLORS ARE MATCHING WITH THE ANSWER"<<std::endl;
	std::cout<<"THE SECOND NUMBER IS THE PLACE AT WHICH PLACE YOUR GUESSED COLORS HAVE MATCHED THE ANSWER"<<std::endl;
	std::string comb(number,'.'),guess;
	int cok=0,pok=0;
	M cm,gm;
	std::srand(time(0)),generate(comb.begin(),comb.end(),ChoosePeg(color));

	while(pok<comb.size())
	{
		std::cout<<"\nWHAT IS YOUR GUESS?-->";
		std::cin>>guess;
		guess.resize(comb.size(),' ');
		cm=gm=M();
		std::transform(comb.begin(),comb.end(),guess.begin(),guess.end(),CountPlace(cm,gm,pok));
		std::for_each(color.begin(),color.end(),CountColor(cm,gm,cok));
		if(pok!=comb.size())
			std::cout<<cok<<' '<<pok;
	}
	std::cout<<"\n-CONGRATULATIONS!!! PROBLEM SOLVED!\n";
	char ch;
	std::cout<<"PRESS ANY CHAR TO EXIT!";
	std::cin>>ch;
	return 0;

}
