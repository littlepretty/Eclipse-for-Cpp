/*
 * InequlityLinearProblem.h
 *
 *  Created on: 2012-4-30
 *      Author: pc5
 */

#ifndef INEQULITYLINEARPROBLEM_H_
#define INEQULITYLINEARPROBLEM_H_
/*
 * 针对解决m_个线性不等约束的线性规划问题
 */
class InequlityLinearProblem {
public:
	InequlityLinearProblem(int n,int m);
	virtual ~InequlityLinearProblem();
	void ReadInfo();	//从Information.txt中读取极值问题的参数
	void Solution();	//主体
	void Output();		//在console打印计算结果
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
