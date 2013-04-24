/*
 * SingularTuning.cpp
 *
 *  Created on: 2012-5-2
 *      Author: pc5
 */

#include "SingularTuning.h"
#include <fstream>
#include <iostream>
#include <cmath>

SingularTuning::SingularTuning(int n)
{
	n_=n;
	/*
	 * �ڹ��캯���з����Ա�����Ŀռ�
	 */
	x_=new double[n_+1];
	//ForceStop_=1000;
	//niu=1.6;
	//lamta=0.4;
	X_=new double *[n_];
	for(int i=0;i<n_;i++)
	{
		X_[i]=new double[n_+1];
	}
	f_=new double [n_+1];
	Xe_=new double[n_];
	Xt_=new double[n_];
	Xf_=new double[n_];
	//eps_=1.0e-28;
	flag_=0;
}

SingularTuning::~SingularTuning()
{
	/*
	 * �������������ͷ��ڴ�ռ�
	 */
	delete [] x_;
	delete [] f_;
	delete [] Xe_;
	delete [] Xt_;
	delete [] Xf_;
	for(int i=0;i<n_;i++)
	{
		delete[] X_[i];
	}
	delete[] X_;
}

/*
 * ��������������Ϣ
 */
void SingularTuning::ReadInfo()
{
	char filename[]="Information.txt";
	std::ifstream in(filename);
	if(!in)
		std::perror(filename);

	in>>ForceStop_>>dist_>>niu>>lamta>>eps_;
	std::cout<<ForceStop_<<" "<<dist_<<" "<<niu<<" "<<lamta<<" "<<eps_<<" \n";
	in.close();
}

/*
 * �ڿ���̨��ʾ���յ����ŵ㼰����ֵ
 */
void SingularTuning::Output()
{
	std::cout<<"Iteration Numbers: "<<flag_<<"\n"<<"Variable's Value: ";
	for(int i=0;i<n_;i++)
	{
		std::cout<<"x["<<i<<"]="<<x_[i]<<"  ";
	}
	std::cout<<"\nmin(f)="<<x_[n_]<<"\n";

}
/*
 * ����Ŀ�꺯��ֵ��ֵ
 */
double SingularTuning::TargetFunction(double *var)
{
	return std::pow(var[1]-var[0]*var[0],2)*100+std::pow(1-var[0],2);
}

void SingularTuning::InitX()
{
	double nn=n_*1.0;
	double fr=std::sqrt(1+nn);
	double fl=dist_*(fr-1)/(std::sqrt(2)*nn);
	double fg=dist_*(fr+nn-1)/(std::sqrt(2)*nn);
	for(int i=0;i<n_;i++)
	{
		for(int j=0;j<n_+1;j++)
		{
			X_[i][j]=fl;
		}
	}
	for(int i=1;i<n_+1;i++)
	{
		X_[i-1][i]=fg;
	}
}

