#include "DMConjGrad.h"
#include <fstream>
#include <iostream>
#include "MatrixOperateYJQ.h"

DMConjGrad::DMConjGrad()
{
	A=new DiagMatrix();		//�������ɶԽǾ���A
	n_=A->GetN();
	b=new double[n_];
	x=new double[n_];
}

DMConjGrad::~DMConjGrad(void)
{
	delete A;
}
/*
 * ���ļ�Equation.txt�ж�ȡb��eps
 */
void DMConjGrad::ReadInMatrix()
{
	//A->ReadInData();
	char filename[20]="Equation.txt";
	std::ifstream in(filename);
	if(!in)
	{
		std::perror(filename);
	}

	for(int i=0;i<n_;i++)
	{
		in>>b[i];
		std::cout<<b[i]<<" ";
	}
	std::cout<<"\n";
	in>>eps;
	in.close();
}
/*
 * ��������д��ResultData.txt
 */
void DMConjGrad::GiveResult()
{
	char filename[20]="ResultData.txt";
	std::ofstream out(filename);
	if(!out)
	{
		std::perror(filename);
	}
	for(int i=0;i<n_;i++)
	{
		out<<x[i]<<"   ";
		std::cout<<x[i]<<"   ";
	}
	out<<"\n";
	std::cout<<"\n";
	out.close();
}

void DMConjGrad::operator ()()
{
	int ForceStop(1000);
	double *r=new double [n_];
	double *P=new double [n_];
	double *AP=new double [n_];
	double *Ar=new double [n_];
	double *Ax=new double [n_];
	/*
	 * ��ʼ��������Ϊ������
	 * ��ʼ���в�r������PΪb
	 */
	for(int i=0;i<n_;i++)
	{
		r[i]=P[i]=b[i];
		x[i]=0.0;
	}
	double alf,beta;
	//���������������в�r�������С��eps
	while(NormalMatrix::Norm(r,n_)>eps)
	{
		AP=DiagMaxtrix::DiagMatrixProduct(A,P,n_);	//AP=A*P
		alf=NormalMatrix::InnerProduct(P,b,n_)/NormalMatrix::InnerProduct(P,AP,n_);		//alf=(P,b)/(P,AP)

		for(int i=0;i<n_;i++)
		{
			x[i]=x[i]+alf*P[i];		//x=x+alf*P
		}
		GiveResult();
		Ax=DiagMaxtrix::DiagMatrixProduct(A,x,n_);		//Ax=A*x
		for(int i=0;i<n_;i++)
		{
			r[i]=b[i]-Ax[i];		//r=b-Ax
		}

		Ar=DiagMaxtrix::DiagMatrixProduct(A,r,n_);	//Ar=A*r
		beta=NormalMatrix::InnerProduct(Ar,P,n_)/NormalMatrix::InnerProduct(AP,P,n_);	//beta=(Ar,P)/(AP,P)

		for(int i=0;i<n_;i++)
		{
			P[i]=r[i]-beta*P[i];	//P=r-beta*P
		}

		--ForceStop;
		if(ForceStop<0)
		{
			delete [] r;
			delete [] P;
			delete [] Ar;
			delete [] AP;
			delete [] Ax;
			return;
		}
	}
	delete [] r;
	delete [] P;
	delete [] Ar;
	delete [] AP;
	delete [] Ax;
}
