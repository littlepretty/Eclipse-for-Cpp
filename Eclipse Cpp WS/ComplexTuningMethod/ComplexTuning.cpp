/*
 * ComplexTuning.cpp
 *
 *  Created on: 2012-5-3
 *      Author: pc5
 */

#include "ComplexTuning.h"
#include <fstream>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>

ComplexTuning::ComplexTuning(int n,int m)
{
	n_=n;
	m_=m;

	/*
	 * �ڹ��캯���з����Ա�����Ŀռ�
	 */
	a_=new double [n_];
	b_=new double [n_];
	x_=new double [n_+1];

	Xf_=new double [n_];
	Xt_=new double [n_];
	T_=new double [n_];

	C_=new double [m_];
	W_=new double [m_];
	D_=new double [m_];

	X_=new double * [n_+1];
	for(int i=0;i<n_;i++)
	{
		X_[i]=new double [2*n];
	}
	f_=new double [2*n];
	flag_=0;
}

ComplexTuning::~ComplexTuning()
{
	/*
	 * �������������ͷ��ڴ�ռ�
	 */
	delete [] a_;
	delete [] b_;
	delete [] x_;
	delete [] Xf_;
	delete [] Xt_;
	delete [] T_;
	for(int i=0;i<n_;i++)
	{
		delete [] X_[i];
	}
	delete [] X_;
	delete [] f_;
}
/*
 * ��������������Ϣ
 */
void ComplexTuning::ReadInfo()
{
	char filename[]="Information.txt";
	std::ifstream in(filename);
	if(!in)
	{
		std::perror(filename);
	}

	in>>ForceStop_>>alpha_>>eps_;
	std::cout<<ForceStop_<<" "<<alpha_<<" "<<eps_<<"\n";

	for(int i=0;i<n_;i++)
	{
		in>>a_[i];
		std::cout<<"a_["<<i<<"]="<<a_[i]<<" ";
	}
	std::cout<<"\n";
	for(int i=0;i<n_;i++)
	{
		in>>b_[i];
		std::cout<<"b_["<<i<<"]="<<b_[i]<<" ";
	}
	std::cout<<"\n";
	for(int i=0;i<n_;i++)
	{
		in>>x_[i];
		std::cout<<"x_["<<i<<"]="<<x_[i]<<" ";
	}
	std::cout<<"\n\n";
}

/*
 * �ڿ���̨��ʾ���յ����ŵ㼰����ֵ
 */
void ComplexTuning::Output()
{
	std::cout<<"\n\nIteration Numbers: "<<flag_<<"\n"<<"Variable's Value: ";
	for(int i=0;i<n_;i++)
	{
		std::cout<<"x["<<i<<"]="<<x_[i]<<"  ";
	}
	std::cout<<"\nmin(f)="<<x_[n_]<<"\n";
}
/*
 * ����Ŀ�꺯��ֵ��ֵ
 */
double ComplexTuning::TargetFunction(double *var)
{
	double y=std::pow(var[1],3)/(27*std::sqrt(3.0));
	y=-y*(9-std::pow(var[0]-3,2));
	return y;
}
/*
 * �Զ��庯��Լ����
 * C_,W_,D_Լ��	C(x_)<=W_(x_)<=D_(x_)
 */
void ComplexTuning::FunctionConstrains(double *var)
{
	C_[0]=0.0;
	C_[1]=0.0;
	D_[0]=var[0]/std::sqrt(3.0);
	D_[1]=6;
	W_[0]=var[1];
	W_[1]=var[0]+var[1]*std::sqrt(3);
}
/*
 * ����һ��(0,1)֮��������
 */
double ComplexTuning::GenRand()
{
	/*
	double x=0;
	std::srand((unsigned)time(0));
	x=std::rand()/(double)RAND_MAX;
	return x;
	*/
	double s,u,v,p;
	int m;
	s=65536.0;
	u=2053.0;
	v=13849.0;
	m=(int)(var_/s);
	var_=var_-m*s;
	var_=u*var_+v;
	m=(int)(var_/s);
	var_=var_-m*s;
	p=var_/s;
	return p;
}

