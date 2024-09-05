/*Control Code for running motor using PWM with adjustable sample time, cmd limits and PID values*/

/* declaring variables */

double measured, cmdInV, setpoint;
double I_term, last_measured=0;
double kp, kd, ki;
double SampleTime=0.001; //sample time in sec (1 ms)
double cmdMax=9, cmdMin=-9;

void compute()  //function to be called in each sample time
{
	/*calculating different error components for P,I and D*/
	double error = setpoint - measured; //proportional term
	
	I_term += (ki*error); //integrating term
	if (I_term>cmdMax)I_term=cmdMax;
	else if (I_term<cmdMin)I_term=cmdMin; 
	
	double d_measured=measured-last_measured; //differential term
	
	/*computing output of PID as cmd in voltage*/
	cmdInV=kp*error + ki*sum_err + kd*d_measured;
	
	if (cmdInV>cmdMax)cmdInV=cmdMax;
	else if(cmdInV<cmdMin)cmdInV=cmdMin;
	
	/*take this output, divide by 9.0 and multiply by 100 to give PWM level in percentage)*/
	
	/*change in values*/
	last_measured=measured;

	}

void set_PID_values(double Kp, double Ki,double Kd)  /*for motor control, Kp=0.141847319632733,Ki=0.0533184625505999,Kd=0.099070843796960*/
{
	
	kp=Kp;
	ki=Ki*SampleTime; /* SampleTime here must be in seconds since units of kp and kd are (1/s)*/
	kd=Kd/SampleTime;
}

void set_Sample_Time(double NewSampleTime)  /*currently set to 0.001 secs*/
{
	if (NewSampleTime>0)
	{	double ratio=NewSampleTime/SampleTime;
		ki*=ratio;
		kd/=ratio;
		SampleTime=NewSampleTime;
	}
}

void set_cmd_Limits(double Min,double Max) /*currently set to -9 to +9V*/
{
	if (Min>Max) return;
	cmdMin=Min;
	cmdMax=Max;
	
	if (cmdInV>cmdMax)cmdInV=cmdMax;
	else if(cmdInV<cmdMin)cmdInV=cmdMin;
	
	if (I_term>cmdMax)I_term=cmdMax;
	else if (I_term<cmdMin)I_term=cmdMin;
	
	
}
