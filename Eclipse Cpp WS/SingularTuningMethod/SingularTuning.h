/*
 * SingularTuning.h
 *
 *  Created on: 2012-5-2
 *      Author: pc5
 */

#ifndef SINGULARTUNING_H_
#define SINGULARTUNING_H_
/*
 * ��������Լ����n_Ԫ��ֵ����ĵ��ε��ŷ�
 */

class SingularTuning {
public:
	SingularTuning(int n);
	virtual ~SingularTuning();
	double TargetFunction(double *var);
	void ReadInfo();		//��Information.txt�ж�ȡ��ֵ����Ĳ���
	void Output();		//���ļ��м�console�д�ӡ������
	void InitX();	//Solution������Ҫ�õ��Ĳ�����ʼ���εĺ���
	void Solution();		//���ε��ŷ�������
private:
	/*
	 * n_��Ԫ����
	 * ���飺
	 * x_Ϊ��Сֵ��Ӧ��ȡֵ
	 * X_Ϊn_��n_ά�㣬���ɵ���
	 * f_ΪX_�еĵ��Ӧ��Ŀ�꺯��ֵ
	 * Xe_Ϊ����������niu������lamta����ĵ�
	 * Xt_Ϊ���ĶԳƵ�
	 * Xf_Ϊ���������е������
	 * flag_��������������������ΪForceStop
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
