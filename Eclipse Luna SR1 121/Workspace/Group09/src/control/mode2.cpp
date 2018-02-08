/*
 * mode2_av.cpp
 *
 *  Created on: 27 Jan 2018
 *      Author: Felix Hessinger (Controller-logic and values done by Neha)
 */

#include "../../lib/control/mode2.h"

#include <stdio.h>
#include "hal.h"
#include <string>
#include <math.h>

/////////////////////////////////////////////////////////////////////
// Position and Angle redefinition for control
/////////////////////////////////////////////////////////////////////
double set_pointPos[2];
double currentPos[2];
int t1, t2, t3;
double theta_set_point;
int desiredThrust = 0;
int desiredMS = 0;
/////////////////////////////////////////////////////////////////////
// MODE(-1)
/////////////////////////////////////////////////////////////////////
//DO NOTHING

/////////////////////////////////////////////////////////////////////
// MODE0
/////////////////////////////////////////////////////////////////////
//position(distance)-control
double Kp_linp = 20; //0.0761445586673765;
double Ki_linp = 0.0005; //0.000345918152371027;
double Kd_linp = 178.92; //1.397986555697760;

double error_linp, d_measured_linp;
double measured_linp, cmd_to_thrusters_linp, setpoint_linp = 0; //measured vale to be calculated back to linear positions
double I_term_linp, last_measured_linp;
double kp_linp, kd_linp, ki_linp;
double dist = 0;
double SampleTime_linp = 0.1; //sample time in sec (100 ms)
double cmdMax_linp = 0.5, cmdMin_linp = -0.5; //limiting thrust cmd of vehicle to 0.5 N . depends on max thrust from thrusters. To be checked experimentally.

/////////////////////////////////////////////////////////////////////
// MODE1
/////////////////////////////////////////////////////////////////////
HAL_GPIO Magnet(HBRIDGEAB);
int servoAngle;
double additionalAngle;
int rotate180;
int magnet;

/////////////////////////////////////////////////////////////////////
// MODE2
/////////////////////////////////////////////////////////////////////
// speed of reactionwheel
int desiredRWSpeed;
//angular-velocity-values
double Kp_av = 150.5291625083045; 		//3.10651129527485;
double Ki_av = 8.71388896065982; 		//42.3123158476514;
double Kd_av = 8.0763853389015466;		//12.786470237785991;

double error_av, d_measured_av;
double measured_av, cmdInRpm_motor_av, setpoint_av;
double I_term_av, last_measured_av;
double kp_av, kd_av, ki_av;
double SampleTime_av = 0.1; //sample time in sec (10 ms)
double cmdMax_av = MAX_RPM_RW_POSSIBLE, cmdMin_av = -MAX_RPM_RW_POSSIBLE; //limiting max speed of vehicle to 60rpm i.e one rotation per sec max.

//angular-position-values
double Kp_ap = 108.008029581828; 					//19.0570345025429;
double Ki_ap = 1.862167538165 * 0.3;   				//0.597438419047018;
double Kd_ap = -33.227346330873338 * 2;				//-4.243928899024035;

double error_ap, d_measured_ap;
double measured_ap, cmdInRpm_motor_ap, setpoint_ap;
double I_term_ap, last_measured_ap;
double kp_ap, kd_ap, ki_ap;
double SampleTime_ap = 0.1; //sample time in sec (10 ms)
double cmdMax_ap = MAX_RPM_RW_POSSIBLE, cmdMin_ap = -MAX_RPM_RW_POSSIBLE; //limiting max angle of vehicle to 90 deg turn

/////////////////////////////////////////////////////////////////////
// MODE3
/////////////////////////////////////////////////////////////////////
double dock_dist;
double dock_angle_radians;
double dock_angle;
int64_t past_mode3 = 0;

/////////////////////////////////////////////////////////////////////
// MODE4
/////////////////////////////////////////////////////////////////////

HAL_PWM Servo(PWM_IDX00);

CommBuffer<SensorData> SensorDataBuffer2;
Subscriber SensorDataSubscriber2(SensorDataTopic, SensorDataBuffer2);

