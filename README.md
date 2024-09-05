# Overview
A fully controlled 3 DoF satellite prototype - two translational and one rotational DoF.

# Missions
To traverse through a predetermined path with a changing predetermined orientation.
To navigate to and dock/undock with another satellite and/or stationary space station.

# Subsystems
Mechanical: Frame and Docking Mechanism, adequate component distribution.
Electronics: Interface and Power distribution.
Embedded: Working Board STM32F4 & AHRS (Attitude and Heading Reference System). Sensors/actuators calibration, Thruster, Reaction-Wheel, Floaters, Camera, Infra-Red Sensors, Star-Tracker, UWB positioning
Ground station: Communication link with satellite, subsystem statuses, telecommand/telemetry, guidance-navigation-control. 
Ob-board software: Realtime on-board, embedded and threaded software for real-time communication, positioning, Guidance-Navigation-Control and target docking.
Control systems: Identify Control Variables and their sensors/actuators, Derive Mathematical models and configure initial controller, Open loop testing with hardware (calibration), Gain tuning, Finalizing sensor weightage
Closed loop testing.

Contributors: Shashikant Gupta, Marco Sanroman, Felix Hessinger, Jordi Oller Sànchez, Neha Chohan.
