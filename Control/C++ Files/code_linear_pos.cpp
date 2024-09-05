/*Control Code for vehicle linear position with adjustable sample time, cmd limits */

/* declaring variables */

double measured_linp, cmd_to_thrusters_linp, setpoint_linp=0; //measured vale to be calculated back to linear positions
double I_term_linp, last_measured_linp;
double kp_linp, kd_linp, ki_linp;
double SampleTime_linp=0.1; //sample time in sec (100 ms)
double cmdMax_linp=0.5, cmdMin_linp=-0.5;  //limiting thrust cmd of vehicle to 0.5 N . depends on max thrust from thrusters. To be checked experimentally. 

void compute_linp()  //function to be called in each sample time
{
	/*calculating different error components for P,I and D*/
	double error_linp = setpoint_linp - measured_linp; //proportional term
	
	I_term_linp += (ki_linp*error_linp); //integrating term
	if (I_term_linp>cmdMax_linp)I_term_linp=cmdMax_linp;
	else if (I_term_linp<cmdMin_linp)I_term_linp=cmdMin_linp; 
	
	double d_measured_linp=measured_linp-last_measured_linp; //differential term
	
	/*computing output of PID as thrust required*/
	cmd_to_thrusters_linp=kp_linp*error_linp + ki_linp*I_term_linp + kd_linp*d_measured_linp; //absolute value of required thrust
	
	if (cmd_to_thrusters_linp>cmdMax_linp)cmd_to_thrusters_linp=cmdMax_linp;
	else if(cmd_to_thrusters_linp<cmdMin_linp)cmd_to_thrusters_linp=cmdMin_linp;
	
	// if (cmd_to_thrusters_linp>0.005)thruster_1=false;thruster_2=true;thruster_3=true;
	// else if (cmd_to_thrusters_linp<-0.005)thruster_2=false; thruster_3=false; thruster_1=true;
	
	/*change in values*/
	last_measured_linp=measured_linp;

	}

void set_PID_values_linp(double Kp_linp, double Ki_linp,double Kd_linp)  /*for linear position control, Kp=0.0761445586673765,Ki=0.000345918152371027,Kd=1.397986555697760*/
{
	
	kp_linp=_linp;
	ki_linp=Ki_linp*SampleTime_linp; /* SampleTime here must be in seconds since units of kp and kd are (1/s)*/
	kd_linp=Kd_linp/SampleTime_linp;
}

void set_Sample_Time_linp(double NewSampleTime_linp)  /*currently set to 0.1 secs*/
{
	if (NewSampleTime_linp>0)
	{	double ratio_linp=NewSampleTime_linp/SampleTime_linp;
		ki_linp*=ratio_linp;
		kd_linp/=ratio_linp;
		SampleTime_linp=NewSampleTime_linp;
	}
}

void set_cmd_Limits_linp(double Min_linp,double Max_linp) //limiting thrust cmd of vehicle to 0.5 N . depends on max thrust from thrusters. To be checked experimentally. /
{
	if (Min_linp>Max_linp) return;
	cmdMin_linp=Min_linp;
	cmdMax_linp=Max_linp;
	
	if (cmd_to_thrusters_linp>cmdMax-av)cmd_to_thrusters_linp=cmdMax_linp;
	else if(cmd_to_thrusters_linp<cmdMin_linp)cmd_to_thrusters_linp=cmdMin_linp;
	
	if (I_term_linp>cmdMax_linp)I_term_linp=cmdMax_linp;
	else if (I_term_linp<cmdMin_linp)I_term_linp=cmdMin_linp;
	
	
}