CommBuffer<FusedData> FusedDataBuffer2;
Subscriber FusedDataSubscriber2(FusedDataTopic, FusedDataBuffer2);

CommBuffer<TelecommandData> TelecommandDataBuffer2;
Subscriber TelecommandDataSubscriber2(TelecommandDataTopic,
		TelecommandDataBuffer2);

CommBuffer<IRData> IRDataBuffer2;
Subscriber IRDataSubscriber2(IRDataTopic, IRDataBuffer2);

Mode2 mode2;

Mode2::Mode2() :
		Thread() {

}

void Mode2::init() {
	set_PID_values_av(Kp_av, Ki_av, Kd_av);
	set_PID_values_ap(Kp_ap, Ki_ap, Kd_ap);
	set_PID_values_linp(Kp_linp, Ki_linp, Kd_linp);
	Servo.init(50, 20000);
	Magnet.init(true, 1, 0);

}

void Mode2::run() {
//	suspendCallerUntil(WAITINGTIME_UNTIL_START * SECONDS);

	while (1) {

		SensorDataBuffer2.get(sensorData);
		FusedDataBuffer2.get(fusedData);
		TelecommandDataBuffer2.get(telecommandData);
//get all data needed for next control
		setpoint_ap = telecommandData.desiredAngle;
		setpoint_av = telecommandData.desiredAngularVel;
		desiredRWSpeed = telecommandData.rwRotationSpeed;
		t1 = telecommandData.t1;
		t2 = telecommandData.t2;
		t3 = telecommandData.t3;
		rotate180 = telecommandData.rotate180;
		magnet = telecommandData.activateMagnet;
		servoAngle = telecommandData.servoAngle;

/////////////////////////////////////////////////////////////
// for testing purposes only
/////////////////////////////////////////////////////////////
		setpoint_av = 9999; //SET ANGULAR VELOCITY GOAL (9999 is for disabling angle-velocity-control)// this has to be removed later
//		int desiredThrust = 0;
//		int desiredMS = 0;

		telecommandData.mode = 1;
//		setpoint_ap = -45;
//		theta_set_point = -45;

//FOR MAKING PROOF VIDEO
		telecommandData.servoAngle=940;
		magnet = 1;
		if (NOW()>2*SECONDS){telecommandData.servoAngle=1900;}
		if (NOW()>4*SECONDS){t1=1;}
		if (NOW()>6*SECONDS){t2=1;t1=0;}
		if (NOW()>8*SECONDS){t3=1;t2=0;}
		if (NOW()>10*SECONDS){t3=0;}
		if (NOW()>30*SECONDS){magnet=0;}

///////////////////////////////////////////////////////////

//		PRINTF("(mode2.cpp)Mode%d", modes2.mode);

		switch (telecommandData.mode) {

		case ATTITUDE_CONTROL_MODE_STANDBY:
			t1=t2=t3=0;
			desiredMS =0;
			break;

		case ATTITUDE_CONTROL_MODE_AUTO:
			Servo.write(940);
			currentPos[0] = 0;
			currentPos[1] = 0;
			set_pointPos[0] = 1;
			set_pointPos[1] = 1;
//POSITION CONTROL TEST DATA FOR NEHA
//			if (NOW()>20000000000){
//			set_pointPos[0] = 1;
//			set_pointPos[1] = 1;
//			if (NOW()>40000000000){
//			set_pointPos[0] = 1;
//			set_pointPos[1] = 0;
//			if (NOW()>60000000000){
//			set_pointPos[0] = 0;
//			set_pointPos[1] = -1;
//			}
//			if (NOW()>80000000000){
//			set_pointPos[0] = 0;
//			set_pointPos[1] = 1;
//			}
//			set_pointPos[0] = telecommandData.targetPosition_x;
//			set_pointPos[1] = telecommandData.targetPosition_y;
//			currentPos[0] = fusedData.x;
//			currentPos[0] = fusedData.y;

//////////////////
// set test values for calibration of own controller
/////////////////
//			set_pointPos[0] = 0.0f;
//			set_pointPos[1] = 0.0f;
//			currentPos[0]	= 0.0f;
//			currentPos[1]	= 0.0f;
/////////////////

			anglePositionRedefinition_Mode0();
			/////////////////
			//angle control
			/////////////////
			measured_ap = (double) sensorData.angleZ / 100;
			compute_ap();
			desiredMS = (int) (cmdInRpm_motor_ap * 1000 / cmdMax_ap);
			/////////////////
			//distance control
			/////////////////
			dist = sqrt(
					pow((set_pointPos[0] - currentPos[0]), 2.0)
							+ pow((set_pointPos[1] - currentPos[1]), 2));
			measured_linp = -dist;
			if (fabs(error_ap) < 3) {
				compute_linp();
				desiredThrust = (int) (cmd_to_thrusters_linp);
			} else {
				desiredThrust = 0;
				t1 = t2 = t3 = 0;
			}
//TODO: clear with Shashi that mode change to docking mode should be executed here with command to groundstation. Plus set mode for uplink to 1
			if (fabs(iRData.distance) < 150) {
				//do docking
//				telecommandData.mode = ATTITUDE_CONTROL_MODE_DOCKING;
				//send telecommand to groundstation to change the mode to docking mode (mode 3)

			}
			break;

		case ATTITUDE_CONTROL_MODE_MANUAL:
			Servo.write(telecommandData.servoAngle);
			measured_ap = (double) sensorData.angleZ;
//			PRINTF("before");
			if (telecommandData.rotateDelta == 1) {
				desiredMS = -1000;
			} else {
				if (telecommandData.rotateDelta == 0) {
					desiredMS = 0;
				} else {
					if (telecommandData.rotateDelta == (-1)) {
						desiredMS = 1000;
					} else {
						PRINTF(
								"\nERROR in rotationDeltaInput from Groundstation");
					}
				}
			}

			//thruster are already set in the beginning while reading from telecommandDatabuffer

			//control of Magnet
			Magnet.setPins(magnet);

			break;

		case ATTITUDE_CONTROL_MODE_ANGULAR_SPEED:
			Servo.write(1900);

			///////////////////////////////////////////////////////////////////////////
			// START OF ANGUAR VELOCITY CONTROR
			///////////////////////////////////////////////////////////////////////////

			if (setpoint_av == 9999) {
				//do nothing
			} else {	// enable angular velocity controller

				//PRINTF("\nGyroZ %d",sensorData.gyroZ);
				//PRINTF("\nGyroZ/100 %d",(sensorData.gyroZ/100));
				measured_av = (double) sensorData.gyroZ / 100;
				//			PRINTF("\nMeasured_av: %f",measured_av);
				compute_av();

				//			PRINTF("\nI_term_av: %f", I_term_av);
				//			PRINTF("\nd_measured: %f", d_measured_av);
				//			PRINTF("\nERROR: %f", error_av);
				//			PRINTF("\nDouble: %f", cmdInRpm_motor_av);
				//
				//			PRINTF("\nINT: %d", (int) cmdInRpm_motor_av);
				desiredMS = (int) (cmdInRpm_motor_av * 1000 / cmdMax_av);
				//			PRINTF("\nINT_cal: %d", desiredMS);
			}

			//////////////////////////////////////////////////////////////////////////
			// START OF ANGLE CONTROL
			//////////////////////////////////////////////////////////////////////////

			if (setpoint_ap == 9999) {
				// do nothing
			} else {
				measured_ap = (double) sensorData.angleZ / 100;
				anglePositionRedefinition_Mode2();
				compute_ap();
				desiredMS = (int) (cmdInRpm_motor_ap * 1000 / cmdMax_ap);

			}
			break;

		case ATTITUDE_CONTROL_MODE_DOCKING:
			Servo.write(940);
			Magnet.setPins(1);

			dock_dist = iRData.distance;
			dock_angle_radians = iRData.angle;
			dock_angle = dock_angle_radians * 180 / M_PI;

			if (dock_angle > 3) {
				if (NOW()-past_mode3>=1*SECONDS) {
					turn(dock_angle);
				}
				compute_ap();
				desiredMS = (int) (cmdInRpm_motor_ap * 1000 / cmdMax_ap);
			} else {
				break;
			}
			measured_linp = -dock_dist;
			compute_linp();
			past_mode3 = NOW();

			PRINTF("\nIR_ANGLE: %f", iRData.angle);
			PRINTF("\nIR_DISTANCE: %f", iRData.distance);
			PRINTF("\nIR_RANGE1: %d", iRData.range1);
			PRINTF("\nIR_RANGE2: %d", iRData.range2);

			break;

			case ATTITUDE_CONTROL_MODE_OBJECTRECOGNITION:
			Servo.write(1900);
//			setpoint_av = 10;			//desired rotational speed in [deg/sec]
			//functions ask how it should be done (MARTIN'S MISSION)

			break;

			case ATTITUDE_CONTROL_MODE_SEMIMANUAL:
			Servo.write(1900);
			measured_ap = (double) sensorData.angleZ / 100;
			additionalAngle = (double) telecommandData.rotateDelta;
			turn(additionalAngle);
			compute_ap();
			desiredMS = (int) (cmdInRpm_motor_ap * 1000 / cmdMax_ap);
////////////////

			break;
		}

		// set the controlled m-speed and publish it
		controlData.desiredThrusterValue = desiredThrust;
		controlData.desiredRWSpeed = desiredMS;
		controlData.controlledT1 = t1;
		controlData.controlledT2 = t2;
		controlData.controlledT3 = t3;
		ControlDataTopic.publish(controlData);

		suspendCallerUntil(NOW()+20*MILLISECONDS);
	}
}

