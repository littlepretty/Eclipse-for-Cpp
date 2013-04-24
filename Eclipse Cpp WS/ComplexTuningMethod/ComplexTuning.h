/*
 * ComplexTuning.h
 *
 *  Created on: 2012-5-3
 *      Author: pc5
 */

#ifndef COMPLEXTUNING_H_
#define COMPLEXTUNING_H_
/*
 * 针对求解n_个常量约束条件，m_个函数约束条件的n_元极值问题
 */
class ComplexTuning {
public:
	ComplexTuning(int n,int m);
	virtual ~ComplexTuning();
	void ReadInfo();	//从Information.txt中读取极值问题的参数
	void Output();	//在console打印计算结果
	double GenRand();	//Solution函数中要用到的产生随机数的函数
	void Solution();	//复形调优法的主体
	void FunctionConstrains(double *var);	//定义m_个函数约束
	double TargetFunction(double *var);		//求取目标函数的值
private:
	/*
	 * n_,m_：变元个数与函数约束条件个数
	 * 数组：
	 * a、b存放变元的常数约束值
	 * x_为最小值对应的取值
	 * C_,W_,D_约束：C(x_)<=W_(x_)<=D_(x_)
	 * X_为2n_个n_维点，构成复形
	 * f_为X_中的点对应的目标函数值
	 * Xt_为最坏点的对称点
	 * Xf_为除最坏点的所有点的重心
	 * T_为前j个满足所有条件约束的点的重心
	 * flag_给出迭代次数，其上限为ForceStop
	 */
	int n_,m_;
	double var_;
	double *a_,*b_,*x_;
	double *C_,*W_,*D_;
	double alpha_,eps_;
	double **X_;
	double *f_;
	double *Xt_,*Xf_,*T_;
	int ForceStop_,flag_;
};

#endif /* COMPLEXTUNING_H_ */
