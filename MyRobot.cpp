/*
*   FRC Team 2879 Robot Code
*
*   See the README file for more information.
*/

#include "WPILib.h"

//Logitech Gamepad Button Maping
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
 // Controller Axis(s)
 #define Stick_LEFT_Y 2
 #define Stick_LEFT_X 1
 #define Stick_RIGHT_X 4
 #define Stick_RIGHT_Y 5

class River : public SimpleRobot
{
	// Motors / drivetrain
     RobotDrive River_Drive; 
     Jaguar feed; // controls the motor that feeds the shooter
     Jaguar shoot_one;
     Jaguar shoot_two;
    // Pneumatics
     Compressor Comp;
     Solenoid Billy;
     Solenoid Sally;
    // IO devices.
     Joystick stick; 
     DriverStationLCD* userDisplay;
    
    // Config Values
     int HANDICAP; //Value between 1 and ?. Limits max throttle to 1/2, 1/3, 1/4, etc.
     int SQUARE; //Value between 1 and 4. Higher number decreases sensativity at lower speeds.
    // Joystick Input Variables
     float moveL;
     float spinL;
     float moveR;
     float spinR;
    // Toggle or State Tracking variables
     bool ShooterState;  // State of the shooter wheels 
     bool PistonState;   // State of the frisbee feed piston
     bool PistonUpdated;
     bool lastBXstate;
     bool lastBYstate;
     bool lastRTstate;
     bool lastLTstate;

public:
	River(void):
	//// these must be initialized in the same order that they are declared above.
	  // Motors / Drivetrain
	     River_Drive(1,2),
	     feed(3),
	     shoot_one(4),
	     shoot_two(5),
	  // Pneumatics
	     Comp(2,1),
	     Billy(1),
	     Sally(2),
	  // IO Devices
	     stick(1)
	//~~~~~~~~~~~~~~~~~~~~~~~~ INITIALIZATION ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
     {
  	
   	 River_Drive.SetExpiration(0.1);
     Comp.Start();

     ShooterState = false; // Should have the shooter OFF upon robot enabled. if not, you done goofed.
     PistonState = false; // Should leave the chamber area OPEN. If not, switch wires on solenoid
     PistonUpdated = false;
     Billy.Set(false);
     Sally.Set(false);
     HANDICAP = 1;
     SQUARE = 1;

     Wait(0.5); // Wait for camera to boot up
     AxisCamera &Camera = AxisCamera::GetInstance("10.28.79.11");

     userDisplay = DriverStationLCD::GetInstance();
     userDisplay->Clear();
     userDisplay->Printf(DriverStationLCD::kUser_Line1, 1, "Robot Initialized. . .");
     userDisplay->Printf(DriverStationLCD::kUser_Line2, 1, "HANDICAP: 1/%d", (int) HANDICAP);
     userDisplay->Printf(DriverStationLCD::kUser_Line3, 1, "SQUARE: %d", (int) SQUARE);
     userDisplay->UpdateLCD();
     }
    //~~~~~~~~~~~~~~~~~~~~~~~~ DISPLAY CODE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
     void clearline1(void) {
     userDisplay->Printf(DriverStationLCD::kUser_Line1, 1, "                   ");  
     userDisplay->UpdateLCD();
     }
     void clearline2(void) {
     userDisplay->Printf(DriverStationLCD::kUser_Line2, 1, "                   ");  
     userDisplay->UpdateLCD();
     }   
     void clearline3(void) {
     userDisplay->Printf(DriverStationLCD::kUser_Line3, 1, "                   ");  
     userDisplay->UpdateLCD();
     }
     void clearline4(void) {
     userDisplay->Printf(DriverStationLCD::kUser_Line4, 1, "                   ");  
     userDisplay->UpdateLCD();
     }
     void clearline5(void) {
     userDisplay->Printf(DriverStationLCD::kUser_Line5, 1, "                   ");  
     userDisplay->UpdateLCD();
     }
     void clearline6(void) {
     userDisplay->Printf(DriverStationLCD::kUser_Line6, 1, "                   ");  
     userDisplay->UpdateLCD();
     }
    //~~~~~~~~~~~~~~~~~~~~~~~~ SHOOTER FUNCTIONS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

     void Reload(void) {
 	  // Open Piston
 	     if(PistonState) {
 	        PistonToggle();
 	        PistonUpdate();
 	     } else if(!PistonState) {
 	        clearline3();
 	        userDisplay->Printf(DriverStationLCD::kUser_Line3, 1, "Piston Error (R)");
 	        userDisplay->UpdateLCD();
 	     }
 	  // Feed new frisbee
 	     Wait(0.05);
 	     feed.Set(0.26);
 	     Wait(0.15);
 	     feed.Set(0.0);
     }

     void ShooterToggle (void) {
      // Toggles Shooter State
         if(ShooterState) {
         	ShooterState = false;
         } else {
         	ShooterState = true;
         }
         ShooterUpdate();
     }

     void PistonToggle (void) {
      // Toggles Piston State
         if(!PistonState) {
         	PistonState = true;
         } else {
         	PistonState = false;
         }
     }

     void ShooterUpdate(void) {
      // Update Shooter State
 	     if(ShooterState) {
            shoot_one.Set(.56);
            shoot_two.Set(.56);
 	     } else {
            shoot_one.Set(0.0);
 	        shoot_two.Set(0.0);
 	     } 
     }
     
