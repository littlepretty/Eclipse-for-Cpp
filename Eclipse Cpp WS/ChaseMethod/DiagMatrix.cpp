#include "DiagMatrix.h"
#include <fstream>
#include <cmath>
#include <iostream>

DiagMatrix::DiagMatrix()
{
	std::ifstream inMat("rawMatrix.txt");
	if(!inMat)
	{
		std::perror("rawMatrix.txt");
		std::cout<<"Fail to Construct!\n";
	}

	/*
	 * 从文件rawMatrix.txt中读取矩阵信息和矩阵中的非零元素
	 */
	inMat>>band;
	inMat>>n_;
	Capacity=(2*band+1)*n_-band-band*band;	//计算对角矩阵的容量
	DM_=new double[Capacity];
	/*
	double tmp;
	int j=0;
	for(int i=0;i<n_*n_;i++)
	{
		inMat>>tmp;
		if(tmp!=0)
		{
			DM_[j]=tmp;
			std::cout<<DM_[j]<<" ";
			++j;
		}
	}
	*/
	int k=0;
	double tmp;
	/*
	 * 读入band内的元素，按行优先原则放入DM_
	 */
	for(int i=0;i<n_;i++)
	{
		for(int j=0;j<n_;j++)
		{
			inMat>>tmp;
			if(!isZero(i,j))
			{
				DM_[k]=tmp;
				std::cout<<DM_[k]<<" ";
				++k;
			}
		}
	}
	std::cout<<"\n";
	inMat.close();
}


DiagMatrix::~DiagMatrix(void)
{
	delete[] DM_;
}


void DiagMatrix::ReadInData()
{
	char filename[20]="DiagMatrixData.txt";
	std::ifstream in(filename);
	if(!in)
	{
		std::perror(filename);
	}

	for(int i=0;i<Capacity;i++)
	{
		in>>DM_[i];
		std::cout<<DM_[i]<<" ";
	}
	std::cout<<"\n";
	in.close();
}

/*
 * 判断是否为带宽内的元素
 */
bool DiagMatrix::isZero(int i,int j)
{
	int tmp=i-j>=0?i-j:j-i;
	return tmp>band;
}
/*
 * 根据索引公式计算对角矩阵中(i,j)上的元素在DM_中的索引值
 */
int DiagMatrix::Location(int i,int j)
{
	if(!isZero(i,j))
	{
		if(i>=1 && i<=band)
			return (2*band+1)*(i-1)/2+j-1;
		if(i>=band+1 && i<=n_-band)
			return (3*band+1)*band/2+(i-band-1)*(2*band+1)+j-i+band;
		if(i>=n_-band+1 && i<=n_)
			return (3*band+1)*band/2+(n_-2*band)*(2*band+1)+(3*band-i+n_+2)*(i-n_+band-1)/2+j-i+band;
		else 
		{
			std::cout<<"fuck index error";
			return -1;
		}
	}
	else
		return -1;
}
/*
 * 获取矩阵元素的值
 */
double DiagMatrix::GetValue(int i,int j)
{
	if(!isZero(i,j))
	{
		return DM_[Location(i,j)];
	}
	else
		return 0;
}
/*
 * 重载索引DM_
 */
double DiagMatrix::operator[](int index)
{
	return DM_[index];
}

/*
void DiagMatrix::ScanMatrixToDM(std::ifstream &in)
{
	double tmp;
	int j=0;
	for(int i=0;i<n_*n_;i++)
	{
			in>>tmp;
			if(tmp!=0)
			{
					DM[j]=tmp;
					std::cout<<DM[j]<<" ";
					++j;
			}
	}
	std::cout<<"\n";
}
*/
