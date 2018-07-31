//non robot includes
#include <iostream>
#include <memory>
#include <string>
#include <cmath>
#include <cstring>
//robot related includes
#include <IterativeRobot.h>
#include <LiveWindow/LiveWindow.h>
#include <SmartDashboard/SendableChooser.h>
#include <SmartDashboard/SmartDashboard.h>
#include "WPILib.h"
#include <PowerDistributionPanel.h>
#include "ctre/Phoenix.h"

#include "Drive/DifferentialDrive.h"
#include <DriverStation.h>
//#include "Commands/ExampleCommand.h"
//#include "CommandBase.h"
Compressor *compressor;
BuiltInAccelerometer accelerometer;
ADXRS450_Gyro gyro;


class Robot: public frc::IterativeRobot {
public:
	//Drive Train
	WPI_TalonSRX l1 {32};
	WPI_TalonSRX l2 {38};
	WPI_TalonSRX l3 {39};
	WPI_TalonSRX r1 {33};
	WPI_TalonSRX r2 {34};
	WPI_TalonSRX r3 {35};
	WPI_TalonSRX arm1 {36};
	WPI_TalonSRX arm2 {37};
	WPI_TalonSRX w1 {41};
	WPI_TalonSRX w2 {30};
	WPI_TalonSRX w3 {31};
	WPI_TalonSRX m1 {40};
	WPI_TalonSRX m2 {42};
	frc::Joystick joy {0};
	frc::Joystick OP {1};
	DoubleSolenoid/* *left, *right,*/ *scizor,*climber;
	Solenoid *left,*right;
	frc::PowerDistributionPanel pdp {0};
	std::shared_ptr<NetworkTable> roboRealm;
	Encoder *enc;
	DigitalInput *SL;
	std::unique_ptr<frc::Command> autonomousCommand;
	frc::SendableChooser<frc::Command*> chooser;

	const double kUpdatePeriod = 0.005; // 5milliseconds / 0.005 seconds.
	static constexpr int kUltrasonicPort = 1;
	static constexpr int kValueToInches = .125;
	cs::UsbCamera camera;
	bool seen = false;
	bool rotation = false;
	int auton = 0;
	int autonMax = 4;
	bool delay = false;
	double maxSpd = 1;
	double joyLC = 0;
	double joyRC = 0;
	Robot(){
		compressor = new Compressor(0);
	//	right = new DoubleSolenoid(2,3);
		right = new Solenoid(1);

		left = new Solenoid(0);
		scizor = new DoubleSolenoid(2,3);
		climber = new DoubleSolenoid(4,6);
		SL = new DigitalInput(2);

		enc = new Encoder(0, 1, false, Encoder::EncodingType::k4X);

	}
	void RobotInit() /*: accelerometer(Accelerometer::Range::kRange_8G)*/{
		cs::UsbCamera camera = CameraServer::GetInstance()->StartAutomaticCapture();
		compressor->SetClosedLoopControl(true);
		//chooser.AddDefault("Center Start", new OffensizeCommand());
		//chooser.AddObject("My Auto", new DefensiveCommand());
	}

