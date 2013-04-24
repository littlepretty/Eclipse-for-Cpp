/*
 * main.c
 *
 *  Created on: May 31, 2012
 *      Author: yang
 */

#include "WriteFunction.h"
#include "flp.h"
#include "temperature.h"
#include "temperature_block.h"
#include <stdio.h>
#include <string.h>
#include <time.h>



int main()
{
	/*
	 * forget what this is for...
	 */
/*
	int Number=64;

	WriteFloorPlan(Number,L2,L2_Left,L2_Right,Core);

	char *flp_file="SixteenCore.flp";

	char *steady_temp_file="SixteenCore.steady";

    flp_t *flp;
    RC_model_t *model;
    thermal_config_t config;
    double *power, *temp;

    flp = read_flp(flp_file, 0);
    config = yjq_thermal_config();
    model = alloc_RC_model(&config, flp);
    temp = hotspot_vector(model);
    power = hotspot_vector(model);

	populate_R_model(model,flp);
	populate_C_model(model,flp);

    int i=3;
    char *power_file="SixteenCore.ptrace";
    for(i=0;i<Number;i++)
    {
		WriteNormalPowerTrace(Number,L2,L2_Left,L2_Right,Core,i+1);
		read_power(model, power, power_file);

		steady_state_temp(model,power,temp);

		dump_temp(model, temp, steady_temp_file);

		ExtractTemp(steady_temp_file,Number,i+1);
    }

    delete_RC_model(model);
    free_flp(flp, 0);
    free_dvector(temp);
    free_dvector(power);

*/


/**************************************************************************************************************/
	/*
	 * for analysis of transient temperature,obtain raw data to extract R and tau parameters
	 */

/*
	char *flp_file="SixteenCore.flp";
	char *init_temp_file="SixteenCoreInit.txt";

	FILE *trace;

    flp_t *flp;
    RC_model_t *model;
    thermal_config_t config;
    double *power, *temp;
    double delta_time=1e-2;

	int TraceNumber=10000;
	int ModuleNumber=64;
	char *numbers[1]={"1.txt"};//,"2.txt","3.txt","4.txt","5.txt","6.txt","7.txt","8.txt","9.txt","10.txt","11.txt","12.txt","13.txt","14.txt","15.txt","16.txt","17.txt","18.txt","19.txt","20.txt","21.txt","22.txt","23.txt","24.txt","25.txt","26.txt","27.txt","28.txt","29.txt","30.txt","31.txt","32.txt","33.txt","34.txt","35.txt","36.txt","37.txt","38.txt","39.txt","40.txt","41.txt","42.txt","43.txt","44.txt","45.txt","46.txt","47.txt","48.txt","49.txt","50.txt","51.txt","52.txt","53.txt","54.txt","55.txt","56.txt","57.txt","58.txt","59.txt","60.txt","61.txt","62.txt","63.txt","64.txt",};
	int on,sample;
	double* iter;

	//hotspot configuration: see yjq_thermal_config
    flp = read_flp(flp_file, 0);
    config = yjq_thermal_config();
    model = alloc_RC_model(&config, flp);
    temp = hotspot_vector(model);
    power = hotspot_vector(model);
	populate_R_model(model,flp);
	populate_C_model(model,flp);

	clock_t startTime=clock();
	for(on=0;on<1;on++)
	{
		//cat the filenames
		char power_file[40]="ModuleIsOn";
		char steady_temp_file[40]="TemSteadyIsOn";
		char trace_temp_file[40]="TemTraceIsOn";
		strcat(power_file,numbers[on]);
		strcat(steady_temp_file,numbers[on]);
		strcat(trace_temp_file,numbers[on]);

		read_power(model, power, power_file);

		//compute steady temperature
		read_temp(model,temp,init_temp_file,FALSE);
		steady_state_temp(model,power,temp);
		dump_temp(model, temp, steady_temp_file);


		//interate transient temperature
		trace=fopen(trace_temp_file,"w+");
		read_temp(model,temp,init_temp_file,FALSE);
		for(sample=0;sample<TraceNumber;sample++)
		{
			for(iter=temp;iter<temp+64;iter++)
			{
				fprintf(trace,"%.4f\t",*iter-273.15);
			}
			fprintf(trace,"\n");

			compute_temp(model,power,temp,delta_time);
		}
	}
	double timeUsed=(clock()-startTime)/(double)CLOCKS_PER_SEC;
	printf("%.4f",timeUsed);

	//clean environment
	fclose(trace);

    delete_RC_model(model);
    free_flp(flp, 0);
    free_dvector(temp);
    free_dvector(power);
*/


	/*
	 * give every block a PhraseStep power trace, get a temperature trace
	 */


	#define Generate_New_Random_Power_Trace 1
	#define Run_Steady_Test 1
	#define Run_Transient_Test 0

	if(Generate_New_Random_Power_Trace)
		WriteRandomPowerForSixteenCore();

  	char *flp_file="SixteenCore.flp";
	char *init_temp_file="SixteenCoreInit.txt";

	FILE *trace;
	FILE *powerSteady;

    flp_t *flp;
    RC_model_t *model;
    thermal_config_t config;
    double *power,*dynamicPower, *temp;
    double delta_time=1e-6;

	int TraceNumber=8000;
	int sample;
	int iter;

	char *power_file="NewPowerTrace.txt";
	char *steady_power_file="PowerSteady.txt";
	//char *steady_temp_file="GccSteady.txt";
	char *steady_temp_file_PT="GccSteadyConsiderPT.txt";
	char *trace_temp_file="GccTrace.txt";


	//hotspot configuration: see yjq_thermal_config
	flp = read_flp(flp_file, 0);


	//compute steady temperature
//	if(Run_Steady_Test)
//	{
//		read_temp(model,temp,init_temp_file,FALSE);
//		steady_state_temp(model,power,temp);
//		dump_temp(model, temp, steady_temp_file);
//	}


	//compute steady temperature with/without considering of P & T relationship (decided by yjq_thermal_config)
	if(Run_Steady_Test)
	{


		clock_t startTime=clock();
		int i;
		for(i=0;i<500;i++)
		{
			if(Generate_New_Random_Power_Trace)
				WriteRandomPowerForSixteenCore();

			config=yjq_thermal_config_withPT();
			model = alloc_RC_model(&config, flp);
			temp = hotspot_vector(model);
			power = hotspot_vector(model);
			populate_R_model(model,flp);
			populate_C_model(model,flp);

			read_power(model, power, power_file);

			read_temp(model,temp,init_temp_file,FALSE);

			steady_state_temp(model,power,temp);
		}
		double timeUsed=(clock()-startTime)/(double)CLOCKS_PER_SEC;
		printf("Steady Analysis Used Time Is %.6f",timeUsed);
		dump_temp(model,temp,steady_temp_file_PT);
	}


	//interate transient temperature
	if(Run_Transient_Test)
	{
		config = yjq_thermal_config();
		model = alloc_RC_model(&config, flp);
		temp = hotspot_vector(model);
		power = hotspot_vector(model);
		dynamicPower=hotspot_vector(model);
		populate_R_model(model,flp);
		populate_C_model(model,flp);

		read_power(model, power, power_file);
		read_power(model, dynamicPower, power_file);

		trace=fopen(trace_temp_file,"w+");
		powerSteady=fopen(steady_power_file,"w+");
		read_temp(model,temp,init_temp_file,FALSE);
		clock_t startTime=clock();
		for(sample=0;sample<TraceNumber;sample++)
		{
			for(iter=0;iter<64;iter++)
			{
				fprintf(trace,"%.4f\t",temp[iter]-273.15);
				fprintf(powerSteady,"%.4f\t",power[iter]);
			}
			fprintf(trace,"\n");
			fprintf(powerSteady,"\n");

			compute_temp(model,power,temp,delta_time);
			//whether consider leakage power or not
			//add_leakage_power(model,power,dynamicPower,temp);
		}
		double timeUsed=(clock()-startTime)/(double)CLOCKS_PER_SEC;
		fprintf(powerSteady,"\n#transient analysis uses %g sec.\n",timeUsed);
		printf("\n transient analysis uses %g sec.\n",timeUsed);
		fclose(trace);
		fclose(powerSteady);
	}




	//All done, clean environment

	delete_RC_model(model);
	free_flp(flp, 0);
	free_dvector(temp);
	free_dvector(power);





	/*
	 * give every block a series of random power trace, get temperature trace sequence
	 */

/*
  	char *flp_file="SixteenCore.flp";
	char *init_temp_file="SixteenCoreInit.txt";

	FILE *trace;
	//FILE *powerSteady;
	FILE *randomRecord;

	//FILE *randomRecordWithLeakage;

    flp_t *flp;
    RC_model_t *model;
    thermal_config_t config;
    double *power,*dynamicPower, *temp;
    double delta_time=1e-2;

    //char *trace_c="trace_c.txt";
    //char *trace_r="trace_r.txt";


	int TraceNumber=100;
	int InnerTraceNumber=100;
	int sample,innerSample;
	int iter;

	char *power_file="PowerTraceSeries.txt";
	//char *steady_power_file="PowerSteadySeries.txt";
	char *trace_temp_file="TemTraceSeries.txt";
	char *random_power_record="PowerTraceRecord.txt";
	//char *steady_temp_filename="GccSteadyConsiderPT.txt";
	//char *random_power_record_with_leakage="PowerTraceRecordWithLeakage.txt";

	//hotspot configuration: see yjq_thermal_config
	flp = read_flp(flp_file, 0);

	//interate transient temperature
	config = yjq_thermal_config();
	model = alloc_RC_model(&config, flp);
	temp = hotspot_vector(model);
	power = hotspot_vector(model);
	dynamicPower=hotspot_vector(model);
	populate_R_model(model,flp);
	populate_C_model(model,flp);


	trace=fopen(trace_temp_file,"w+");
	//powerSteady=fopen(steady_power_file,"w+");
	randomRecord=fopen(random_power_record,"w+");

	//randomRecordWithLeakage=fopen(random_power_record_with_leakage,"w+");
	read_temp(model,temp,init_temp_file,FALSE);
	clock_t startTime=clock();
	int seed=10;
	for(sample=0;sample<TraceNumber;sample++)
	{
		WriteRandomPowerForSixteenCoreWithSeed(seed);
		seed+=100;
		read_power(model, power, power_file);
		read_power(model, dynamicPower, power_file);
		for(iter=0;iter<64;iter++)
		{
			fprintf(randomRecord,"%.4f\t",power[iter]);
		}
		fprintf(randomRecord,"\n");

		//add_leakage_power(model,power,dynamicPower,temp);

//		for(iter=0;iter<64;iter++)
//		{
//			fprintf(randomRecordWithLeakage,"%.4f\t",power[iter]);
//		}
//		fprintf(randomRecordWithLeakage,"\n");

		for(innerSample=0;innerSample<InnerTraceNumber;innerSample++)
		{
			for(iter=0;iter<64;iter++)
			{
				fprintf(trace,"%.4f\t",temp[iter]-273.15);
				//fprintf(powerSteady,"%.4f\t",power[iter]);
			}
			fprintf(trace,"\n");
			//fprintf(powerSteady,"\n");

			compute_temp(model,power,temp,delta_time);
			//whether consider leakage power or not
			//add_leakage_power(model,power,dynamicPower,temp);
		}
	}
	double timeUsed=(clock()-startTime)/(double)CLOCKS_PER_SEC;
	//fprintf(powerSteady,"\n#transient analysis uses %g sec.\n",timeUsed);
	printf("\n transient analysis uses %g sec.\n",timeUsed);
	//fclose(randomRecordWithLeakage);
	fclose(randomRecord);
	fclose(trace);

	//steady_state_temp(model,power,temp);
	//dump_temp(model,temp,steady_temp_filename);

	//printRC( trace_c, model, trace_r);

	//fclose(powerSteady);

	//All done, clean environment
	delete_RC_model(model);
	free_flp(flp, 0);
	free_dvector(temp);
	free_dvector(power);

*/


	/*
	 * give every block a SIN power trace, see the process of temperature raise
	 */
/*

	#define Generate_New_Random_Power_Trace 1
	#define Run_Steady_Test 0
	#define Run_Transient_Test 1

	if(Generate_New_Random_Power_Trace)
	{
		int i;
		for(i=0;i<1000;i++)
		{
			WriteSINPowerForSixteenCore(1,0.01*i,1);
		}
	}


  	char *flp_file="SixteenCore.flp";
	char *init_temp_file="SixteenCoreInit.txt";

	FILE *trace;
	FILE *powerSteady;

    flp_t *flp;
    RC_model_t *model;
    thermal_config_t config;
    double *power,*dynamicPower, *temp;
    double delta_time=1e-2;

	int TraceNumber=5000;
	int sample;
	int iter;

	char *power_file="SINPowerTrace.txt";
	char *steady_power_file="SINPowerSteady.txt";
	char *steady_temp_file="SINSteady.txt";
	char *steady_temp_file_PT="SINSteadyConsiderPT.txt";
	char *trace_temp_file="SINTemTrace.txt";


	//hotspot configuration: see yjq_thermal_config
	flp = read_flp(flp_file, 0);


	//compute steady temperature
	if(Run_Steady_Test)
	{
		read_temp(model,temp,init_temp_file,FALSE);
		steady_state_temp(model,power,temp);
		dump_temp(model, temp, steady_temp_file);
	}


	//compute steady temperature with/without considering of P & T relationship (decided by yjq_thermal_config)
	if(Run_Steady_Test)
	{
		config=yjq_thermal_config_withPT();
		model = alloc_RC_model(&config, flp);
		temp = hotspot_vector(model);
		power = hotspot_vector(model);
		populate_R_model(model,flp);
		populate_C_model(model,flp);

		read_power(model, power, power_file);

		read_temp(model,temp,init_temp_file,FALSE);
		steady_state_temp(model,power,temp);
		dump_temp(model,temp,steady_temp_file_PT);
	}


	//interate transient temperature
	if(Run_Transient_Test)
	{
		config = yjq_thermal_config();
		model = alloc_RC_model(&config, flp);
		temp = hotspot_vector(model);
		power = hotspot_vector(model);
		dynamicPower=hotspot_vector(model);
		populate_R_model(model,flp);
		populate_C_model(model,flp);

		trace=fopen(trace_temp_file,"w+");
		powerSteady=fopen(steady_power_file,"w+");
		read_temp(model,temp,init_temp_file,FALSE);
		clock_t startTime=clock();

		for(sample=0;sample<TraceNumber;sample++)
		{
			WriteSINPowerForSixteenCore(1,delta_time*sample,1);

			read_power(model, power, power_file);
			read_power(model, dynamicPower, power_file);

			for(iter=0;iter<64;iter++)
			{
				fprintf(trace,"%.4f\t",temp[iter]-273.15);
				fprintf(powerSteady,"%.4f\t",power[iter]);
			}
			fprintf(trace,"\n");
			fprintf(powerSteady,"\n");

			compute_temp(model,power,temp,delta_time);
			//whether consider leakage power or not
			//add_leakage_power(model,power,dynamicPower,temp);
		}

		double timeUsed=(clock()-startTime)/(double)CLOCKS_PER_SEC;
		fprintf(powerSteady,"\n#transient analysis uses %g sec.\n",timeUsed);
		printf("\n transient analysis uses %g sec.\n",timeUsed);
		fclose(trace);
		fclose(powerSteady);
	}

	//All done, clean environment
	if(Run_Steady_Test || Run_Transient_Test)
	{
		delete_RC_model(model);
		free_flp(flp, 0);
		free_dvector(temp);
		free_dvector(power);
	}

*/

	return 0;

}