/////////////////////////////////////////////////////////////////////////////////
//  FUNCTIONS FOR CONTROL
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
//Control Code for vehicle angular speed with adjustable sample time, cmd limits and PID values
/////////////////////////////////////////////////////////////////////////////////

void Mode2::compute_av()  //function to be called in each sample time
{
	/*calculating different error components for P,I and D*/
	error_av = setpoint_av - measured_av; //proportional term

	I_term_av += (ki_av * error_av); //integrating term
	if (I_term_av > cmdMax_av)
		I_term_av = cmdMax_av;
	else if (I_term_av < cmdMin_av)
		I_term_av = cmdMin_av;

	d_measured_av = measured_av - last_measured_av; //differential term

	/*computing output of PID as cmd in voltage*/
	cmdInRpm_motor_av = kp_av * error_av + ki_av * I_term_av
			+ kd_av * d_measured_av; //goes as setpoint to motor speed control

	if (cmdInRpm_motor_av > cmdMax_av)
		cmdInRpm_motor_av = cmdMax_av;
	else if (cmdInRpm_motor_av < cmdMin_av)
		cmdInRpm_motor_av = cmdMin_av;

	/*change in values*/
	last_measured_av = measured_av;
	suspendCallerUntil(NOW()+100*MILLISECONDS);
}

