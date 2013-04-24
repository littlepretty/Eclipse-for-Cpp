/*
 * Data.h
 *
 * @brief 定义芯片的尺寸、结构以及热阻、热容等热参数；提供了：读取功耗文件的函数，显示多核向量结果的函数，计算并联电阻的函数
 * @date 2012-4-9
 * @author pc5
 */


#include <vector>
#include <iomanip>
#include <fstream>
#include <cstring>
/* @brief
 * A：芯片面积
 * h_si h_cu 硅衬底和铜散热片的厚度
 * gRatio为热导率（W/（cm*K））
 * cRatio为比热容（J/（kg*K））
 * mRatio为密度（kg/cm3）
 * SumPower为总功耗密度（W/cm2）
 */
const double A(4.35);
const double h_si(5e-2);
const double h_cu(1);

const double gRatio_si(1);
const double cRatio_si(171);
const double mRatio_si(2.32e-3);

const double gRatio_cu(4);
const double cRatio_cu(386);
const double mRatio_cu(8.92e-3);

double SumPower;

//铜导热层的热容、热阻参数
const double r_conduct=h_cu/gRatio_cu;
const double c_conduct=cRatio_cu*mRatio_cu*h_cu;

//硅衬底的热容、热阻参数
const double r_bulk=h_si/gRatio_si;
const double c_bulk=cRatio_si*mRatio_si*h_si;

//散热系统的热阻参数
const double r_system=0.146*A;
const double r_fan=r_system-r_bulk-r_conduct;

//粗略估计RC充电时间
const double RCtime=(r_bulk*r_conduct/(r_bulk+r_conduct))*(c_bulk+c_conduct);
const double h=0.1*RCtime;//分隔时间片
const double Tamb=0;
std::vector<double> CorePower;

int CoreNumber;
int IterNumber(5000);
int SetWidth(12);
int SetPrecision(8);

/*
 * @brief
 * 格式化输出以@param name为名字的向量@param v
 */
void DisplayVector(std::vector<double> v, char* name)
{
	char dis[20];
	strcpy(dis,name);
	strcat(dis,".txt");
	std::ofstream out(dis);

	if(not out)
		std::perror(dis);

	out.precision(SetPrecision);

	for(int i=0;i<CoreNumber;i++)
		out<<std::setw(SetWidth-3)<<std::right<<name<<"("<<i+1<<")";
	out<<std::endl;

	for(std::vector<double>::iterator i=v.begin();i!=v.end();i++)
		out<<std::setw(SetWidth)<<*i;
	out<<std::endl;

	out.close();
}


/*
 * @brief 计算两个电阻@param r1与@param r2的并联值
 */
double Pararlle(double r1,double r2)
{
	return r1*r2/(r1+r2);
}

/*
 * @brief 从文件@param PowerFile中读取功率等有关信息
 * 1. 核数@param CoreNumber
 * 2. 总功耗@param SumPower
 * 3. 每个核上的功耗@param CorePower=功耗的分配比例*总功耗@param SumPower
 */
void GetPower(char* PowerFile)
{
	std::ifstream in;
	in.open(PowerFile);
	in>>CoreNumber>>SumPower;
	SumPower=SumPower/A;

	std::vector<double> distribution;
	double tmp;
	for(int i=0;i<CoreNumber;i++)
	{
		in>>tmp;
		CorePower.push_back(SumPower*tmp);//初始化核中的功率
	}
	in.close();
}