	void AutonomousInit()
	{
		enc->SetMaxPeriod(.1);
		enc->SetMinRate(10);
		enc->SetDistancePerPulse(5);
		enc->SetReverseDirection(true);
		enc->SetSamplesToAverage(7);
		enc->Reset();
		std::string gameData;
		gameData = frc::DriverStation::GetInstance().GetGameSpecificMessage();
		std::string startPos = std::to_string(gameData[0]);
		double distance;
		TankDrive(0,0);
		Wait(.5);
		Autonomous(startPos);
	}
	double CorrectJoystick(double myJoystick)
	{
		if(std::abs(myJoystick) <= .18)
		{
			return 0;
		}
		else if(std::abs(myJoystick) > .18 && std::abs(myJoystick) < maxSpd)
		{
			return myJoystick;
		}
		else if(std::abs(myJoystick) >= maxSpd)
		{
			if(myJoystick < 0)
				return -maxSpd;
			else if(myJoystick > 0)
				return maxSpd;
		}
	}
	double ToTarget(double target, double current)
	{
		SmartDashboard::PutString("DB/String 0", std::to_string(target));
		SmartDashboard::PutString("DB/String 1", std::to_string(current));
		if((target - current) < 0)
		{
			return current -= .09;
		}
		else if((target - current) > 0)

		{
			return current += .09;
		}
		else if((target - current) == 0)
		{
			return 0;
		}
	}
	void DisabledInit() override
	{
		delay = false;
		SmartDashboard::PutString("DB/String 5","not set");
		SmartDashboard::PutString("DB/String 6","");
		SmartDashboard::PutString("DB/String 7","");
		SmartDashboard::PutString("DB/String 8", "Delay off");
	}
	void DisabledPeriodic() override
	{
		if(joy.GetRawButton(1))
		{
			delay = true;
			SmartDashboard::PutString("DB/String 8","Delay on");
		}
		if(joy.GetRawButton(6))
		{
			auton++;
			Wait(.2);
			if(auton == autonMax + 1)
			{
				auton = 0;
			}
			else if(auton == -1)
			{
				auton = 4;
			}
		}
		else if(joy.GetRawButton(5))
		{
			auton--;
			std::cout<<auton<<"auton"<<std::endl;
			Wait(.2);
			if(auton == autonMax + 1)
			{
				auton = 0;
			}
			else if(auton == -1)
			{
				auton = 4;
			}
		}


		SmartDashboard::PutString("DB/String 5","Auton mode: " + std::to_string(auton));
		printf("auton mode %s \n",std::to_string(auton).c_str());
		std::string gameData;
		gameData = frc::DriverStation::GetInstance().GetGameSpecificMessage();
		std::string startPos = std::to_string(gameData[0]);


		if(auton == 0)
		{
			SmartDashboard::PutString("DB/String 7","Left start");
		}
		else if(auton == 1)
		{
			SmartDashboard::PutString("DB/String 7","Center start");
		}
		else if(auton == 2)
		{
			SmartDashboard::PutString("DB/String 7","Right start");
		}
		else if(auton == 3)
		{
			SmartDashboard::PutString("DB/String 6","asdf");
			SmartDashboard::PutString("DB/String 7","Any start");
		}
		else if(auton == 4)
		{
			SmartDashboard::PutString("DB/String 7","side start and a hope");
		}

	}
	void TeleopInit()
	{
		arm1.SetSelectedSensorPosition(0,0,0);
	}
	void TeleopPeriodic() {
		SL->Get();
		SmartDashboard::PutBoolean("DB/LED", SL);
		//double maxSpeed = 1 //Multiplies the joystick movement by a number(for example .5) which changes the max speed

		joyLC = ToTarget(CorrectJoystick(joy.GetRawAxis(1)), joyLC)/*maxSpeed*/;
		joyRC = ToTarget(CorrectJoystick(joy.GetRawAxis(5)), joyRC)/*maxSpeed*/;
		TankDrive(joyLC, joyRC); //this section is the ramp drive

		//TankDrive(joy.GetRawAxis(1), joy.GetRawAxis(5));//this is non ramp drive
		if(joy.GetRawButton(0))
		{
			maxSpd = .9;
		}
		else if(joy.GetRawButton(1))
		{
			maxSpd = .8;
		}
		else if(joy.GetRawButton(2))
		{
			maxSpd = .7;
		}
		if(OP.GetRawAxis(2) > .25)
		{
			arm1.Set(.65);
			arm2.Set(.65);

		}
		else if(OP.GetRawAxis(3) > .25)
		{
			arm1.Set(-.65);
			arm2.Set(-.65);
		}
		else
		{
			arm1.Set(0);
			arm2.Set(0);
		}
		if(OP.GetRawAxis(5) > .2)
		{
			w1.Set(-1);
			w2.Set(-1);
			w3.Set(-1);
		}
		else if(OP.GetRawAxis(5) < -.2)
		{
			w1.Set(1);
			w2.Set(1);
			w3.Set(1);
		}
		else
		{
			w1.Set(0);
			w2.Set(0);
			w3.Set(0);
		}
		if(joy.GetRawButton(5))
		{
			climber->Set(DoubleSolenoid::kForward);
		}
		if(joy.GetRawButton(6))
			climber->Set(DoubleSolenoid::kForward);

		if(OP.GetRawButton(5))
		{
		scizor->Set(DoubleSolenoid::kForward);
		}
		else if(OP.GetRawButton(6))
		{
			scizor->Set(DoubleSolenoid::kReverse);




		}
		else
		{
			scizor->Set(DoubleSolenoid::kOff);
		}/*
		SmartDashboard::PutString("DB/String 0", std::to_string(pdp.GetCurrent(4)));
		SmartDashboard::PutString("DB/String 1", std::to_string(pdp.GetCurrent(5)));
		SmartDashboard::PutString("DB/String 2", std::to_string(pdp.GetCurrent(6)));
		SmartDashboard::PutString("DB/String 3", std::to_string(pdp.GetCurrent(7)));
		SmartDashboard::PutString("DB/String 4", std::to_string(pdp.GetCurrent(8)));
		SmartDashboard::PutString("DB/String 5", std::to_string(pdp.GetCurrent(9)));
		SmartDashboard::PutString("DB/String 6", std::to_string(pdp.GetCurrent(10)));
		SmartDashboard::PutString("DB/String 7", std::to_string(pdp.GetCurrent(11)));
		SmartDashboard::PutString("DB/String 8", std::to_string(enc->GetDistance()));
		SmartDashboard::PutString("DB/String 9", std::to_string(arm1.GetSelectedSensorPosition(0)));
		*/
		if(OP.GetRawButton(4))
		{
			m1.Set(1);
			m2.Set(-1);

		}
		else if(OP.GetRawButton(3))
		{
			m1.Set(-1);
			m2.Set(1);
		}
		else if(OP.GetRawButton(1))
		{
			m1.Set(1);
			m2.Set(1);
		}
		else if(OP.GetRawButton(2))
		{
			m1.Set(-1);
			m2.Set(-1);
		}
		else
		{
			m1.Set(0);
			m2.Set(0);
		}

		if(OP.GetRawAxis(1) > .2)
		{
			right->Set(true);
			left->Set(false);
			SmartDashboard::PutString("DB/String 2", "In A");
		}
		else if(OP.GetRawAxis(1) < -.2)
		{
			left->Set(true);
			right->Set(false);
			SmartDashboard::PutString("DB/String 2", "In B");
		}
		//1,2, 12, 13, 14, 15

	}
	void TankDrive(double left, double right)
	{
		//l1.
		l1.Set(-left);
		l2.Set(-left);
		l3.Set(-left);
		r1.Set(right);
		r2.Set(right);
		r3.Set(right);
	}