void Mode2::set_PID_values_av(double Kp_av, double Ki_av, double Kd_av) /*for angular velocity control, Kp=3.10651129527485,Ki=42.3123158476514,Kd=12.786470237785991*/
{
	kp_av = Kp_av;
	ki_av = Ki_av * SampleTime_av; /* SampleTime here must be in seconds since units of kp and kd are (1/s)*/
	kd_av = Kd_av / SampleTime_av;
}

void Mode2::set_Sample_Time_av(double NewSampleTime_av) /*currently set to 0.01 secs*/
{
	if (NewSampleTime_av > 0) {
		double ratio_av = NewSampleTime_av / SampleTime_av;
		ki_av *= ratio_av;
		kd_av /= ratio_av;
		SampleTime_av = NewSampleTime_av;
	}
}

void Mode2::set_cmd_Limits_av(double Min_av, double Max_av) /*currently set to -60 to +60 rpm*/
{
	if (Min_av > Max_av)
		return;
	cmdMin_av = Min_av;
	cmdMax_av = Max_av;

	if (cmdInRpm_motor_av > cmdMax_av)
		cmdInRpm_motor_av = cmdMax_av;
	else if (cmdInRpm_motor_av < cmdMin_av)
		cmdInRpm_motor_av = cmdMin_av;

	if (I_term_av > cmdMax_av)
		I_term_av = cmdMax_av;
	else if (I_term_av < cmdMin_av)
		I_term_av = cmdMin_av;
}