void ComplexTuning::Solution()
{
	var_=0.0;
	double fT,fR,fG;
	bool iter,iterXR;
	bool isViolate;
	/*
	 * ���ô��ļ��ж����ĳ�ʼ���е�������2n_�����е㣬���ɸ���
	 */
	for(int i=0;i<n_;i++)
	{
		X_[i][0]=x_[i];	//��ʼ���е�
	}
	f_[0]=TargetFunction(x_);

	for(int j=1;j<2*n_;j++)
	{
		/*
		 * ��a_��b_�ĳ���Լ���£��������2n_����
		 */
		for(int i=0;i<n_;i++)
		{
			double rand=GenRand();
			X_[i][j]=a_[i]+(b_[i]-a_[i])*rand;
			std::cout<<"random number this time:"<<rand<<"\n";
			x_[i]=X_[i][j];
		}
		iter=true;
		/*
		 * ���������ɵĵ��Ƿ�����Լ����������������ǰj���������T_�����е���
		 */
		while(iter)
		{
			iter=false;
			int i=0;
			isViolate=false;
			while(i<n_ && isViolate==false)
			{
				if(a_[i]<=x_[i] && x_[i]<=b_[i])	//���������Ƿ����㳣������
				{
					++i;
				}
				else
				{
					isViolate=true;
				}
			}
			if(isViolate==false)
			{
				FunctionConstrains(x_);
				int i=0;
				while(i<m_ && isViolate==false)
				{
					if(C_[i]<=W_[i] && W_[i]<=D_[i])	////���������Ƿ����㺯������
					{
						++i;
					}
					else
					{
						isViolate=true;
					}
				}
			}
			/*
			 * ��ǰj-1�����������ĵ������j����
			 */
			if(isViolate==true)
			{
				for(int i=0;i<n_;i++)
				{
					T_[i]=0;
					for(int k=0;k<j;k++)
					{
						T_[i]+=X_[i][k]/j;	//��ǰj-1���������
					}
					X_[i][j]=(T_[i]+X_[i][j])/2;	//����
					x_[i]=X_[i][j];
				}
				iter=true;
			}
			else
			{
				f_[j]=TargetFunction(x_);	//��j�����Ѿ�����Լ������������õ��ϵĺ���ֵ
			}

		}
	}

	iter=true;
	while(iter)
	{
		std::cout<<"Iter Number: "<<flag_<<"\n";
		for(int i=0;i<n_;i++)
		{
			for(int j=0;j<2*n_;j++)
			{
				std::cout<<X_[i][j]<<" ";
			}
			std::cout<<"\n";

		}
		//std::cout<<"\n";
		for(int i=0;i<2*n_;i++)
		{
			std::cout<<"f["<<i<<"]="<<f_[i]<<"\n";
		}

		iter=false;
		/*
		 * ȷ����㣬��2n_�����к���ֵ���ĵ�
		 */
		fR=f_[0];
		int R=0;
		for(int j=1;j<2*n_;j++)
		{
			if(fR<f_[j])
			{
				fR=f_[j];
				R=j;
			}
		}
		/*
		 * ȷ���λ��㣬��2n_�����к���ֵ�ڶ���ĵ�
		 */
		fG=f_[0];
		int G=0;
		int begin=0;
		if(R==0)
		{
			G=1;
			begin=1;
			fG=f_[G];
		}
		for(int j=begin+1;j<2*n_;j++)
		{
			if(fG<f_[j] && j!=R)
			{
				fG=f_[j];
				G=j;
			}
		}
		std::cout<<"R="<<R<<" G="<<G<<"\n";
		std::cout<<"\n\n";
		/*
		 * ���ȼ���2n_���������Xf_��Ȼ���������ڸ����ĵĶԳƵ�Xt_
		 */
		for(int i=0;i<n_;i++)
		{
			Xf_[i]=0;
			for(int j=0;j<2*n_;j++)
			{
				if(j!=R)
				{
					Xf_[i]+=X_[i][j]/(2*n_-1);
				}
			}
			Xt_[i]=(1.0+alpha_)*Xf_[i]-alpha_*X_[i][R];
		}

		/*
		 * ����ѡ����㣨����ĳ��򣩣��öԳƵ�������㣬������Լ����������
		 */
		iterXR=true;
		while(iterXR)
		{
			iterXR=false;
			isViolate=false;
			fT=TargetFunction(Xt_);

			/*
			 * �����ԳƵ㣬ֱ���ԳƵ�ĺ���ֵС�ڴλ���
			 */
			while(fT>fG)
			{
				for(int i=0;i<n_;i++)
				{
					Xt_[i]=(Xf_[i]+Xt_[i])/2;
				}
				fT=TargetFunction(Xt_);
			}

			/*
			 * �����㳣������ʱ����������
			 */
			for(int i=0;i<n_;i++)
			{
				if(a_[i]>Xt_[i])
				{
					Xt_[i]=a_[i]+1.0e6;
					isViolate=true;
				}
				if(b_[i]<Xt_[i])
				{
					Xt_[i]=b_[i]-1.0e6;
					isViolate=true;
				}
			}
			if(isViolate==true)
				iterXR=true;
			else
			{
				FunctionConstrains(Xt_);	//��ú���Լ��
				int i=0;
				isViolate=false;
				while(i<m_ && isViolate==false)
				{
					if(C_[i]<=W_[i] && W_[i]<=D_[i])
						++i;
					else
						isViolate=true;
				}
				if(i<m_)              //
				{
					/*
					 * �����㺯������ʱ����������
					 */
					for(int l=0;l<n_;l++)
					{
						Xt_[l]=(Xt_[l]+Xf_[l])/2;
						iterXR=true;
					}
				}
			}
		}
		/*
		 * ������޳�����Ϊ��ԳƵ㣬���㸴�δ�����ĺ���ֵ���ж��Ƿ��Ѿ���Ϊһ������ֵ
		 */
		for(int i=0;i<n_;i++)
		{
			X_[i][R]=Xt_[i];
		}
		f_[R]=fT;
		fR=fG=0;
		double tmp;
		for(int i=0;i<2*n_;i++)
		{
			tmp=f_[i];
			fR+=tmp/(2.0*n_);
			fG+=tmp*tmp;
		}
		tmp=(fG-2*n_*fR*fR)/(2*n_-1);
		if(tmp>=eps_)
		{
			++flag_;	//������������
			iter=true;
		}
	}
	/*
	 * ������������2n_���㣨�Ѿ����ƾۼ�Ϊһ���㣩��������Ϊ���ŵ�
	 */
	for(int i=0;i<n_;i++)
	{
		x_[i]=0;
		for(int j=0;j<2*n_;j++)
		{
			x_[i]+=X_[i][j]/(2.0*n_);
			Xt_[i]=x_[i];
		}
	}
	x_[n_]=TargetFunction(Xt_);
	return;

}
