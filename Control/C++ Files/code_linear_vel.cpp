/*Control Code for vehicle linear velocity with adjustable sample time, cmd limits */

/* declaring variables */

double measured_linv, cmd_to_thrusters_linv, setpoint_linv; 
double kp_linv, kd_linv, ki_linv;
double SampleTime_linv=0.1; //sample time in sec (100 ms)
double cmdMax_linv=0.5, cmdMin_linv=-0.5;  //limiting thrust cmd of vehicle to 0.5 N . depends on max thrust from thrusters. To be checked experimentally. 

void compute_linv()  //function to be called in each sample time
{
	/*calculating different error components for P,I and D*/
	double error_linv = setpoint_linv - measured_linv; //proportional term
	
	I_term_linv += (ki_linv*error_linv); //integrating term
	if (I_term_linv>cmdMax_linv)I_term_linv=cmdMax_linv;
	else if (I_term_linv<cmdMin_linv)I_term_linv=cmdMin_linv; 
	
	double d_measured_linv=measured_linv-last_measured_linv; //differential term
	
	/*computing output of PID as thrust required*/
	cmd_to_thrusters_linv=kp_linv*error_linv + ki_linv*I_term_linv + kd_linv*d_measured_linv; //absolute value of required thrust
	
	if (cmd_to_thrusters_linv>cmdMax_linv)cmd_to_thrusters_linv=cmdMax_linv;
	else if(cmd_to_thrusters_linv<cmdMin_linv)cmd_to_thrusters_linv=cmdMin_linv;
	
	if (cmd_to_thrusters_linv>0.005)thruster_1=false;thruster_2=true;thruster_3=true;
	else if (cmd_to_thrusters_linv<-0.005)thruster_2=false; thruster_3=false; thruster_1=true;
	
	/*change in values*/
	last_measured_linv=measured_linv;

	}

void set_PID_values_linv(double Kp_linv, double Ki_linv,double Kd_linv)  /*for linear velocity control, 
Kp=0.878697351138722,Ki=0.0696549617328774,Kd=1.546138490255642*/
{
	
	kp_linv=_linv;
	ki_linv=Ki_linv*SampleTime_linv; /* SampleTime here must be in seconds since units of kp and kd are (1/s)*/
	kd_linv=Kd_linv/SampleTime_linv;
}

void set_Sample_Time_linv(double NewSampleTime_linv)  /*currently set to 0.1 secs*/
{
	if (NewSampleTime_linv>0)
	{	double ratio_linv=NewSampleTime_linv/SampleTime_linv;
		ki_linv*=ratio_linv;
		kd_linv/=ratio_linv;
		SampleTime_linv=NewSampleTime_linv;
	}
}

void set_cmd_Limits_linv(double Min_linv,double Max_linv) /*limiting thrust cmd of vehicle to 0.5 N . */
{
	if (Min_linv>Max_linv) return;
	cmdMin_linv=Min_linv;
	cmdMax_linv=Max_linv;
	
	if (cmd_to_thrusters_linv>cmdMax-av)cmd_to_thrusters_linv=cmdMax_linv;
	else if(cmd_to_thrusters_linv<cmdMin_linv)cmd_to_thrusters_linv=cmdMin_linv;
	
	if (I_term_linv>cmdMax_linv)I_term_linv=cmdMax_linv;
	else if (I_term_linv<cmdMin_linv)I_term_linv=cmdMin_linv;
	
	
}
