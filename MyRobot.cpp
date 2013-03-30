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
     int SolenoidUpdate;
     int ShooterSpeed;
     int AutoloadWait;
     int FeedTime;
     int BackFeedTime;
     bool SquaredInputs;
     bool BackFeed;
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
     bool lastBAstate;

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
	     Sally(7),
	  // IO Devices
	     stick(1)
	//~~~~~~~~~~~~~~~~~~~~~~~~ INITIALIZATION ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
     {
  	
   	 River_Drive.SetExpiration(0.1);
     Comp.Start(); // Starts the compressor.

     lastRTstate = false;
     lastBYstate = false;
     lastBXstate = false;
     lastLTstate = false;
     lastBAstate = false;

     ShooterState = false; // Should have the shooter OFF upon robot enabled. if not, you done goofed.
     PistonState = false; // Should leave the chamber area OPEN. If not, switch wires on solenoid
     PistonUpdated = false;
     Billy.Set(false);
     Sally.Set(false);

     // CONFIG VALUES ~~~~~~~
     // ~~~~~~~~~~~~~~~~~~~~~
        HANDICAP = 1;          // configures the max throttle limitation. 1 gives (1/1) max, 2 gives (1/2) max, etc.
        SquaredInputs = true;  // turns squared inputs on/off. Alters control sensativity at low speeds.
        SolenoidUpdate = 1;    // configures the solenoid update time, the amount of time the solenoids are fired for when called.
        ShooterSpeed = 0.56;   // sets shooter wheel speed. 0.0 = 0% , 1.0 = 100%
        AutoloadWait = 0.25;   // sets the amount of time (seconds) that the robot will wait before autoloading a new disc after shooting.
        FeedTime = 0.48;       // sets the amount of time (s) that the feed wheel runs for when reloading. Default is 0.48
        BackFeed = true;       // turns back feeding on/off in the reload function. Back feed should fix discs that are lodged far into the feed wheels, and avoid jams.
        BackFeedTime = 0.5;    // time (s) that wheels will spin in backfeed.
     // ~~~~~~~~~~~~~~~~~~~~~ 
     // CONFIG VALUES ~~~~~~~

     Wait(0.5); // Wait for camera to boot up
     AxisCamera &Camera = AxisCamera::GetInstance("10.28.79.11");

     userDisplay = DriverStationLCD::GetInstance();
     userDisplay->Clear();
     userDisplay->Printf(DriverStationLCD::kUser_Line1, 1, "Robot Initialized. . .");
     userDisplay->Printf(DriverStationLCD::kUser_Line2, 1, "HANDICAP: 1/%d", (int) HANDICAP);
     if(SquaredInputs) {
        userDisplay->Printf(DriverStationLCD::kUser_Line3, 1, "Squared Inputs: ONN");
    } else {
        userDisplay->Printf(DriverStationLCD::kUser_Line3, 1, "Squared Inputs: OFF");
    }
     
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
 	  // Feed new frisbee
        if(PistonState) {
            PistonToggle();
        }
        if(BackFeed) {
            feed.Set(-0.30);
            Wait(BackFeedTime);
            feed.Set(0.0);
        }
 	     feed.Set(0.30);
 	     Wait(FeedTime);
 	     feed.Set(0.0);
     }

     void ShooterUpdate(void) {
      // Update Shooter State
 	     if(ShooterState) {
            shoot_one.Set(-.56);
            shoot_two.Set(-.56);
            //shoot_one.Set(-.15);
            //shoot_two.Set(-.15);
 	     } else {
            shoot_one.Set(0.0);
 	        shoot_two.Set(0.0);
 	     } 
     }
     
    void PistonToggle(void) {
      // Toggles the piston position
     	 if(!PistonState) {
            Billy.Set(true);   // billy is on port #1
            Wait(SolenoidUpdate);
            Billy.Set(false);
            PistonState = true;
         } else if(PistonState) {
            Sally.Set(true);    // sally is on port #3
            Wait(SolenoidUpdate);
            Sally.Set(false);
            PistonState = false;
         }
     	 
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
         userDisplay->Clear();
      // Operator Control Loop
     	 while (IsOperatorControl())
     	 {
     	 	// Reload Shooter manually
     	 	 if (lastBXstate==false && (stick.GetRawButton(Button_X))) {
     	 	 	Reload();
     	 	 }

     	 	// Toggle Shooter Wheels
     	 	 if(lastBYstate==false && (stick.GetRawButton(Button_Y))) {
                // SHOOTER TOGGLE
                printf("Button Y Toggle!!!!!\n"); 
                if(ShooterState) {
                    printf("ShooterState becoming false\n");
                    ShooterState = false;
                } else {
                    printf("ShooterState becoming true\n");
                    ShooterState = true;
                }
     	 	 }
          
             if(lastBAstate==false && (stick.GetRawButton(Button_A))) {
              //Shoot a frisbee
                if(ShooterState) {
                 clearline3();
                 PistonUpdate();
                 Wait(AutoloadWait);
                 Reload();
                } else {
                 clearline3();
                 userDisplay->Printf(DriverStationLCD::kUser_Line3, 1, "SLAP MATT FOR ME");
                 userDisplay->UpdateLCD();
                }
             }

     	 	// Update Variables
     	 	 moveL = ((stick.GetRawAxis(Stick_LEFT_Y)) / HANDICAP); 
     	 	 spinL = -((stick.GetRawAxis(Stick_LEFT_X)) / HANDICAP);

     	 	// Update LCD Display with variables
     	 	 clearline1();
             clearline2();
             clearline4();
             clearline5();
             clearline6();
     	 	 userDisplay->Printf(DriverStationLCD::kUser_Line1, 1, "MoveL: %d", (int) (moveL*100));
     	 	 userDisplay->Printf(DriverStationLCD::kUser_Line2, 1, "SpinL: %d", (int) (spinL*100));
     	 	 // Update Shooter State on LCD
     	 	 if(ShooterState == true) {
     	 	    userDisplay->Printf(DriverStationLCD::kUser_Line6, 1, "Shooter is onn");
     	 	 } else {
     	 	 	userDisplay->Printf(DriverStationLCD::kUser_Line6, 1, "Shooter is off");
     	 	 }

             userDisplay->UpdateLCD();

            // Drive Robot
             
             River_Drive.ArcadeDrive(moveL, spinL, SquaredInputs);

            // Final Checks and Updates
             ShooterUpdate();
             lastRTstate = stick.GetRawButton(Button_RIGHT_TRIGGER);
             lastBYstate = stick.GetRawButton(Button_Y);
             lastBXstate = stick.GetRawButton(Button_X);
             lastBAstate = stick.GetRawButton(Button_A);
             lastLTstate = stick.GetRawButton(Button_LEFT_TRIGGER);

             Wait(0.005); // wait for motor update time
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

              if(stick.GetRawButton(Button_Y)) {
                printf("Button Y being held\n");
             }
             if(stick.GetRawButton(Button_X)) {
                printf("Button X being held\n");
             }
             if(stick.GetRawButton(Button_A)) {
                printf("Button A being held\n");
             }
             if(stick.GetRawButton(Button_B)) {
                printf("Button B being held\n");
             }
     		 
            userDisplay->UpdateLCD();
     		Wait(0.005); // wait for motor update time
     	 }
     } // end of test mode code
 };

 START_ROBOT_CLASS(River);














