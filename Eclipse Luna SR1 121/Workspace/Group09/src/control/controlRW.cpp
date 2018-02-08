/*
 * controlRW.cpp
 *
 *  Created on: 20 Dec 2017
 *      Author: felix
 */

#include "../../lib/control/controlRW.h"




ControlRW controlRW;

/////////////////////Values for Control of Reactionwheel/////////////
double kp=0.141847319632733;						//0.141847319632733;
double ki=0.0533184625505999;						//0.0533184625505999;
double kd=0.0099070843796960;						//0.0099070843796960;


double measured, cmdInV, setpoint;
double I_term, last_measured;
double SampleTime=0.01; //sample time in sec (10 ms)
double cmdMax=9, cmdMin=-9;

//	int16_t offGyroX, offGyroY, offGyroZ;
	// local variables
//	static volatile int16_t oldLeftEncoder =0;
//	static volatile int16_t oldRightEncoder=0;
//	static volatile int16_t leftEncoder=0;
//	static volatile int16_t rightEncoder=0;
//	static volatile int16_t encoderSum=0;
//	static volatile int16_t encoderDiff=0;

	//available to the rest of the code
	//speeds
	volatile int16_t leftCount;
	volatile int16_t rightCount;
	volatile int16_t fwdCount;
	volatile int16_t rotCount;
	//distances
	volatile int32_t leftTotal;
	volatile int32_t rightTotal;
	volatile int32_t fwdTotal;
	volatile int32_t rotTotal;



//	CommBuffer<MotorData> MotorDataBuffer1;
//	Subscriber MotorDataSubscriber1(MotorDataTopic, MotorDataBuffer1);

	CommBuffer<ControlData> ControlDataBuffer1;
	Subscriber ControlDataSubscriber1(ControlDataTopic, ControlDataBuffer1);




ControlRW::ControlRW() {
}

void ControlRW::init() {
	encoderInit();
}

int16_t rpm;


void ControlRW::run() {
	init();
	now = 0;
	suspendCallerUntil(WAITINGTIME_UNTIL_START*SECONDS);


	while(1){

		ControlDataBuffer1.get(controlData);
		setpoint = -controlData.desiredRWSpeed;

		rpm = getRPM();

		measured = (rpm*1000/MAX_RPM_RW_POSSIBLE);  // divided by 12000rpm times 1000 for write input (range 0 to 1000)(0to100%)
//		("InputFromSensor: %f\n",input);
	///////////////Closed loop (for now only open loop control)
//		RWC_out = 0;

//		compute();
		//suspendCallerUntil(NOW()+500*MILLISECONDS);
//		RWC_out= cmdInV*1000/9;
//		motorData.controlled_m_speed = RWC_out;
	/////////////// uncomment uppper code for closed loop


		motorData.controlled_m_speed = setpoint;
		motorData.sensorMotorSpeed = rpm;
		MotorDataTopic.publish(motorData);
		suspendCallerUntil(NOW()+10*MILLISECONDS);

	}
}

void ControlRW::set_PID_values(double Kp, double Ki,double Kd)  /*for motor control, Kp=0.141847319632733,Ki=0.0533184625505999,Kd=0.099070843796960*/
{

	kp=Kp;
	ki=Ki*SampleTime; /* SampleTime here must be in seconds since units of kp and kd are (1/s)*/
	kd=Kd/SampleTime;
}

void ControlRW::set_Sample_Time(double NewSampleTime)  /*currently set to 0.001 secs*/
{
	if (NewSampleTime>0)
	{	double ratio=NewSampleTime/SampleTime;
		ki*=ratio;
		kd/=ratio;
		SampleTime=NewSampleTime;
	}
}

void ControlRW::set_cmd_Limits(double Min,double Max) /*currently set to -9 to +9V*/
{
	if (Min>Max) return;
	cmdMin=Min;
	cmdMax=Max;

	if (cmdInV>cmdMax)cmdInV=cmdMax;
	else if(cmdInV<cmdMin)cmdInV=cmdMin;

	if (I_term>cmdMax)I_term=cmdMax;
	else if (I_term<cmdMin)I_term=cmdMin;

}
void ControlRW::compute() //function to be called in each sample time
{
	/*calculating different error components for P,I and D*/
	double error = setpoint - measured; //proportional term

	I_term += (ki*error); //integrating term
	if (I_term>cmdMax)I_term=cmdMax;
	else if (I_term<cmdMin)I_term=cmdMin;

	double d_measured=measured-last_measured; //differential term

	/*computing output of PID as cmd in voltage*/
	cmdInV=kp*error + ki*I_term + kd*d_measured;

	if (cmdInV>cmdMax)cmdInV=cmdMax;
	else if(cmdInV<cmdMin)cmdInV=cmdMin;

	/*take this output, divide by 9.0 and multiply by 100 to give PWM level in percentage)*/

	/*change in values*/
	last_measured=measured;
	}



void ControlRW::encoderInit() {
	 GPIO_InitTypeDef GPIO_InitStructure;
	  // turn on the clocks for each of the ports needed
	  RCC_AHB1PeriphClockCmd (ENCLA_GPIO_CLK, ENABLE);
	  RCC_AHB1PeriphClockCmd (ENCLB_GPIO_CLK, ENABLE);

	  // now configure the pins themselves
	  // they are all going to be inputs with pullups
	  GPIO_StructInit (&GPIO_InitStructure);
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	  GPIO_InitStructure.GPIO_Pin = ENCLA_PIN;
	  GPIO_Init (ENCLA_GPIO_PORT, &GPIO_InitStructure);
	  GPIO_InitStructure.GPIO_Pin = ENCLB_PIN;
	  GPIO_Init (ENCLB_GPIO_PORT, &GPIO_InitStructure);

	  // Connect the pins to their Alternate Functions
	  GPIO_PinAFConfig (ENCLA_GPIO_PORT, ENCLA_SOURCE, ENCLA_AF);
	  GPIO_PinAFConfig (ENCLB_GPIO_PORT, ENCLB_SOURCE, ENCLB_AF);

	  // Timer peripheral clock enable
	  RCC_APB1PeriphClockCmd (ENCL_TIMER_CLK, ENABLE);

	  // set them up as encoder inputs
	  // set both inputs to rising polarity to let it use both edges
	  TIM_EncoderInterfaceConfig (ENCL_TIMER, TIM_EncoderMode_TI12,
	                              TIM_ICPolarity_Rising,
	                              TIM_ICPolarity_Rising);
	  TIM_SetAutoreload (ENCL_TIMER, 0xffffffff);

	  // turn on the timer/counters
	  TIM_Cmd (ENCL_TIMER, ENABLE);

	  encoderReset();
}

void ControlRW::encoderReset() {
	  __disable_irq();
	  oldLeftEncoder = 0;
	  leftTotal = 0;
	  fwdTotal = 0;
	  rotTotal = 0;
	  TIM_SetCounter (ENCL_TIMER, 0);
	  time=SECONDS_NOW();
	  encoderRead();
	  __enable_irq();
}


void ControlRW::encoderRead() {
	dt=SECONDS_NOW()-time;
	time=SECONDS_NOW();
	 oldLeftEncoder = leftEncoder;
	  leftEncoder = TIM_GetCounter (ENCL_TIMER) ;
	  leftCount = leftEncoder - oldLeftEncoder;
	  fwdTotal += fwdCount;
	  rotTotal += rotCount;
	  leftTotal += leftCount;
}


int16_t ControlRW::getRPM() {
	encoderRead();
	return leftCount*60/(16*dt)/4;
}

