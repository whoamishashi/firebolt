/*Control Code for vehicle angular position with adjustable sample time, cmd limits and PID values*/

/* declaring variables */

double measured_ap, cmdInRpm_motor_ap, setpoint_ap;
double I_term_ap, last_measured_ap;
double kp_ap, kd_ap, ki_ap;
double SampleTime_ap=0.01; //sample time in sec (10 ms)
double cmdMax_ap=90, cmdMin_ap=-90;  //limiting max angle of vehicle to 90 deg turn

void compute_ap()  //function to be called in each sample time
{
	/*calculating different error components for P,I and D*/
	double error_ap = setpoint_ap - measured_ap; //proportional term
	
	//interlock to be made here taking error_ap value. If it is more than abs(1) deg, thrusters must be suspended//
	
	I_term_ap += (ki_ap*error_ap); //integrating term
	if (I_term_ap>cmdMax_ap)I_term_ap=cmdMax_ap;
	else if (I_term_ap<cmdMin_ap)I_term_ap=cmdMin_ap; 
	
	double d_measured_ap=measured_ap-last_measured_ap; //differential term
	
	/*computing output of PID as cmd in voltage*/
	cmdInRpm_motor_ap=kp_ap*error_ap + ki_ap*I_term_ap + kd_ap*d_measured_ap; //goes as setpoint to motor speed control
	
	if (cmdInRpm_motor_ap>cmdMax_ap)cmdInRpm_motor_ap=cmdMax_ap;
	else if(cmdInRpm_motor_ap<cmdMin_ap)cmdInRpm_motor_ap=cmdMin_ap;
	
	/*change in values*/
	last_measured_ap=measured_ap;

	}

void set_PID_values_ap(double Kp_ap, double Ki_ap,double Kd_ap)  /*for angular position control, Kp=19.0570345025429,Ki=0.597438419047018,Kd= -4.243928899024035*/
{
	
	kp_ap=Kp_ap;
	ki_ap=Ki_ap*SampleTime_ap; /* SampleTime here must be in seconds since units of kp and kd are (1/s)*/
	kd_ap=Kd_ap/SampleTime_ap;
}

void set_Sample_Time_ap(double NewSampleTime_ap)  /*currently set to 0.01 secs*/
{
	if (NewSampleTime_ap>0)
	{	double ratio_ap=NewSampleTime_ap/SampleTime_ap;
		ki_ap*=ratio_ap;
		kd_ap/=ratio_ap;
		SampleTime_ap=NewSampleTime_ap;
	}
}

void set_cmd_Limits_ap(double Min_ap,double Max_ap) /*currently set to -90 to +90 deg*/
{
	if (Min_ap>Max_ap) return;
	cmdMin_ap=Min_ap;
	cmdMax_ap=Max_ap;
	
	if (cmdInRpm_motor_ap>cmdMax-av)cmdInRpm_motor_ap=cmdMax_ap;
	else if(cmdInRpm_motor_ap<cmdMin_ap)cmdInRpm_motor_ap=cmdMin_ap;
	
	if (I_term_ap>cmdMax_ap)I_term_ap=cmdMax_ap;
	else if (I_term_ap<cmdMin_ap)I_term_ap=cmdMin_ap;
	
	
}
