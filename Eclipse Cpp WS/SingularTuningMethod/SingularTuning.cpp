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
	 * 在构造函数中分配成员变量的空间
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
	 * 在析构函数中释放内存空间
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
 * 读入求解问题的信息
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
 * 在控制台显示最终的最优点及最优值
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
 * 计算目标函数值的值
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
	 * 根据初始生成的单形点，计算每点上的函数值
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
		 * 寻找最坏点（R标定）、次坏点（G标定）、最好点（L标定）
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
		 * 首先计算除去最坏点的所有点的中心Xf_，然后计算最坏点关于Xf_的对称点
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
		 * 对称点的值小于最好点，进一步扩张为Xe_
		 */
		if(fT<fL)
		{
			for(int i=0;i<n_;i++)
			{
				Xe_[i]=(1-niu)*Xf_[i]+niu*Xt_[i];           //扩张方式
			}
			fE=TargetFunction(Xe_);
			/*
			 * Xe_的函数值仍然好于最优点，那么用Xe_替换掉最坏点
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
					X_[i][R]=Xt_[i];	//扩张效果不好，用原对称点代替最坏点
					f_[R]=fT;
				}
			}
		}
		else
		{
			//对称点好于次坏点，直接用对称点代替最坏点
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
				if(fT<=fR)   	//如果对称点好于最差点（差于次坏点）,将XR替换为XT
				{
					for(int i=0;i<n_;i++)
					{
						X_[i][R]=Xt_[i];
						f_[R]=fT;
					}
				}
				/*
				 * 试着缩小对称点为Xe_
				 */
				for(int i=0;i<n_;i++)
				{
					Xe_[i]=Xf_[i]*(1-lamta)+lamta*X_[i][R];
					//Xe_[i]=Xf_[i]*(1-lamta)+lamta*Xt_[i];
				}
				fE=TargetFunction(Xe_);
				/*
				 * 缩小结果不好（函数值大于最差点），则需要根据最好点X[][L]来构造新的n_+1个单形顶点
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
				 * 收缩效果好（小于最差点），则用缩小后的点替代最差点
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
		 * 判断单形点的函数值是否收敛到一个值
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
		 * 更新x_为X_的中心，计算函数值，当收敛时就是最优点与最优值
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







