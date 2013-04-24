/*
 * WriteFunction.h
 *
 *  Created on: May 31, 2012
 *      Author: yang
 */

/*
 * WriteFunctions.h
 *
 *  Created on: May 31, 2012
 *      Author: yang
 */

#ifndef WRITEFUNCTIONS_H_
#define WRITEFUNCTIONS_H_

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include  "temperature.h"
#include "temperature_block.h"


char* L2="L2_";
char* L2_Left="L2_Left_";
char* L2_Right="L2_Right_";
char* Core="Core_";


void WriteFloorPlan(const int Number,char*  L2,char*  L2_Left,char*  L2_Right,char*  Core)
{
	double L2_l,L2_w;
	L2_l=L2_w=0.005;
	double L2_R_l,L2_R_w;
	L2_R_l=0.003;
	L2_R_w=0.005;
	double L2_L_l,L2_L_w;
	L2_L_l=0.005;
	L2_L_w=0.003;
	double Core_l,Core_w;
	Core_l=Core_w=0.003;

	double BlockPoint[Number][2];//=new double[Number][2];
	BlockPoint[0][0]=0;
	BlockPoint[0][1]=0;
	BlockPoint[1][0]=0.005;
	BlockPoint[1][1]=0;
	BlockPoint[2][0]=0.005;
	BlockPoint[2][1]=0.005;
	BlockPoint[3][0]=0;
	BlockPoint[3][1]=0.005;

	int i;
	for(i=4;i<=12;i=i+4)
	{
		BlockPoint[i][0]=BlockPoint[i-4][0];
		BlockPoint[i][1]=BlockPoint[i-4][1]+0.008;
		BlockPoint[i+1][0]=BlockPoint[i-4+1][0];
		BlockPoint[i+1][1]=BlockPoint[i-4+1][1]+0.008;
		BlockPoint[i+2][0]=BlockPoint[i-4+2][0];
		BlockPoint[i+2][1]=BlockPoint[i-4+2][1]+0.008;
		BlockPoint[i+3][0]=BlockPoint[i-4+3][0];
		BlockPoint[i+3][1]=BlockPoint[i-4+3][1]+0.008;
	}

	for(i=16;i<=28;i=i+4)
	{
		BlockPoint[i][0]=BlockPoint[i-16][0]+0.008;
		BlockPoint[i][1]=BlockPoint[i-16][1];
		BlockPoint[i+1][0]=BlockPoint[i-16+1][0]+0.008;
		BlockPoint[i+1][1]=BlockPoint[i-16+1][1];
		BlockPoint[i+2][0]=BlockPoint[i-16+2][0]+0.008;
		BlockPoint[i+2][1]=BlockPoint[i-16+2][1];
		BlockPoint[i+3][0]=BlockPoint[i-16+3][0]+0.008;
		BlockPoint[i+3][1]=BlockPoint[i-16+3][1];
	}

	for(i=32;i<=44;i=i+4)
	{
		BlockPoint[i][0]=BlockPoint[i-16][0]+0.008;
		BlockPoint[i][1]=BlockPoint[i-16][1];
		BlockPoint[i+1][0]=BlockPoint[i-16+1][0]+0.008;
		BlockPoint[i+1][1]=BlockPoint[i-16+1][1];
		BlockPoint[i+2][0]=BlockPoint[i-16+2][0]+0.008;
		BlockPoint[i+2][1]=BlockPoint[i-16+2][1];
		BlockPoint[i+3][0]=BlockPoint[i-16+3][0]+0.008;
		BlockPoint[i+3][1]=BlockPoint[i-16+3][1];
	}

	for(i=48;i<=60;i=i+4)
	{
		BlockPoint[i][0]=BlockPoint[i-16][0]+0.008;
		BlockPoint[i][1]=BlockPoint[i-16][1];
		BlockPoint[i+1][0]=BlockPoint[i-16+1][0]+0.008;
		BlockPoint[i+1][1]=BlockPoint[i-16+1][1];
		BlockPoint[i+2][0]=BlockPoint[i-16+2][0]+0.008;
		BlockPoint[i+2][1]=BlockPoint[i-16+2][1];
		BlockPoint[i+3][0]=BlockPoint[i-16+3][0]+0.008;
		BlockPoint[i+3][1]=BlockPoint[i-16+3][1];
	}

	FILE *flp;
	flp=fopen("SixteenCore.flp","w+");
	int k=1;
	for(i=0;i<Number;i=i+4)
	{
		fprintf(flp,"%s%d\t%f\t%f\t%f\t%f\n",L2,k,L2_l,L2_w,BlockPoint[i][0],BlockPoint[i][1]);

		fprintf(flp,"%s%d\t%f\t%f\t%f\t%f\n",L2_Right,k,L2_R_l,L2_R_w,BlockPoint[i+1][0],BlockPoint[i+1][1]);

		fprintf(flp,"%s%d\t%f\t%f\t%f\t%f\n",Core,k,Core_l,Core_w,BlockPoint[i+2][0],BlockPoint[i+2][1]);

		fprintf(flp,"%s%d\t%f\t%f\t%f\t%f\n",L2_Left,k,L2_L_l,L2_L_w,BlockPoint[i+3][0],BlockPoint[i+3][1]);

		k=k+1;

	}
	fclose(flp);
}


