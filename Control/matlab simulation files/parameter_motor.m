clc
clear all
d1=3.45;%diameter of motor in cm
wt_m1=110; %weight of motor in grams
Vm=12; %max voltage in V
nl_speed1=11000; %no load speed in rpm
I_stall=5; %stall current in Amperes
T_stall1=5; %Stall torque in oz-inch
I_free=0.3; %current at no load in Amperes
dw1=4.9; % reaction wheel outer dia in cm
dw2=4; %reaction wheel inner dia in cm
wt_w1=179; %%weight of reaction wheel in grams
L=0.5; %H
m_s=2.0; %weight of satelite in kg
rho=1.225; %density of air in kg/m^3
d_air_valve=2.5e-3; %diameter of air valve m
A=pi*d_air_valve^2/4; %area of air valve m^2
v_e=32;  %escape velocity of air in m/s
b_lin=0; %linear velocity damping
 

%%Conversions to SI
%Jm=(d1/2)^2*wt_m1*10^-7/2; %moment of inertia of motor (mr^2/2) in kgm^2
Jw=wt_w1*10^-3/2*((dw1/2)^2+(dw2/2)^2)*10^-4; %moment of inertia of Reaction wheel in kgm^2
nl_speed=nl_speed1*2*pi/60; %rad/s
T_stall=T_stall1*0.0070615; %Nm

K_t=T_stall/I_stall; %forward torque gain in Nm/A
R=K_t*Vm/T_stall; %resistance of circuit in ohms
K_b=(Vm-I_free*R)/nl_speed; %back emf gain in V.s/rad
b=I_free*K_t/nl_speed; %damping factor in Nms/rad
J_eq=Jw; %Nm^2
Js=5*Jw;

%Jm
Jw
J_eq
b
R
K_t
K_b
Js

Tf_lin_vel=tf([1],[m_s 0.01])
%Tf_lin_vel=c2d(Tf_lin_vel,0.001,'zoh')