/////////////////////////////////////////////////////////////////////////////////
//Control Code for vehicle ANGLE with adjustable sample time, cmd limits and PID values
/////////////////////////////////////////////////////////////////////////////////

void Mode2::compute_ap() //function to be called in each sample time
{
	/*calculating different error components for P,I and D*/
	error_ap = setpoint_ap - measured_ap; //proportional term
	if (fabs(error_ap) > 180)
		error_ap = error_ap + 360;
	if (fabs(error_ap) > 360)
		error_ap = error_ap - 720;

//Interlock to be made here taking error_ap value. If it is more than abs(1) deg, thrusters must be suspended//

	I_term_ap += (ki_ap * error_ap); //integrating term
	if (I_term_ap > cmdMax_ap)
		I_term_ap = cmdMax_ap;
	else if (I_term_ap < cmdMin_ap)
		I_term_ap = cmdMin_ap;

	d_measured_ap = measured_ap - last_measured_ap; //differential term

	/*computing output of PID as cmd in voltage*/
	cmdInRpm_motor_ap = kp_ap * error_ap + ki_ap * I_term_ap
			+ kd_ap * d_measured_ap; //goes as setpoint to motor speed control

	if (cmdInRpm_motor_ap > cmdMax_ap)
		cmdInRpm_motor_ap = cmdMax_ap;
	else if (cmdInRpm_motor_ap < cmdMin_ap)
		cmdInRpm_motor_ap = cmdMin_ap;

	/*change in values*/
	last_measured_ap = measured_ap;
	suspendCallerUntil(NOW()+100*MILLISECONDS);

}

void Mode2::set_PID_values_ap(double Kp_ap, double Ki_ap, double Kd_ap) /*for angular position control, Kp=19.0570345025429,Ki=0.597438419047018,Kd= -4.243928899024035*/
{
	kp_ap = Kp_ap;
	ki_ap = Ki_ap * SampleTime_ap; /* SampleTime here must be in seconds since units of kp and kd are (1/s)*/
	kd_ap = Kd_ap / SampleTime_ap;
}

void Mode2::set_Sample_Time_ap(double NewSampleTime_ap) /*currently set to 0.01 secs*/
{
	if (NewSampleTime_ap > 0) {
		double ratio_ap = NewSampleTime_ap / SampleTime_ap;
		ki_ap *= ratio_ap;
		kd_ap /= ratio_ap;
		SampleTime_ap = NewSampleTime_ap;
	}
}

void Mode2::set_cmd_Limits_ap(double Min_ap, double Max_ap) /*currently set to -90 to +90 deg*/
{
	if (Min_ap > Max_ap)
		return;
	cmdMin_ap = Min_ap;
	cmdMax_ap = Max_ap;

	if (cmdInRpm_motor_ap > cmdMax_ap)
		cmdInRpm_motor_ap = cmdMax_ap;
	else if (cmdInRpm_motor_ap < cmdMin_ap)
		cmdInRpm_motor_ap = cmdMin_ap;

	if (I_term_ap > cmdMax_ap)
		I_term_ap = cmdMax_ap;
	else if (I_term_ap < cmdMin_ap)
		I_term_ap = cmdMin_ap;

}

/////////////////////////////////////////////////////////////////////
// Position and Angle redefinition for ANGLE AND POS control
/////////////////////////////////////////////////////////////////////

void Mode2::turn(double angle) {
	setpoint_ap = measured_ap + angle; //only single value for currentPos angle to be taken for measured, not a continuous function//
}

void Mode2::anglePositionRedefinition_Mode0() {
	theta_set_point = atan2((set_pointPos[1] - currentPos[1]),
			(set_pointPos[0] - currentPos[0])); //angle in range of -180 to 180 deg//
	theta_set_point = theta_set_point * 180 / M_PI;
	if (theta_set_point <= 180 && theta_set_point >= 0) {
		theta_set_point = 1 * theta_set_point;
	} else if (theta_set_point > -180 && theta_set_point < 0) {
		theta_set_point = theta_set_point + 360; //makes it between 0 to 360 deg//
	}
	double difference = theta_set_point - measured_ap; //total  delta

	if (270 > fabs(difference) > 90) {
		setpoint_ap = theta_set_point + 180;
		t1 = 1;
		t2 = 0;
		t3 = 0; //ensuring delta is less than 90 deg//
	} else {
		setpoint_ap = theta_set_point;
		t1 = 0;
		t2 = 1;
		t3 = 1;
	}
	if (setpoint_ap >= 360) {
		setpoint_ap = setpoint_ap - 360; //ensuring setpoint_ap is less than 360 deg
	}
	PRINTF("Setpoint_ap_MODE0: %f", setpoint_ap);
}

