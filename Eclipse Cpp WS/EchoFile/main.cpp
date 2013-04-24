/*
 * main.cpp
 *
 *  Created on: 2012-4-7
 *      Author: pc5
 */

#include <fstream>
#include <iostream>

template<typename In, typename Out>
void Process(In& in,Out& out)
{
	out<<in.rdbuf();
}

int main(int argc, char* argv[])
{
	using namespace std;
	fstream in;
	fstream out;
	if(argc>1)
		in.open(argv[1],ios::in|ios::binary);
	if(argc>2)
		out.open(argv[2],ios::out|ios::binary);
	bool inopen=in.is_open();
	bool outopen=out.is_open();

	Process(inopen?in:cin,outopen?out:cout);

}