void SingularTuning::Solution()
{
	/*
	 * ���ݳ�ʼ���ɵĵ��ε㣬����ÿ���ϵĺ���ֵ
	 */
	for(int i=0;i<n_+1;i++)
	{
		for(int j=0;j<n_;j++)
		{
			Xt_[j]=X_[j][i];
		}
		f_[i]=TargetFunction(Xt_);
	}

	double isBig=100+eps_;
	int number=0;
	double fR;
	double fG;
	double fL;
	double fT,fE;
	int R,G,L;
	while(isBig>eps_ && number<ForceStop_)
	{
		number=number+1;
		//for(int i=0;i<n_+1;i++)
		//{
		//	std::cout<<"\nx_["<<i<<"]="<<x_[i]<<" ";
		//}
		std::cout<<"\nX_ now is: \n";
		for(int i=0;i<n_;i++)
		{
			for(int j=0;j<n_+1;j++)
			{
				std::cout<<X_[i][j]<<" ";
			}
			std::cout<<"\n";
		}
		for(int i=0;i<n_+1;i++)
		{
			std::cout<<"f_["<<i<<"]="<<f_[i]<<" \n";
		}


		/*
		 * Ѱ����㣨R�궨�����λ��㣨G�궨������õ㣨L�궨��
		 */
		fR=fG=fL=f_[0];
		R=G=L=0;
		for(int i=0;i<n_+1;i++)
		{
			if(fR<f_[i])
			{
				fR=f_[i];
				R=i;
			}
			if(fL>f_[i])
			{
				fL=f_[i];
				L=i;
			}
		}
		int begin=0;
		if(R==0)
		{
			G=1;fG=f_[1];begin=1;
		}
		for(int i=begin+1;i<n_+1;i++)
		{
			if(fG<f_[i] && i!=R)
			{
				fG=f_[i];
				G=i;
			}
		}

		std::cout<<"R="<<R<<" G="<<G<<" L="<<L<<"\n";
		std::cout<<"\n\n";
		/*
		 * ���ȼ����ȥ�������е������Xf_��Ȼ�����������Xf_�ĶԳƵ�
		 */
		for(int j=0;j<n_;j++)
		{
			Xf_[j]=0;
			for(int i=0;i<n_+1;i++)
			{
				if(i!=R)
				{
					Xf_[j]+=X_[j][i]/n_;
				}
			}
			Xt_[j]=2*Xf_[j]-X_[j][R];
		}

		fT=TargetFunction(Xt_);

		/*
		 * �ԳƵ��ֵС����õ㣬��һ������ΪXe_
		 */
		if(fT<fL)
		{
			for(int i=0;i<n_;i++)
			{
				Xe_[i]=(1-niu)*Xf_[i]+niu*Xt_[i];           //���ŷ�ʽ
			}
			fE=TargetFunction(Xe_);
			/*
			 * Xe_�ĺ���ֵ��Ȼ�������ŵ㣬��ô��Xe_�滻�����
			 */
			if(fE<fL)
			{
				for(int i=0;i<n_;i++)
				{
					X_[i][R]=Xe_[i];
					f_[R]=fE;
				}
			}
			else
			{
				for(int i=0;i<n_;i++)
				{
					X_[i][R]=Xt_[i];	//����Ч�����ã���ԭ�ԳƵ�������
					f_[R]=fT;
				}
			}
		}
		else
		{
			//�ԳƵ���ڴλ��㣬ֱ���öԳƵ�������
			if(fT<=fG)
			{
				for(int i=0;i<n_;i++)
				{
					X_[i][R]=Xt_[i];
					f_[R]=fT;
				}
			}
			else
			{
				if(fT<=fR)   	//����ԳƵ�������㣨���ڴλ��㣩,��XR�滻ΪXT
				{
					for(int i=0;i<n_;i++)
					{
						X_[i][R]=Xt_[i];
						f_[R]=fT;
					}
				}
				/*
				 * ������С�ԳƵ�ΪXe_
				 */
				for(int i=0;i<n_;i++)
				{
					Xe_[i]=Xf_[i]*(1-lamta)+lamta*X_[i][R];
					//Xe_[i]=Xf_[i]*(1-lamta)+lamta*Xt_[i];
				}
				fE=TargetFunction(Xe_);
				/*
				 * ��С������ã�����ֵ�������㣩������Ҫ������õ�X[][L]�������µ�n_+1�����ζ���
				 */
				if(fE>f_[R])
				{
					for(int i=0;i<n_+1;i++)
					{
						for(int j=0;j<n_;j++)
						{
							X_[j][i]=(X_[j][i]+X_[j][L])/2;
							x_[j]=X_[j][i];
							Xe_[j]=x_[j];
						}
						fE=TargetFunction(Xe_);
						f_[i]=fE;
					}
				}
				/*
				 * ����Ч���ã�С�����㣩��������С��ĵ��������
				 */
				else
				{
					for(int i=0;i<n_;i++)
					{
						X_[i][R]=Xe_[i];
					}
					f_[R]=fE;
				}
			}
		}
		/*
		 * �жϵ��ε�ĺ���ֵ�Ƿ�������һ��ֵ
		 */
		isBig=0;
		double tmp=0;
		for(int j=0;j<n_;j++)
		{
			tmp+=f_[j]/(1+n_);
			isBig+=f_[j]*f_[j];
		}
		isBig=(isBig-(1.0+n_)*tmp*tmp)/n_;


		/*
		 * ����x_ΪX_�����ģ����㺯��ֵ��������ʱ�������ŵ�������ֵ
		 */
		for(int j=0;j<n_;j++)
		{
			x_[j]=0;
			for(int i=0;i<n_+1;i++)
			{
				x_[j]+=X_[j][i]/(1+n_);
				Xe_[j]=x_[j];
			}
			x_[n_]=TargetFunction(Xe_);
			flag_=number;
		}
	}

	/*
	for(int j=0;j<n_;j++)
	{
		x_[j]=0;
		for(int i=0;i<n_+1;i++)
		{
			x_[j]+=X_[j][i]/(1+n_);
			Xe_[j]=x_[j];
		}
		x_[n_]=TargetFunction(Xe_);
		flag_=number;
		return;
	}
	*/
}







