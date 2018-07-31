#include "WPILib.h"

#ifndef MYROBOT_H
#define MYROBOT_H

//the reason for having a header file is for gathering together all of the special numbers that don't change throughout the code so it doesn't confuse the reader.
//additionally, having this helps a lot when it comes to making quick changes, especially when you are trying to get the code to match up with how the robot is wired.

//naming convention I use:
//<type>_<name>_<description>
//when naming drivetrain motors: "M" for motor, "LR" for left rear location on drivetrain, "SLOT" for which digital sidecar, and "CHANNEL" for where the PWM cable is plugged in that digital sidecar
//when naming joysticks/controllers: "J" for joystick, "L" for left joystick, "USB" for the USB number slot when plugged into a laptop
//when naming encoders: "E" for encoder, "L" for left drivetrain encoder, "ACHANNEL" for where on the DIO of the digital sidecar the alpha channel is plugged in.
//For understanding how encoders work, see this: http://wpilib.screenstepslive.com/s/3120/m/7912/l/85770-measuring-rotation-of-a-wheel-or-other-shaft-using-encoders

//MOTORS

//Drivetrain Motors

//left side

const int M_LR_SLOT = 1;
const int M_LR_CHANNEL = 1;
const int M_LC_SLOT = 1;
const int M_LC_CHANNEL = 2;
const int M_LF_SLOT = 1;
const int M_LF_CHANNEL = 3;

//right side

const int M_RR_SLOT = 1;
const int M_RR_CHANNEL = 4;
const int M_RC_SLOT = 1;
const int M_RC_CHANNEL = 5;
const int M_RF_SLOT = 1;
const int M_RF_CHANNEL = 6;

//Misc. Motors

//shooter motors

const int M_SHOOTER_01_SLOT = 2;
const int M_SHOOTER_01_CHANNEL = 1;
const int M_SHOOTER_02_SLOT = 2;
const int M_SHOOTER_02_CHANNEL = 2;

//intake motor

const int M_INTAKE_SLOT = 2;
const int M_INTAKE_CHANNEL = 5;


//JOYSTICKS

const int J_L_USB = 1;
const int J_R_USB = 2;
const int J_C_USB = 3;


//ENCODERS

//left encoder

const int E_L_ACHANNEL = 3;
const int E_L_BCHANNEL = 4;
const int E_L_DISTPERPULSE = 1;
const double E_L_MAXPERIOD = 1.0;
const bool E_L_REVERSE = false;

//right encoder

const int E_R_ACHANNEL = 1;
const int E_R_BCHANNEL = 2;
const int E_R_DISTPERPULSE = 1;
const double E_R_MAXPERIOD = 1.0;
const bool E_R_REVERSE = true;

#endif
