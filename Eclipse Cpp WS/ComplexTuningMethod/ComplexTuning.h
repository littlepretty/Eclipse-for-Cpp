/*
 * ComplexTuning.h
 *
 *  Created on: 2012-5-3
 *      Author: pc5
 */

#ifndef COMPLEXTUNING_H_
#define COMPLEXTUNING_H_
/*
 * ������n_������Լ��������m_������Լ��������n_Ԫ��ֵ����
 */
class ComplexTuning {
public:
	ComplexTuning(int n,int m);
	virtual ~ComplexTuning();
	void ReadInfo();	//��Information.txt�ж�ȡ��ֵ����Ĳ���
	void Output();	//��console��ӡ������
	double GenRand();	//Solution������Ҫ�õ��Ĳ���������ĺ���
	void Solution();	//���ε��ŷ�������
	void FunctionConstrains(double *var);	//����m_������Լ��
	double TargetFunction(double *var);		//��ȡĿ�꺯����ֵ
private:
	/*
	 * n_,m_����Ԫ�����뺯��Լ����������
	 * ���飺
	 * a��b��ű�Ԫ�ĳ���Լ��ֵ
	 * x_Ϊ��Сֵ��Ӧ��ȡֵ
	 * C_,W_,D_Լ����C(x_)<=W_(x_)<=D_(x_)
	 * X_Ϊ2n_��n_ά�㣬���ɸ���
	 * f_ΪX_�еĵ��Ӧ��Ŀ�꺯��ֵ
	 * Xt_Ϊ���ĶԳƵ�
	 * Xf_Ϊ���������е������
	 * T_Ϊǰj��������������Լ���ĵ������
	 * flag_��������������������ΪForceStop
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
