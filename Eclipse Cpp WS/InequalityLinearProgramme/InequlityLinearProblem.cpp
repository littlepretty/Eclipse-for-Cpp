/*
 * InequlityLinearProblem.cpp
 *
 *  Created on: 2012-4-30
 *      Author: pc5
 */
#include <fstream>
#include "InequlityLinearProblem.h"
#include "MatrixOperateYJQ.h"

InequlityLinearProblem::InequlityLinearProblem(int n,int m):n_(n),m_(m)
{
	isConverge=false;
	/*
	 * 在构造函数中分配成员变量的空间
	 */
	pick_=new int [m];
	b_=new double [m_];
	x_=new double [n_+m_];
	c_=new double [n_+m_];
	A_=new double* [m_];
	for(int i=0;i<n;i++)
	{
		A_[i]=new double [n_+m_];
	}

	P_=new double* [m_];
	for(int i=0;i<n;i++)
	{
		P_[i]=new double [m_];
	}

	D_=new double* [m_];
	for(int i=0;i<n;i++)
	{
		D_[i]=new double [n_+m_];
	}
}

InequlityLinearProblem::~InequlityLinearProblem() {
	/*
	 * 在析构函数中释放内存空间
	 */
	delete [] pick_;
	delete [] b_;
	delete [] x_;
	delete [] c_;
	for(int i=0;i<m_;i++)
	{
		delete [] A_[i];
		delete [] P_[i];
		delete [] D_[i];
	}
}
/*
 * 读入求解问题的信息
 */
void InequlityLinearProblem::ReadInfo()
{
	char filename[]="Information.txt";
	std::ifstream in(filename);
	if(!in)
	{
		std::perror(filename);
	}
	for(int i=0;i<m_;i++)
	{
		for(int j=0;j<m_+n_;j++)
		{
			in>>A_[i][j];
			std::cout<<A_[i][j]<<" ";
		}
		std::cout<<"\n";
	}

	for(int i=0;i<m_;i++)
	{
		in>>b_[i];
		std::cout<<b_[i]<<" ";
	}
	std::cout<<"\n";
	for(int i=0;i<m_+n_;i++)
	{
		in>>c_[i];
		std::cout<<c_[i]<<" ";
	}
	std::cout<<"\n\n";

	in.close();
}

/*
 * 在控制台显示最终的最优点及最优值
 */
void InequlityLinearProblem::Output()
{
	char filename[]="Result.txt";
	std::ofstream out(filename);
	if(!out)
	{
		std::perror(filename);
	}

	if(!isConverge)
	{
		out<<"FAIL TO SOLVE THIS PROBELM";
		return;
	}
	out<<"if converge? "<<isConverge<<" \n";
	std::cout<<"if converge? "<<isConverge<<" \n";
	for(int i=0;i<n_;i++)
	{
		out<<"x["<<i<<"]="<<x_[i]<<" ";
		std::cout<<"x["<<i<<"]="<<x_[i]<<" ";
	}
	out<<"\nmin(f)="<<x_[n_]<<"\n";
	std::cout<<"\nmin(f)="<<x_[n_]<<"\n";
	out.close();
}

void InequlityLinearProblem::Solution()
{
	int mn=m_+n_;
	for(int i=0;i<m_;i++)
		pick_[i]=n_+i;

	while(!isConverge)
	{
		for(int i=0;i<m_;i++)
		{
			for(int j=0;j<m_;j++)
			{
				P_[i][j]=A_[i][pick_[j]];
			}
		}
		P_=Inverse(P_,m_);

		D_=MatrixProduct(P_,A_,m_,m_,m_+n_);

		for(int i=0;i<m_+n_;i++)
		{
			x_[i]=0.0;
		}

		double tmp;
		for(int i=0;i<m_;i++)
		{
			tmp=0.0;
			for(int j=0;j<m_;j++)
			{
				tmp+=P_[i][j]*b_[j];
			}
			x_[pick_[i]]=tmp;
		}
		int k=-1;
		double eps=1.0e-35;
		double z;
		for(int j=0;j<mn;j++)
		{
			z=0;
			for(int i=0;i<m_;i++)
			{
				z+=c_[pick_[i]]*D_[i][j];
			}
				z=z-c_[j];
				if(z>eps)
				{
					eps=z;
					k=j;
				}
		}

		if(k==-1)
		{
			tmp=0;
			for(int j=0;j<n_;j++)
			{
				tmp+=c_[j]*x_[j];
			}
			x_[n_]=tmp;
			isConverge=true;
			return;
		}

		int pickout=-1;
		double min=1.0e20;
		for(int l=0;l<m_;l++)
		{
			if(D_[l][k]>=1.0e-22)
			{
				double y=x_[pick_[l]]/D_[l][k];
				if(y<min)
				{
					min=y;
					pickout=l;
				}
			}
		}
		if(pickout==-1)
		{
			isConverge=false;
			return;
		}
		pick_[pickout]=k;

	}
}















