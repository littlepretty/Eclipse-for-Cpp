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
	 * ���ļ�rawMatrix.txt�ж�ȡ������Ϣ�;����еķ���Ԫ��
	 */
	inMat>>band;
	inMat>>n_;
	Capacity=(2*band+1)*n_-band-band*band;	//����ԽǾ��������
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
	 * ����band�ڵ�Ԫ�أ���������ԭ�����DM_
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
 * �ж��Ƿ�Ϊ�����ڵ�Ԫ��
 */
bool DiagMatrix::isZero(int i,int j)
{
	int tmp=i-j>=0?i-j:j-i;
	return tmp>band;
}
/*
 * ����������ʽ����ԽǾ�����(i,j)�ϵ�Ԫ����DM_�е�����ֵ
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
 * ��ȡ����Ԫ�ص�ֵ
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
 * ��������DM_
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
