#pragma once
#include <fstream>
/*
 * ��һά����ѹ���洢����Ϊband�ĶԽǾ���
 */
class DiagMatrix
{
public:
	DiagMatrix();
	~DiagMatrix(void);
	void ReadInData();	//�������
	int Location(int i,int j);	//���ݾ��������ij��λDM_�е�λ��
	bool isZero(int i,int j);	//���ݾ��������ij�ж��Ƿ�Ϊband�ڵķ���Ԫ��
	double GetValue(int i,int j);	//���ݾ��������ijȡ��DM_�е�ֵ
	double operator [](int index);	//���ضԽǾ�������������������DM_�е�Ԫ��
	//void ScanMatrixToDM(std::ifstream &in);
	int GetN(){return n_;};		//��ȡ�ԽǾ����ά��
	int GetBand(){return band;};	//��ȡ�ԽǾ���Ĵ���
private:
	int band;	//����
	int n_;		//ά��
	int Capacity;		//����Ԫ�صĸ�����DM_������
	double *DM_;		//һλ������ѹ���ľ���
};