void WriteNormalPowerTrace(const int Number,char*  L2,char*  L2_Left,char*  L2_Right,char*  Core,int position)
{
	FILE *ptrace;
	ptrace=fopen("SixteenCore.ptrace","w+");

	int i;
//	for(i=0;i<16;i++)
//	{
//		fprintf(ptrace,"%s%d\t",L2,i+1);
//
//		fprintf(ptrace,"%s%d\t",L2_Right,i+1);
//
//		fprintf(ptrace,"%s%d\t",Core,i+1);
//
//		fprintf(ptrace,"%s%d\t",L2_Left,i+1);
//	}
//	fprintf(ptrace,"\n");

	double power[Number];
	double normal[]={12.0,8.5,25.0,8.5};
	int k;
	char *name;
	fprintf(ptrace,"#Power profile=> when block_%d is on\n",position);
	for(k=0;k<1;k++)
	{
		for(i=0;i<Number;i++)
		{

			switch ((i+1)%4)
			{
				case 1:
					name=L2;
					break;
				case 2:
					name=L2_Right;
					break;
				case 3:
					name=Core;
					break;
				case 0:
					name=L2_Left;
					break;
				default:
					break;
			}

			//position is from 1 to 64 whereas i is from 0 to 63
			if(i==position-1)
			{
				int index=i%4;
				power[i]=normal[index];
			}
			else
			{
				power[i]=0;
			}
			int y=i/4+1;
			fprintf(ptrace,"%s%d\t%lf\n",name,y,power[i]);
		}
	}
	fprintf(ptrace,"\n");
	fclose(ptrace);

}

void ExtractTemp(char* steady_temp_file,int Number,int onWhich)
{
	FILE *extract;
	extract=fopen("ExtractedResult.txt","a+");
	FILE *origin;
	origin=fopen(steady_temp_file,"r");
	char str[50];
	double value;
	int i;
	fprintf(extract,"Temperature profile=> when block_%d is on\n",onWhich);
	if(extract!=NULL && origin!=NULL)
	{
		for(i=0;i<Number;i++)
		{
			fscanf(origin,"%s%lf",str,&value);
			//printf("%s",str);
			fprintf(extract,"%s\t%lf\n",str,value);
		}
	}
	fprintf(extract,"\n");
	fclose(extract);
	fclose(origin);

}


/*void WriteRandomPowerForSixteenCore(int seed)
{
	const int CoreNumber = 16;

	FILE *powerFile;
	powerFile=fopen("PowerTraceSeries.txt","w+"); // for stable power trace: NewPowerTrace
												//	for series power trace: PowerTraceSeries
	srand(seed);
	int i,j;
	double temp;
	for(i=1;i<=CoreNumber;i++)
	{
		for(j=1;j<=4;j++)
		{
			switch(j%4)
			{
			case 0:
				temp=rand()/(double)(RAND_MAX);
				temp=temp*(4-1.5)+1.5;
				fprintf(powerFile,"L2_Left_%d\t%.6f\n",i,temp);
				break;
			case 1:
				temp=rand()/(double)(RAND_MAX);
				temp=temp*(6-2.5)+2.5;
				fprintf(powerFile,"L2_%d\t%.6f\n",i,temp);
				break;
			case 2:
				temp=rand()/(double)(RAND_MAX);
				temp=temp*(4-1.5)+1.5;
				fprintf(powerFile,"L2_Right_%d\t%.6f\n",i,temp);
				break;
			case 3:
				temp=rand()/(double)(RAND_MAX);
				temp=temp*(10-6)+6;
				fprintf(powerFile,"Core_%d\t%.6f\n",i,temp);
				break;
			default:
				break;
			}
		}
	}

	fclose(powerFile);
}*/

void WriteRandomPowerForSixteenCore()
{
	const int CoreNumber = 16;

	FILE *powerFile;
	powerFile=fopen("NewPowerTrace.txt","w+"); // for stable power trace: NewPowerTrace
												//	for series power trace: PowerTraceSeries
	srand(time(NULL));
	int i,j;
	double temp;
	for(i=1;i<=CoreNumber;i++)
	{
		for(j=1;j<=4;j++)
		{
			switch(j%4)
			{
			case 0:
				temp=rand()/(double)(RAND_MAX);
				temp=temp*(4-0.5)+0.5;
				fprintf(powerFile,"L2_Left_%d\t%.6f\n",i,temp/2);
				break;
			case 1:
				temp=rand()/(double)(RAND_MAX);
				temp=temp*(6-0.5)+0.5;
				fprintf(powerFile,"L2_%d\t%.6f\n",i,temp/2);
				break;
			case 2:
				temp=rand()/(double)(RAND_MAX);
				temp=temp*(4-0.5)+0.5;
				fprintf(powerFile,"L2_Right_%d\t%.6f\n",i,temp/2);
				break;
			case 3:
				temp=rand()/(double)(RAND_MAX);
				temp=temp*(10-4)+4;
				fprintf(powerFile,"Core_%d\t%.6f\n",i,temp/2);
				break;
			default:
				break;
			}
		}
	}

	fclose(powerFile);
}


