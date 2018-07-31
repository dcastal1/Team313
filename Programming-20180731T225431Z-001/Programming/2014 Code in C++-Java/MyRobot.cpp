#include "MyRobot.h"

class RobotDemo : public IterativeRobot {
public:
	//variables are declared here
	
	//drivetrain motors
	Victor *leftRear, *leftCenter, *leftFront, *rightRear, *rightCenter, *rightFront;
	
	//misc. motors (motors not associated with the drivetrain)
	Victor *shooterMotor01, *shooterMotor02;
	Victor *intakeMotor;
	
	//joysticks and controller
	//used for controlling the robot in the teleoperated period of each match
	Joystick *leftJoy;
	Joystick *rightJoy;
	Joystick *controller;
	
	//encoders
	//these sensors calculate the distance each side of the drivetrain has traveled
	Encoder *leftEncoder;
	Encoder *rightEncoder;
	
	//driver station output
	DriverStationLCD *dsLCD;
	
	//smartdashboard test mode
	//when the robot is enabled in test mode, the smartdashboard allows you to set the speed of each motor independently and display information for each sensor:
	//useful for making sure all of the code works independently and is ready to run together
	LiveWindow *lw; 

	RobotDemo() {
		//each variable is assigned a value here using the constants made in MyRobot.h
		
		leftRear = new Victor(M_LR_SLOT, M_LR_CHANNEL);		//The slot is which digital sidecar it is, and the channel is where the PWM cable is plugged in on the digital sidecar
		leftCenter = new Victor(M_LC_SLOT, M_LC_CHANNEL);
		leftFront = new Victor(M_LF_SLOT, M_LF_CHANNEL);
		rightRear = new Victor(M_RR_SLOT, M_RR_CHANNEL);
		rightCenter = new Victor(M_RC_SLOT, M_RC_CHANNEL);
		rightFront = new Victor(M_RF_SLOT, M_RF_CHANNEL);
		
		shooterMotor01 = new Victor(M_SHOOTER_01_SLOT, M_SHOOTER_01_CHANNEL);
		shooterMotor02 = new Victor(M_SHOOTER_02_SLOT, M_SHOOTER_02_CHANNEL);
		intakeMotor = new Victor(M_INTAKE_SLOT, M_INTAKE_CHANNEL);
		
		leftJoy = new Joystick(J_L_USB);
		rightJoy = new Joystick(J_R_USB);
		controller = new Joystick(J_C_USB);
		
		leftEncoder = new Encoder(E_L_ACHANNEL, E_L_BCHANNEL, E_L_REVERSE);
		leftEncoder->SetDistancePerPulse(E_L_DISTPERPULSE);
		leftEncoder->SetMaxPeriod(E_L_MAXPERIOD);
		leftEncoder->Start();
		rightEncoder = new Encoder(E_R_ACHANNEL, E_R_BCHANNEL, E_R_REVERSE);
		rightEncoder->SetDistancePerPulse(E_R_DISTPERPULSE);
		rightEncoder->SetMaxPeriod(E_R_MAXPERIOD);
		rightEncoder->Start();
		
		dsLCD = DriverStationLCD::GetInstance();
		
		lw = LiveWindow::GetInstance();
		lw->AddActuator("Drivetrain", "Left Rear", leftRear);
		lw->AddActuator("Drivetrain", "Left Center", leftCenter);
		lw->AddActuator("Drivetrain", "Left Front", leftFront);
		lw->AddActuator("Drivetrain", "Right Rear", rightRear);
		lw->AddActuator("Drivetrain", "Right Center", rightCenter);
		lw->AddActuator("Drivetrain", "Right Front", rightFront);
		lw->AddSensor("Drivetrain", "Left Encoder", leftEncoder);
		lw->AddSensor("Drivetrain", "Right Encoder", rightEncoder);
		lw->AddActuator("Shooter", "Shooter 01", shooterMotor01);
		lw->AddActuator("Shooter", "Shooter 02", shooterMotor02);
		lw->AddActuator("Intake", "Intake", intakeMotor);
	}
	
	void RobotDemo::RobotInit() {
		
	}
	
	void RobotDemo::AutonomousInit() {
		leftEncoder->Reset();
		rightEncoder->Reset();
		leftEncoder->Start();
		rightEncoder->Start();
	}
	
	void RobotDemo::AutonomousPeriodic() {
		
	}
	
	void RobotDemo::TeleopInit() {
		leftEncoder->Reset();
		rightEncoder->Reset();
		leftEncoder->Start();
		rightEncoder->Start();
	}
	
	void RobotDemo::TeleopPeriodic() {
		TankDrive(-leftJoy->GetRawAxis(2), -rightJoy->GetRawAxis(2));
		if (controller->GetRawButton(2))
		{
			shooterMotor01->Set(1.0);
			shooterMotor02->Set(1.0);
		}
		
		if (controller->GetRawButton(3))
		{
			intakeMotor->Set(1.0);
		}
	}
	
	void RobotDemo::TestInit() {
		
	}
	
	void RobotDemo::TestPeriodic() {
		lw->Run();
	}
	
	void RobotDemo::TankDrive(double left, double right) {
		leftRear->Set(left);
		leftCenter->Set(left);
		leftFront->Set(left);
		
		rightRear->Set(right);
		rightCenter->Set(right);
		rightFront->Set(right);
		
		
		SmartDashboard::PutNumber("Left Encoder", leftEncoder->Get());
		SmartDashboard::PutNumber("Right Encoder", rightEncoder->Get());
		
		dsLCD->Printf(DriverStationLCD::kUser_Line1, 1, "Left: %f", left);
		dsLCD->Printf(DriverStationLCD::kUser_Line2, 1, "Right: %f", right);
		dsLCD->UpdateLCD();
	}
};

START_ROBOT_CLASS(RobotDemo);

