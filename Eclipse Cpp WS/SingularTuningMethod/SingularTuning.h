/*
 * SingularTuning.h
 *
 *  Created on: 2012-5-2
 *      Author: pc5
 */

#ifndef SINGULARTUNING_H_
#define SINGULARTUNING_H_
/*
 * 针对求解无约束的n_元极值问题的单形调优法
 */

class SingularTuning {
public:
	SingularTuning(int n);
	virtual ~SingularTuning();
	double TargetFunction(double *var);
	void ReadInfo();		//从Information.txt中读取极值问题的参数
	void Output();		//在文件中及console中打印计算结果
	void InitX();	//Solution函数中要用到的产生初始单形的函数
	void Solution();		//单形调优法的主体
private:
	/*
	 * n_变元个数
	 * 数组：
	 * x_为最小值对应的取值
	 * X_为n_个n_维点，构成单形
	 * f_为X_中的点对应的目标函数值
	 * Xe_为调整（扩张niu或收缩lamta）后的点
	 * Xt_为最坏点的对称点
	 * Xf_为除最坏点的所有点的重心
	 * flag_给出迭代次数，其上限为ForceStop
	 */
	int n_;
	double *x_;
	int ForceStop_;
	double niu,lamta;
	double **X_;
	double *f_;
	double *Xe_,*Xt_,*Xf_;
	double eps_;
	double dist_;
	int flag_;
};

#endif /* SINGULARTUNING_H_ */
