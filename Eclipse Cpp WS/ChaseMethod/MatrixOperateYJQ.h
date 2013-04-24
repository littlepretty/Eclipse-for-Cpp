#include "DiagMatrix.h"
namespace NormalMatrix
{
	double InnerProduct(double *a,double *b,int size)
{
	double result=0;
	for(int i=0;i<size;i++)
	{
		result+=a[i]*b[i];
	}
	return result;
}

	double* MatrixProduct(double **a,double *b,int size)
{
	double* result=new double[size];
	for(int i=0;i<size;i++)
	{
		result[i]=0;
	}

	for(int i=0;i<size;i++)
	{
		for(int j=0;j<size;j++)
		{
			result[i]+=a[i][j]*b[j];
		}
	}
	return result;
}

	double Norm(double *a,int size)
{
	double max=a[0];
	for(int i=0;i<size;i++)
	{
		if(max<a[i])
		{
			max=a[i];
		}
	}
	return max;
}
}

namespace DiagMaxtrix
{
	double* DiagMatrixProduct(DiagMatrix *a,double *b,int size)
{
	double* result=new double[size];
	for(int i=0;i<size;i++)
	{
		result[i]=0;
	}

	for(int i=0;i<size;i++)
	{
		for(int j=0;j<size;j++)
		{
			result[i]+=a->GetValue(i+1,j+1)*b[j];
		}
	}
	return result;
}
}