     void PistonUpdate(void) {
      // Update Piston State
     	 if(PistonState && !PistonUpdated) {
     	    Billy.Set(true);
     	    Wait(0.15);
     	    Billy.Set(false);
     	 } else if(!PistonState && !PistonUpdated) {
     	    Sally.Set(true);
     	    Wait(0.15);
     	    Sally.Set(false);
     	 }
     	 PistonUpdated = true;
     } 
    //~~~~~~~~~~~~~~~~~~~~~~~~ AUTON CODE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
     void Autonomous(void) {
     	// No auton mode as of yet
     }
    //~~~~~~~~~~~~~~~~~~~~~~~~ OPERATOR CONTROL ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
     void OperatorControl(void) {
      // Operator Startup
     	 River_Drive.SetSafetyEnabled(true);
     	 ShooterState = false;
     	 ShooterUpdate();
      // Operator Control Loop
     	 while (IsOperatorControl())
     	 {
     	 	// Reload Shooter
     	 	 if ((!lastBXstate) && (stick.GetRawButton(Button_X))) {
     	 	 	Reload();
     	 	 }
     	 	// Toggle Shooter Wheels
     	 	 if((!lastBYstate) && (stick.GetRawButton(Button_Y))) {
                ShooterToggle();
     	 	 }
     	 	// Shoot a frisbee
     	 	 if((!lastRTstate) && (stick.GetRawButton(Button_RIGHT_TRIGGER))) {
     	 	 	if(!PistonState) {
     	 	 		PistonToggle();
     	 	 		PistonUpdate();
     	 	 	} else if(PistonState) {
     	 	 		clearline3();
     	 	 		userDisplay->Printf(DriverStationLCD::kUser_Line3, 1, "Piston Error (F)");
     	 	 		userDisplay->UpdateLCD();
     	 	 	}
     	 	 }

     	 	// Update Variables
     	 	 moveL = (((stick.GetRawAxis(Stick_LEFT_Y)) / HANDICAP) * SQUARE); 
     	 	 spinL = (((stick.GetRawAxis(Stick_LEFT_X)) / HANDICAP) * SQUARE);

     	 	// Update LCD Display with variables
     	 	 userDisplay->Clear();
     	 	 userDisplay->Printf(DriverStationLCD::kUser_Line1, 1, "MoveL: %d", (int) (moveL*100));
     	 	 userDisplay->Printf(DriverStationLCD::kUser_Line2, 1, "SpinL: %d", (int) (spinL*100));
     	 	 // Update Shooter State on LCD
     	 	 if(ShooterState) {
     	 	    userDisplay->Printf(DriverStationLCD::kUser_Line6, 1, "Shooter is onn");
     	 	 } else {
     	 	 	userDisplay->Printf(DriverStationLCD::kUser_Line6, 1, "Shooter is off");
     	 	 }
             userDisplay->UpdateLCD();

            // Drive Robot
             River_Drive.ArcadeDrive(moveL, spinL, SQUARE);
             Wait(0.005); // wait for motor update time

            // Final Checks
             lastRTstate = stick.GetRawButton(Button_RIGHT_TRIGGER);
             lastBYstate = stick.GetRawButton(Button_Y);
             lastBXstate = stick.GetRawButton(Button_X);
     	 }
     } // end of operator control code
    //~~~~~~~~~~~~~~~~~~~~~~~~ TEST MODE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
     void Test(void) {
     	feed.Set(0);
     	shoot_one.Set(0);
     	shoot_two.Set(0);
      // Test Loop
     	 while (IsTest()) {
     		userDisplay->Clear();
     		userDisplay->Printf(DriverStationLCD::kUser_Line1, 1, "TEST MODE");
     		
     		/* Button Diagnostics
     		 switch (true)
     		 {
     		 	case stick.GetRawButton(Button_X):
     		 	    userDisplay->Printf(DriverStationLCD::kUser_Line3, 1, "Button_X");
     		 	    break;
     		 	case stick.GetRawButton(Button_Y):
     		 	    userDisplay->Printf(DriverStationLCD::kUser_Line3, 1, "Button_Y");
     		 	    break;
     		 	case stick.GetRawButton(Button_A):
     		 	    userDisplay->Printf(DriverStationLCD::kUser_Line3, 1, "Button_A");
     		 	    break;
     		 	case stick.GetRawButton(Button_B):
     		 	    userDisplay->Printf(DriverStationLCD::kUser_Line3, 1, "Button_B");
     		 	    break;
     		 	case stick.GetRawButton(Button_START):
     		 	    userDisplay->Printf(DriverStationLCD::kUser_Line3, 1, "Button_START");
     		 	    break;
     		 	case stick.GetRawButton(Button_BACK):
     		 	    userDisplay->Printf(DriverStationLCD::kUser_Line3, 1, "Button_BACK");
     		 	    break;
     		 	case stick.GetRawButton(Button_RIGHT_BUMPER):
     		 	    userDisplay->Printf(DriverStationLCD::kUser_Line3, 1, "Button_RIGHT_BUMPER");
     		 	    break;
     		 	case stick.GetRawButton(Button_RIGHT_TRIGGER):
     		 	    userDisplay->Printf(DriverStationLCD::kUser_Line3, 1, "Button_RIGHT_TRIGGER");
     		 	    break;
     		 	case stick.GetRawButton(Button_LEFT_BUMPER):
     		 	    userDisplay->Printf(DriverStationLCD::kUser_Line3, 1, "Button_LEFT_BUMPER");
     		 	    break;
     		 	case stick.GetRawButton(Button_LEFT_TRIGGER):
     		 	    userDisplay->Printf(DriverStationLCD::kUser_Line3, 1, "Button_LEFT_TRIGGER");
     		 	    break;
     		 	default:
     		 	    userDisplay->Printf(DriverStationLCD::kUser_Line3, 1, "No Button Pushed");
     		 }
             */
     		 
            userDisplay->UpdateLCD();
     		Wait(0.005); // wait for motor update time
     	 }
     } // end of test mode code
 };

 START_ROBOT_CLASS(River);














