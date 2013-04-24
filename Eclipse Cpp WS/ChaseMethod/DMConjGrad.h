#pragma once
#include "DiagMatrix.h"
/*
 * 针对对角矩阵的共轭梯度法，函数对象
 */
class DMConjGrad
{
public:
	DMConjGrad();
	~DMConjGrad(void);
	void ReadInMatrix();	//读入方程组信息
	//void DoSolving();
	void GiveResult();		//输出解
	void operator()();		//函数对象的()运算符，进行共个梯度求解方程
private:
	int n_;		//方程规模
	double *b,*x;	//Ax=b
	double eps;		//迭代精度
	DiagMatrix *A;
};

