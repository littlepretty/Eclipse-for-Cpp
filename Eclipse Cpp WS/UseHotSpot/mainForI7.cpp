///*
// * mainForI7.cpp
// *
// *  Created on: 2012-6-12
// *      Author: pc5
// */
//
///*
// * main.c
// *
// *  Created on: May 31, 2012
// *      Author: yang
// */
//
//#include "WriteFunction.h"
//#include "flp.h"
//#include "temperature.h"
//#include <time.h>
//
//int main()
//{
//	int Number=22;
//
//	char *flp_file="i7x.flp";
//
//	char *steady_temp_file="i7x.steady";
//
//    flp_t *flp;
//    RC_model_t *model;
//    thermal_config_t config;
//    double *power, *temp;
//
//    flp = read_flp(flp_file, 0);
//    config = yjq_thermal_config();
//    model = alloc_RC_model(&config, flp);
//    temp = hotspot_vector(model);
//    power = hotspot_vector(model);
//
//	populate_R_model(model,flp);
//	populate_C_model(model,flp);
//
//
////    char *random_file="RandomISeven.ptrace";
////    config.leakage_used=0;
////    WriteRandomPowerTraceForISeven(Number,random_file);
////    read_power(model, power, random_file);
////
////    double span=0.0;
////    int ExeTime;
////    for(ExeTime=0;ExeTime<100;ExeTime++)
////    {
////		clock_t startTime=clock();
////		steady_state_temp(model,power,temp);
////		dump_temp(model, temp, steady_temp_file);
////		span+=(clock()-startTime)/((double)CLOCKS_PER_SEC);
////    }
////    printf("used time is %lf",span);
//
//    char *power_file="i7x.ptrace";
//    int i;
//    double span=0.0;
//    for(i=0;i<Number;i++)
//    {
//		WriteNormalPowerTraceForISeven(Number,i+1,power_file);
//
//		read_power(model, power, power_file);
//
//		clock_t startTime=clock();
//
//		steady_state_temp(model,power,temp);
//
//		dump_temp(model, temp, steady_temp_file);
//
//		span+=(clock()-startTime)/((double)CLOCKS_PER_SEC);
//
//		ExtractTempForISeven(steady_temp_file,Number,i+1);
//    }
//
//    printf("used time is %lf",span);
//
//    delete_RC_model(model);
//    free_flp(flp, 0);
//    free_dvector(temp);
//    free_dvector(power);
//
//	return 0;
//
//}
//
//
