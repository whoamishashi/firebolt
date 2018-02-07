/*
 * mode2.h
 *
 *  Created on: 27 Jan 2018
 *      Author: Felix Hessinger
 */

#ifndef LIB_CONTROL_MODE2_AV_H_
#define LIB_CONTROL_MODE2_AV_H_

#include "rodos.h"
#include "../structs.h"
#include "../topics.h"
#include "../globals.h"




class Mode2: public Thread {
private:
	int mode = 0;
	AttitudeControlMode controlMode;


public:

	ControlData controlData;
	SensorData sensorData;
	FusedData fusedData;
	TelecommandData telecommandData;
	Modes modes2;
	IRData iRData;
	Mode2();
	void init();
	void run();
	void compute_av();
	void set_cmd_Limits_av(double Min_av,double Max_av);
	void set_Sample_Time_av(double NewSampleTime_av);
	void set_PID_values_av(double Kp_av, double Ki_av,double Kd_av);  /*for angular velocity control, Kp=3.10651129527485,Ki=42.3123158476514,Kd=12.786470237785991*/
// functions for angle position
	void compute_ap();
	void set_PID_values_ap(double Kp_ap, double Ki_ap,double Kd_ap);
	void set_Sample_Time_ap(double NewSampleTime_ap);
	void set_cmd_Limits_ap(double Min_ap,double Max_ap);
	void turn(double angle);
// angle/position redefinition for control
	void anglePositionRedefinition_Mode0();
// functions for linear distance control
	void compute_linp();
	void set_PID_values_linp(double Kp_linp, double Ki_linp,double Kd_linp);
	void set_Sample_Time_linp(double NewSampleTime_linp);
	void set_cmd_Limits_linp(double Min_linp,double Max_linp);
	void anglePositionRedefinition_Mode2();


};

extern Mode2 mode2;




#endif /* LIB_CONTROL_MODE2_AV_H_ */
