
double set_point[2];
double current[2];
double measured_ap;


double theta_set_point=atan2d((set_point[1]-current[1])/(set_point[0]-current[0])); //angle in range of -180 to 180 deg//

if (theta_set_point<=180 && theta_set_point>=0): theta_set_point=theta_set_point;
else if (theta_set_point>-180 && theta_set_point<0): theta_set_point=theta_set_point+360; //makes it between 0 to 360 deg//

double difference=theta_set_point-measured_ap; //total  delta

if (270>abs(difference)>90):
	setpoint_ap=theta_set_point+180; 	
	if (setpoint_ap>360): setpoint_ap=set_point_ap-360; //ensuring setpoint_ap is less than 360 deg
	thruster=all_thrusters[1,0,0]; //ensuring delta is less than 90 deg//
else setpoint_ap=theta_set_point;
	   thruster=all_thrusters[0,1,1];
	

void turn(double angle)
{	setpoint_ap=measured_ap(now) + angle //only single value for current angle to be taken for measured, not a continuous function//
	
}
	

	
dist=sqrt((set_point[0]-current[0])^2+(set_point[1]-current[1])^2);

measured_linp=-dist

