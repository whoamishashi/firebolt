dock_dist=iRData.distance;

dock_angle_radians=iRData.angle;
dock_angle=dock_angle_radians*180/M_PI;

if (fabs(dock_angle) > 3) {
	turn(dock_angle / 3);

	compute_ap();
	desiredMS = (int) (cmdInRpm_motor_ap * 1000 / cmdMax_ap);
	} else {
	measured_linp = -dock_dist / 1000;
	compute_linp();
	
dist=-dock_dist;
compute_linp;
	