/*
 * ChaseAfter.cpp
 *
 *  Created on: 2012-4-27
 *      Author: pc5
 */

#include "ChaseAfter.h"
#include "DiagMatrix.h"
#include <fstream>
#include <iostream>

ChaseAfter::ChaseAfter()
{
	//DM=*(new DiagMatrix());
	/*
	 * 从Right.txt读取规模及f
	 */
	std::ifstream inEqu("Right.txt");
	int row=DM.GetN();
	f=new double[row];
	x=new double[row];
	for(int i=0;i<row;i++)
	{
		inEqu>>f[i];
	}
	inEqu.close();
}

ChaseAfter::~ChaseAfter()
{
	delete [] f;
	delete [] x;
	DM.~DiagMatrix();
}

void ChaseAfter::operator ()()
{
	int row=DM.GetN();
	double *beta=new double[row];
	beta[0]=DM[1]/DM[0];
	for(int i=1;i<row-1;i++)
	{
		beta[i]=DM[3*i+1]/(DM[3*i]-DM[3*i-1]*beta[i-1]);
	}

	double *tmpY=new double[row];
	tmpY[0]=f[0]/DM[0];
	for(int i=1;i<row;i++)
	{
		tmpY[i]=(f[i]-DM[3*i-1]*tmpY[i-1])/(DM[3*i]-DM[3*i-1]*beta[i-1]);
	}

	/*
	 * 回代解出x
	 */
	x[row-1]=tmpY[row-1];
	for(int i=row-2;i>=0;i--)
	{
		x[i]=tmpY[i]-beta[i]*x[i+1];
	}
	for(int i=0;i<row;i++)
	{
		std::cout<<x[i]<<" ";
	}
	std::cout<<"\n";

}















