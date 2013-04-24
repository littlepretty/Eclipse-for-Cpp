#include <cmath>
#include <iostream>

template <typename T>
void swap(T& l,T& r)
{
	T tmp;
	tmp=l;
	l=r;
	r=tmp;
}

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

double** MatrixProduct(double **a,double **b,int size_a,int size_ab,int size_b)
{
	double **result=new double*[size_a];
	for(int i=0;i<size_a;i++)
	{
		result[i]=new double[size_b];
		for(int j=0;j<size_b;j++)
		{
			result[i][j]=0.0;
			for(int k=0;k<size_ab;k++)
			{
				result[i][j]+=a[i][k]*b[k][j];
			}
			std::cout<<result[i][j]<<" ";
		}
		std::cout<<"\n";

	}
	std::cout<<"Over MatrixProduct!\n";
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


double** Inverse(double **A,int n)
{
	int *main_i=new int[n];
	int *main_j=new int[n];
	for(int k=0;k<n;k++)
	{
		double max=0,tmp;
		for(int i=k;i<n;i++)
		{
			for(int j=k;j<n;j++)
			{
				tmp=std::fabs(A[i][j]);
				if(tmp>max)
				{
					max=tmp;
					main_i[k]=i;
					main_j[k]=j;
				}
			}
		}

		if(max+1.0==1.0)
		{
			delete [] main_i;
			delete [] main_j;
			std::cout<<"fail to inverse";
			return A;
		}

		if(main_i[k]!=k)
		{
			for(int j=0;j<n;j++)
			{
				swap(A[k][j],A[main_i[k]][j]);
			}
		}
		if(main_j[k]!=k)
		{
			for(int i=0;i<n;i++)
			{
				swap(A[i][k],A[i][main_i[k]]);
			}
		}

		A[k][k]=1.0/A[k][k];
		for(int j=0;j<n;j++)
		{
			if(j!=k)
				A[k][j]=A[k][j]*A[k][k];
		}
		for(int i=0;i<n;i++)
		{
			if(i!=k)
			{
				for(int j=0;j<n;j++)
				{
					if(j!=k)
						A[i][j]=A[i][j]-A[i][k]*A[k][j];
				}
			}
		}
		for(int i=0;i<n;i++)
		{
			if(i!=k)
				A[i][k]=-A[i][k]*A[k][k];
		}

	}

	for(int k=n-1;k>=0;k--)
	{
		if(main_j[k]!=k)
		{
			for(int i=0;i<n;i++)
			{
				swap(A[k][i],A[main_i[k]][i]);
			}
		}
		if(main_i[k]!=k)
		{
			for(int j=0;j<n;j++)
			{
				swap(A[j][k],A[j][main_i[k]]);
			}
		}
	}
	delete [] main_i;
	delete [] main_j;
	for(int i=0;i<n;i++)
	{
		for(int j=0;j<n;j++)
		{
			std::cout<<A[i][j]<<" ";
		}
		std::cout<<"\n";
	}
	std::cout<<"Over Inverse!\n";
	return A;
}

