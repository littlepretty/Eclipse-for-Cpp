#pragma once
#include "DiagMatrix.h"
/*
 * ��ԶԽǾ���Ĺ����ݶȷ�����������
 */
class DMConjGrad
{
public:
	DMConjGrad();
	~DMConjGrad(void);
	void ReadInMatrix();	//���뷽������Ϣ
	//void DoSolving();
	void GiveResult();		//�����
	void operator()();		//���������()����������й����ݶ���ⷽ��
private:
	int n_;		//���̹�ģ
	double *b,*x;	//Ax=b
	double eps;		//��������
	DiagMatrix *A;
};

