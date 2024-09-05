/*Control Code for vehicle angular speed with adjustable sample time, cmd limits and PID values*/

/* declaring variables */

double measured_av, cmdInRpm_motor_av, setpoint_av;
double I_term_av, last_measured_av;
double kp_av, kd_av, ki_av;
double SampleTime_av=0.01; //sample time in sec (10 ms)
double cmdMax_av=60, cmdMin_av=-60;  //limiting max speed of vehicle to 60rpm i.e one rotation per sec max. 

void compute_av()  //function to be called in each sample time
{
	/*calculating different error components for P,I and D*/
	double error_av = setpoint_av - measured_av; //proportional term
	
	I_term_av += (ki_av*error_av); //integrating term
	if (I_term_av>cmdMax_av)I_term_av=cmdMax_av;
	else if (I_term_av<cmdMin_av)I_term_av=cmdMin_av; 
	
	double d_measured_av=measured_av-last_measured_av; //differential term
	
	/*computing output of PID as cmd in voltage*/
	cmdInRpm_motor_av=kp_av*error_av + ki_av*I_term_av + kd_av*d_measured_av; //goes as setpoint to motor speed control
	
	if (cmdInRpm_motor_av>cmdMax_av)cmdInRpm_motor_av=cmdMax_av;
	else if(cmdInRpm_motor_av<cmdMin_av)cmdInRpm_motor_av=cmdMin_av;
	
	/*change in values*/
	last_measured_av=measured_av;

	}

void set_PID_values_av(double Kp_av, double Ki_av,double Kd_av)  /*for angular velocity control, Kp=3.10651129527485,Ki=42.3123158476514,Kd=12.786470237785991*/
{
	
	kp_av=_av;
	ki_av=Ki_av*SampleTime_av; /* SampleTime here must be in seconds since units of kp and kd are (1/s)*/
	kd_av=Kd_av/SampleTime_av;
}

void set_Sample_Time_av(double NewSampleTime_av)  /*currently set to 0.01 secs*/
{
	if (NewSampleTime_av>0)
	{	double ratio_av=NewSampleTime_av/SampleTime_av;
		ki_av*=ratio_av;
		kd_av/=ratio_av;
		SampleTime_av=NewSampleTime_av;
	}
}

void set_cmd_Limits_av(double Min_av,double Max_av) /*currently set to -60 to +60 rpm*/
{
	if (Min_av>Max_av) return;
	cmdMin_av=Min_av;
	cmdMax_av=Max_av;
	
	if (cmdInRpm_motor_av>cmdMax-av)cmdInRpm_motor_av=cmdMax_av;
	else if(cmdInRpm_motor_av<cmdMin_av)cmdInRpm_motor_av=cmdMin_av;
	
	if (I_term_av>cmdMax_av)I_term_av=cmdMax_av;
	else if (I_term_av<cmdMin_av)I_term_av=cmdMin_av;
	
	
}
