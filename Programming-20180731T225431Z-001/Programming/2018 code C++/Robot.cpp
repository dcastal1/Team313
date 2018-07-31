//non robot includes
#include <iostream>
#include <memory>
#include <string>
#include <cmath>
//robot related includes
#include <IterativeRobot.h>
#include <LiveWindow/LiveWindow.h>
//#include <SmartDashboard/SendableChooser.h>
//#include <SmartDashboard/SmartDashboard.h>
#include "WPILib.h"
#include <PowerDistributionPanel.h>
#include "ctre/Phoenix.h"
#include "Drive/DifferentialDrive.h"
#include "DriverStation.h"



class Robot: public frc::IterativeRobot {
public:
	WPI_TalonSRX l1 {32};
	WPI_TalonSRX l2 {38};
	WPI_TalonSRX l3 {39};
	WPI_TalonSRX r1 {33};
	WPI_TalonSRX r2 {34};
	WPI_TalonSRX r3 {35};
	WPI_TalonSRX arm1 {36};
	WPI_TalonSRX arm2 {37};

	frc::Joystick joy {0};
	frc::PowerDistributionPanel pdp {0};
	std::shared_ptr<NetworkTable> roboRealm;

	static constexpr int kUltrasonicPort = 1;
	static constexpr int kValueToInches = .125;
	cs::UsbCamera camera;
	bool seen = false;
	bool turned = false;
	Robot(){}
	void RobotInit(){
		cs::UsbCamera camera = CameraServer::GetInstance()->StartAutomaticCapture();
	}

	void AutonomousInit() {
	}

	void TeleopPeriodic() {
		if(joy.GetRawButton(5))
		{
			arm1.Set(.2);
			arm2.Set(.2);
		}
		else if(joy.GetRawButton(6))
		{
			arm1.Set(-.2);
			arm2.Set(-.2);
		}
		if(std::abs(joy.GetRawAxis(5)) > .2 && std::abs(joy.GetRawAxis(1)) < .2)
		{

			TankDrive(0, joy.GetRawAxis(5));//drive the right side only

			//Reads the input voltage of the PDP
			double pdpVin = pdp.GetVoltage();
			printf("input voltage: %f \n", pdpVin);

			//Reads the input amperage of the talons
			double t1 = pdp.GetCurrent(12);
			double t2 = pdp.GetCurrent(13);
			double t3 = pdp.GetCurrent(14);
			double t4 = pdp.GetCurrent(15);

			//printf("t1: %f; t2: %f; t3: %f, t4: %f \n", t1, t2, t3, t4);
			printf("Right side drive ");
		}
		else if(std::abs(joy.GetRawAxis(5)) < .2 && std::abs(joy.GetRawAxis(1)) > .2)
		{
			TankDrive(joy.GetRawAxis(1),0);//drive the left side only
			//Reads the input voltage of the PDP
			double pdpVin = pdp.GetVoltage();
			printf("input voltage: %f \n", pdpVin);

			//Reads the input amperage of the talons
			double t1 = pdp.GetCurrent(12);
			double t2 = pdp.GetCurrent(13);
			double t3 = pdp.GetCurrent(14);
			double t4 = pdp.GetCurrent(15);

			//printf("t1: %f; t2: %f; t3: %f, t4: %f \n", t1, t2, t3, t4);
			printf("Left side drive");
		}
		else if(std::abs(joy.GetRawAxis(5)) > .2 && std::abs(joy.GetRawAxis(1)) > .2)
		{
			TankDrive(joy.GetRawAxis(1),joy.GetRawAxis(5));  //drive both sides
			//Reads the input voltage of the PDP
			double pdpVin = pdp.GetVoltage();
			printf("input voltage: %f \n", pdpVin);

			//Reads the input amperage of the talons
			double t1 = pdp.GetCurrent(12);
			double t2 = pdp.GetCurrent(13);
			double t3 = pdp.GetCurrent(14);
			double t4 = pdp.GetCurrent(15);

			//printf("t1: %f; t2: %f; t3: %f, t4: %f \n", t1, t2, t3, t4);
			printf("Both sides");
		}
		else if(std::abs(joy.GetRawAxis(5)) > .2 && std::abs(joy.GetRawAxis(1)) < .2)
		{
			TankDrive(joy.GetRawAxis(1),joy.GetRawAxis(5));  //drive both sides
			//Reads the input voltage of the PDP
			double pdpVin = pdp.GetVoltage();
			printf("input voltage: %f \n", pdpVin);

			//Reads the input amperage of the talons
			double t1 = pdp.GetCurrent(12);
			double t2 = pdp.GetCurrent(13);
			double t3 = pdp.GetCurrent(14);
			double t4 = pdp.GetCurrent(15);

			//printf("t1: %f; t2: %f; t3: %f, t4: %f \n", t1, t2, t3, t4);
			printf("Both sides");
		}
		else
		{
			TankDrive(0,0);
		}
	}
	void TankDrive(double left, double right)
	{
		l1.Set(-left);
		l2.Set(-left);
		l3.Set(-left);
		r1.Set(right);
		r2.Set(right);
		r3.Set(right);
	}
};

START_ROBOT_CLASS(Robot)
