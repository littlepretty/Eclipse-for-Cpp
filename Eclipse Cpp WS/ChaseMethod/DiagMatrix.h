#pragma once
#include <fstream>
/*
 * 用一维数组压缩存储带宽为band的对角矩阵
 */
class DiagMatrix
{
public:
	DiagMatrix();
	~DiagMatrix(void);
	void ReadInData();	//读入矩阵
	int Location(int i,int j);	//根据矩阵的行列ij定位DM_中的位置
	bool isZero(int i,int j);	//根据矩阵的行列ij判断是否为band内的非零元素
	double GetValue(int i,int j);	//根据矩阵的行列ij取得DM_中的值
	double operator [](int index);	//重载对角矩阵的索引运算符，返回DM_中的元素
	//void ScanMatrixToDM(std::ifstream &in);
	int GetN(){return n_;};		//获取对角矩阵的维度
	int GetBand(){return band;};	//获取对角矩阵的带宽
private:
	int band;	//带宽
	int n_;		//维度
	int Capacity;		//非零元素的个数，DM_的容量
	double *DM_;		//一位数组存放压缩的矩阵
};

