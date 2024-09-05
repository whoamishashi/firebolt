		case ATTITUDE_CONTROL_MODE_DOCKING:
			Servo.write(940);
			Magnet.setPins(1);

			dock_dist = iRData.distance;
			dock_angle_radians = iRData.angle;
			dock_angle = dock_angle_radians * 180 / M_PI;

			PRINTF("\nIR_ANGLE: %f", iRData.angle);
			PRINTF("\nIR_DISTANCE: %f", iRData.distance);
			PRINTF("\nIR_RANGE1: %d", iRData.range1);
			PRINTF("\nIR_RANGE2: %d", iRData.range2);

			if (fabs(dock_angle) > 3) {				
				turn(dock_angle / 3);
				compute_ap();
				desiredMS = (int) (cmdInRpm_motor_ap * 1000 / cmdMax_ap);
			} else {
				measured_linp = -dock_dist / 1000;
				compute_linp();