/*
 * InequlityLinearProblem.h
 *
 *  Created on: 2012-4-30
 *      Author: pc5
 */

#ifndef INEQULITYLINEARPROBLEM_H_
#define INEQULITYLINEARPROBLEM_H_
/*
 * ��Խ��m_�����Բ���Լ�������Թ滮����
 */
class InequlityLinearProblem {
public:
	InequlityLinearProblem(int n,int m);
	virtual ~InequlityLinearProblem();
	void ReadInfo();	//��Information.txt�ж�ȡ��ֵ����Ĳ���
	void Solution();	//����
	void Output();		//��console��ӡ������
private:
	bool isConverge;
	int n_,m_;
	int *pick_;
	double **A_;
	double **P_;
	double **D_;
	double *c_;
	double *b_;
	double *x_;
};

#endif /* INEQULITYLINEARPROBLEM_H_ */