void WriteRandomPowerForSixteenCoreWithSeed(int seed)
{
	const int CoreNumber = 16;

	FILE *powerFile;
	powerFile=fopen("PowerTraceSeries.txt","w+"); // for stable power trace: NewPowerTrace
												//	for series power trace: PowerTraceSeries
	srand(seed);
	int i,j;
	double temp;
	for(i=1;i<=CoreNumber;i++)
	{
		//temp=rand()/(double)(RAND_MAX);
		for(j=1;j<=4;j++)
		{
			switch(j%4)
			{

			case 0:
				temp=rand()/(double)(RAND_MAX);
				temp=temp*(4-1.5)+1.5;
				fprintf(powerFile,"L2_Left_%d\t%.6f\n",i,temp/2);
				break;
			case 1:
				temp=rand()/(double)(RAND_MAX);
				temp=temp*(6-2.5)+2.5;
				fprintf(powerFile,"L2_%d\t%.6f\n",i,temp/2);
				break;
			case 2:
				temp=rand()/(double)(RAND_MAX);
				temp=temp*(4-1.5)+1.5;
				fprintf(powerFile,"L2_Right_%d\t%.6f\n",i,temp/2);
				break;
			case 3:
				temp=rand()/(double)(RAND_MAX);
				temp=temp*(10-6)+6;
				fprintf(powerFile,"Core_%d\t%.6f\n",i,temp/2);
				break;
			default:
				break;
			}
		}
	}

	fclose(powerFile);
}


void printRC(char* trace_c, RC_model_t* model, char* trace_r)
{
	int i;
	double C;
	double R;
	FILE* fp1 = fopen(trace_c, "w+");
	for (i = 0; i < 64; i++) {
		C = model->block->a[i];
		fprintf(fp1, "%f\n", C);
		//	 if((i+1)%4==0)
		//		 fprintf(fp1,"\n");
	}
	fclose(fp1);
	FILE* fp2 = fopen(trace_r, "w+");
	int j;
	for (i = 0; i < 64; i++) {
		for (j = 0; j < 64; j++) {
			R = model->block->b[i][j];
			fprintf(fp2, "%.6f\t", R);
		}
		fprintf(fp2, "\n");
	}
	fclose(fp2);
}


void WriteSINPowerForSixteenCore(int seed,double t,double w)
{
	const int CoreNumber = 16;

	FILE *powerFile;
	FILE *recordPowerFile;

	powerFile=fopen("SINPowerTrace.txt","w+");
	recordPowerFile=fopen("SINRecord.txt","a+");

	if(seed==0)
		srand(time(0));
	else
		srand(seed);

	int i,j;
	double temp;
	for(i=1;i<=CoreNumber;i++)
	{
		for(j=1;j<=4;j++)
		{
			switch(j%4)
			{
			case 0:
				temp=rand()/(double)(RAND_MAX);
				temp=temp*(4-1.5)+1.5;
				temp=temp*(sin(w*t)+1)/4;
				fprintf(powerFile,"L2_Left_%d\t%.6f\n",i,temp);
				fprintf(recordPowerFile,"%.6f\t",temp);
				break;
			case 1:
				temp=rand()/(double)(RAND_MAX);
				temp=temp*(6-2.5)+2.5;
				temp=temp*(sin(w*t)+1)/4;
				fprintf(powerFile,"L2_%d\t%.6f\n",i,temp);
				fprintf(recordPowerFile,"%.6f\t",temp);
				break;
			case 2:
				temp=rand()/(double)(RAND_MAX);
				temp=temp*(4-1.5)+1.5;
				temp=temp*(sin(w*t)+1)/4;
				fprintf(powerFile,"L2_Right_%d\t%.6f\n",i,temp);
				fprintf(recordPowerFile,"%.6f\t",temp);
				break;
			case 3:
				temp=rand()/(double)(RAND_MAX);
				temp=temp*(10-6)+6;
				temp=temp*(sin(w*t)+1)/4;
				fprintf(powerFile,"Core_%d\t%.6f\n",i,temp);
				fprintf(recordPowerFile,"%.6f\t",temp);
				break;
			default:
				break;
			}
		}
	}

	fprintf(recordPowerFile,"\n");
	fclose(powerFile);
	fclose(recordPowerFile);

}




#endif /* WRITEFUNCTIONS_H_ */
