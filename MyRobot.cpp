/*
 * FRC Team 2879 Robot Code
*   HARDWARE CONFIGURATION
*   Device:                      Connection:
*   --------------------------------------------------
*   (jag(talon)) Back Right             (Sidecar) PWM port #1 
*   (jag(talon)) Back Left              (Sidecar) PWM port #2
*   (spike) Compressor           (Sidecar) Relay port #1
*   Digital Pressure Sensor      (Sidecar) Digital Input Port #1
*   USB Xbox controller          (Laptop)  USB port #1 (Usin it for real now. This code will not work with a standard joystick)
*/


#include "WPILib.h"


#define Button_X 1
#define Button_Y 4
#define Button_A 2
#define Button_B 3
#define Button_START 10
#define Button_BACK 9
#define Button_RIGHT_BUMPER 6
#define Button_RIGHT_TRIGGER 8
#define Button_LEFT_BUMPER 5
#define Button_LEFT_TRIGGER 7

#define Stick_LEFT_Y 2
#define Stick_LEFT_X 1
#define Stick_RIGHT_X 4
#define Stick_RIGHT_Y 5


class River : public SimpleRobot
{
    RobotDrive River_Drive; // robot drive system
	Joystick stick; // only joystick
	DriverStationLCD* userDisplay;
	float moveL;
	float spinL;
	float moveR;
	float spinR;
	bool SquaredInputs;
	bool DriveToggle;
	bool AltDrive;

public:
	River(void):
		River_Drive(1, 2),	// these must be initialized in the same order
		stick(1)		// as they are declared above.
	{
		River_Drive.SetExpiration(0.1);
		 userDisplay  = DriverStationLCD::GetInstance();
	}


	void clearlineOne(void) {   // Clears line #1
	       userDisplay->Printf(DriverStationLCD::kUser_Line1, 1, "                   ");
	       userDisplay->UpdateLCD();
	    }
	    void clearlineTwo(void) {   // Clears line #2
	       userDisplay->Printf(DriverStationLCD::kUser_Line2, 1, "                   ");
	       userDisplay->UpdateLCD();
	    }
	    void clearlineThree(void) {   // Clears line #3
	       userDisplay->Printf(DriverStationLCD::kUser_Line3, 1, "                   ");
	       userDisplay->UpdateLCD();
	    }
	    void clearlineFour(void) {   // Clears line #4
	       userDisplay->Printf(DriverStationLCD::kUser_Line4, 1, "                   ");
	       userDisplay->UpdateLCD();
	    }
	    void clearlineFive(void) {   // Clears line #5
	       userDisplay->Printf(DriverStationLCD::kUser_Line5, 1, "                   ");
	       userDisplay->UpdateLCD();
	    }
	    void clearlineSix(void) {   // Clears line #6
	       userDisplay->Printf(DriverStationLCD::kUser_Line6, 1, "                   ");
	       userDisplay->UpdateLCD();
	    }
	    
	void Autonomous(void)
	{
      // no auton mode as of yet
	}

	
	void OperatorControl(void)
	{
		River_Drive.SetSafetyEnabled(true);
		while (IsOperatorControl())
		{
			
			
			if (stick.GetRawButton(Button_LEFT_TRIGGER)) {
				AltDrive=true;
			}
			else {
				AltDrive=false;
			}
			
			// Sets squared inputs for driving
			if (stick.GetRawButton(Button_RIGHT_TRIGGER)){
			    SquaredInputs=true;
			}
		    else {
				SquaredInputs=false;
		    }
			// Updates variables
			if (SquaredInputs) {
			    moveL = (stick.GetRawAxis(Stick_LEFT_Y));  //axis 2
		      	spinL = (stick.GetRawAxis(Stick_LEFT_X));  //axis 1
				moveR = (stick.GetRawAxis(Stick_RIGHT_Y)); //axis 5
			    spinR = (stick.GetRawAxis(Stick_RIGHT_X)); //axis 4
			}
			else {
				moveL = ((stick.GetRawAxis(Stick_LEFT_Y)) / 2);
				spinL = ((stick.GetRawAxis(Stick_LEFT_X)) / 2);
				moveR = ((stick.GetRawAxis(Stick_RIGHT_Y)) / 2); 
				spinR = ((stick.GetRawAxis(Stick_RIGHT_X)) / 2);
			}
			    
						    // Right now, we are unable to get valid output for axis 4.
			
			// Updates LCD Display with variables
			clearlineOne();
		    clearlineTwo();
		    clearlineThree();
		    clearlineFour();
		    clearlineFive();
		    userDisplay->Printf(DriverStationLCD::kUser_Line1, 1, "MoveL: %d", (int) (moveL*100));
		    userDisplay->Printf(DriverStationLCD::kUser_Line2, 1, "MoveR:%d", (int) (moveR*100));
		    userDisplay->Printf(DriverStationLCD::kUser_Line3, 1, "SpinL: %d", (int) (spinL*100));
		    userDisplay->Printf(DriverStationLCD::kUser_Line4, 1, "SpinR: %d", (int) (spinR*100));
		    if (SquaredInputs){
		      userDisplay->Printf(DriverStationLCD::kUser_Line5, 1, "squared inputs active");
		    }
		    else {
		    	clearlineFive(); 
		    	userDisplay->UpdateLCD();
		    }
			userDisplay->UpdateLCD();
			
			
			 
			// Drives Robot
			 if (AltDrive){
		       River_Drive.ArcadeDrive(moveL, spinR, SquaredInputs);
			 }
			 else {
			   River_Drive.ArcadeDrive(moveL, spinL, SquaredInputs); // drive with arcade style (use right stick)
			 }
			
			 
			Wait(0.005);				// wait for a motor update time
		}
	}
	
	/**
	 * Runs during test mode nOT WORKING RITE NAOW DON"T USE PLOX
	 */
	void Test() {
		while (IsTest()) {
		   moveL = (stick.GetRawAxis(Stick_LEFT_Y));
	       spinL = (stick.GetRawAxis(Stick_LEFT_X));
	       moveR = (stick.GetRawAxis(Stick_RIGHT_Y));
	       spinR = (stick.GetRawAxis(Stick_RIGHT_X));
		
	       userDisplay->Clear();
	       userDisplay->Printf(DriverStationLCD::kUser_Line2, 1, "MoveL: %d", (int) (moveL*100));
           userDisplay->Printf(DriverStationLCD::kUser_Line2, 12, "MoveR:%d", (int) (moveR*100));
	       userDisplay->Printf(DriverStationLCD::kUser_Line3, 1, "SpinL: %d", (int) (spinL*100));
	       userDisplay->Printf(DriverStationLCD::kUser_Line3, 12, "SpinR: %d", (int) (spinR*100));
	       if (SquaredInputs){
	 		 userDisplay->Printf(DriverStationLCD::kUser_Line4, 1, "squared inputs active");
	 	   }
	       userDisplay->UpdateLCD();
	 
	 Wait(0.005);
		}
	}
};

START_ROBOT_CLASS(River);