void Mode2::anglePositionRedefinition_Mode2() {
	setpoint_ap = theta_set_point;
	PRINTF("Setpoint_ap_MODE2: %f", setpoint_ap);

}

/////////////////////////////////////////////////////////////////////////////////
//Control Code for vehicle POSITION(DISTANCE) with adjustable sample time, cmd limits and PID values
/////////////////////////////////////////////////////////////////////////////////

void Mode2::compute_linp() //function to be called in each sample time
{
	/*calculating different error components for P,I and D*/
	error_linp = setpoint_linp - measured_linp; //proportional term
	if (error_linp < 0.02) {
		return;
	}

	I_term_linp += (ki_linp * error_linp); //integrating term
	if (I_term_linp > cmdMax_linp)
		I_term_linp = cmdMax_linp;
	else if (I_term_linp < cmdMin_linp)
		I_term_linp = cmdMin_linp;

	d_measured_linp = measured_linp - last_measured_linp; //differential term

	/*computing output of PID as thrust required*/
	cmd_to_thrusters_linp = kp_linp * error_linp + ki_linp * I_term_linp
			+ kd_linp * d_measured_linp; //absolute value of required thrust

	if (cmd_to_thrusters_linp > cmdMax_linp)
		cmd_to_thrusters_linp = cmdMax_linp;
	else if (cmd_to_thrusters_linp < cmdMin_linp)
		cmd_to_thrusters_linp = cmdMin_linp;

	if (cmd_to_thrusters_linp > 0.005) {
		t1 = 0;
		t2 = 1;
		t3 = 1;
	} else {
		if (cmd_to_thrusters_linp < -0.005) {
			t1 = 1;
			t2 = 0;
			t3 = 0;
		}
	}
	/*change in values*/
	last_measured_linp = measured_linp;
	suspendCallerUntil(NOW()+100*MILLISECONDS);

}

void Mode2::set_PID_values_linp(double Kp_linp, double Ki_linp, double Kd_linp) /*for linear position control, Kp=0.0761445586673765,Ki=0.000345918152371027,Kd=1.397986555697760*/
{

	kp_linp = Kp_linp;
	ki_linp = Ki_linp * SampleTime_linp; /* SampleTime here must be in seconds since units of kp and kd are (1/s)*/
	kd_linp = Kd_linp / SampleTime_linp;
}

void Mode2::set_Sample_Time_linp(double NewSampleTime_linp) /*currently set to 0.1 secs*/
{
	if (NewSampleTime_linp > 0) {
		double ratio_linp = NewSampleTime_linp / SampleTime_linp;
		ki_linp *= ratio_linp;
		kd_linp /= ratio_linp;
		SampleTime_linp = NewSampleTime_linp;
	}
}

void Mode2::set_cmd_Limits_linp(double Min_linp, double Max_linp) //limiting thrust cmd of vehicle to 0.5 N . depends on max thrust from thrusters. To be checked experimentally. /
		{
	if (Min_linp > Max_linp)
		return;
	cmdMin_linp = Min_linp;
	cmdMax_linp = Max_linp;

	if (cmd_to_thrusters_linp > cmdMax_av) {
		cmd_to_thrusters_linp = cmdMax_linp;
	} else {
		if (cmd_to_thrusters_linp < cmdMin_linp) {
			cmd_to_thrusters_linp = cmdMin_linp;
		}
	}
	if (I_term_linp > cmdMax_linp)
		I_term_linp = cmdMax_linp;
	else if (I_term_linp < cmdMin_linp)
		I_term_linp = cmdMin_linp;

}