	void Autonomous(std::string lightPos)
	{
		right->Set(true);
		left->Set(false);
		std::string gameData;
		gameData = frc::DriverStation::GetInstance().GetGameSpecificMessage();
		std::string startPos = std::to_string(gameData[0]);
		/*TankDrive(-.3,-.3);
		Wait(2);
		TankDrive(0,0);*/
		SmartDashboard::PutString("DB/String 1", std::to_string(enc->GetDistance()));
		if(gameData[0] == 'L')
		{
			if(auton == 0)
			{
				driveFrwd(5000);
				shoot();
			}
			else if(auton == 1)
			{
				driveFrwd(3000);
				turnLeft();
				driveFrwd(2500);
				turnRight();
				driveFrwd(3500);
				shoot();
			}
			else if(auton == 2)
			{
				driveFrwd(3000);
				turnLeftLow();
				driveFrwd(7000);
				turnRightLow();
				driveFrwd(2000);
				shoot();
			}
			else if(auton == 3)
			{
				TankDrive(-.3,-.3);
				Wait(2);
				TankDrive(0,0);
			}
		}

		else if(gameData[0] == 'R')
		{
			if(auton == 0)
			{
				driveFrwd(3000);
				turnRightLow();
				driveFrwd(5000);
				turnLeftLow();
				driveFrwd(3500);
				shoot();
			}
			else if(auton == 1)
			{
				driveFrwd(3000);
				turnRightLow();
				driveFrwd(4000);
				turnLeftLow();
				driveFrwd(2000);
				shoot();
			}
			else if(auton == 2)
			{
				driveFrwd(5000);
				shoot();
			}
			else if(auton == 3)
			{
				TankDrive(-.3,-.3);
				Wait(2);
				TankDrive(0,0);
			}
		}

		if(gameData[1] == 'R')
		{
			if(auton == 4)
			{
				right->Set(true);
				left->Set(false);
				m1.Set(-1);
				m2.Set(1);
				driveFrwd(7500);
				hardShoot();

			}
		}
		else if(gameData[1] == 'L')
		{
			if(auton == 4)
			{
				right->Set(true);
				left->Set(false);
				driveFrwd(7500);
				hardShoot();
			}
		}

	}
	void driveFrwd(double dist)
	{
		resetDriveEnc();
		while(enc->GetDistance() < dist && IsAutonomous() == true)
		{
			SmartDashboard::PutString("DB/String 0", "distance " + std::to_string(enc->GetDistance()));
			TankDrive(-.6,-.6);
		}
		TankDrive(0,0);
	}
	void turnLeftLow()
	{
		gyro.Reset();
		while(gyro.GetAngle() > -50 && IsAutonomous() == true)
		{
			SmartDashboard::PutString("DB/String 1", "left turn" + std::to_string(gyro.GetAngle()));
			TankDrive(.65,-.65);
		}
	}
	void turnRightLow()
	{
		gyro.Reset();
		while(gyro.GetAngle() < 50 && IsAutonomous() == true)
		{
			SmartDashboard::PutString("DB/String 1", "right turn" + std::to_string(gyro.GetAngle()));
			TankDrive(-.65,.65);
		}
	}
	void turnLeft()
	{
		gyro.Reset();
		while(gyro.GetAngle() > -75 && IsAutonomous() == true)
		{
			SmartDashboard::PutString("DB/String 1", "left turn" + std::to_string(gyro.GetAngle()));
			TankDrive(.65,-.65);
		}
	}
	void turnRight()
	{
		gyro.Reset();
		while(gyro.GetAngle() < 75 && IsAutonomous() == true)
		{
			SmartDashboard::PutString("DB/String 1", "right turn" + std::to_string(gyro.GetAngle()));
			TankDrive(-.65,.65);
		}
	}
	void resetDriveEnc()
	{
		SmartDashboard::PutString("DB/String 9","Enc reset");
		enc->Reset();
	}
	void shoot()
	{
		Wait(.5);
		m1.Set(1);
		m2.Set(-1);
		Wait(1);
		m1.Set(0);
		m2.Set(0);
	}
	void hardShoot()
	{
		arm1.Set(1);
		arm2.Set(1);
		Wait(.32);
		left->Set(true);
		right->Set(false);
		m1.Set(1);
		m2.Set(-1);
		Wait(1);
		m1.Set(0);
		m2.Set(0);
		arm1.Set(0);
		arm2.Set(0);
		TankDrive(0,0);
	}
	void hardDriveFrwd(double dist)
	{
		resetDriveEnc();
		while(enc->GetDistance() < dist && IsAutonomous() == true)
		{
			SmartDashboard::PutString("DB/String 0", "distance " + std::to_string(enc->GetDistance()));
			TankDrive(-.6,-.6);
		}
	}
};
START_ROBOT_CLASS(Robot)
