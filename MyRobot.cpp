/*
*   FRC Team 2879 Robot Code
*
*   See the README file for more information.
*/

#include "WPILib.h"

//Controller Buttons
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
    Jaguar feed; // controls the motor that feeds the shooter
    Jaguar shoot_one; // shooter motor #1
    Jaguar shoot_two; // shooter motor #2
    Compressor Comp; // Compressor
    Solenoid Billy; // The solenoid that shoots stuff
    Solenoid Sally; // Billy's cousin.
    Joystick stick; // Logitech Gamepad
    DriverStationLCD* userDisplay;

    float moveL;
    float spinL;
    float moveR;
    float spinR;
    bool SquaredInputs;
    bool shooter;
    bool pistonState;
    bool pistonUpdated;
    bool lastbXstate;
    bool lastbYstate;
    bool lastRTstate;
    bool lastLTstate;

public:
    River(void):
        //these must be initialized in the same order as they are declared above.
        River_Drive(1, 2),
        feed(3),
        shoot_one(4),
        shoot_two(5),
        Comp(2, 1), // digital input for sensor and Relay port for spike
        Billy(1),
        Sally(2),
        stick(1)
    {
        // this code will run when the robot is powered up, but disabled.
        River_Drive.SetExpiration(0.1);

        shooter = false;
        SquaredInputs = false;
        pistonUpdated = false;  //open
        feed.Set(0.0);
        Billy.Set(false);  // neither side of the solenoid is activated. NEVER ACTIVATE BOTH AT ONCE
        Sally.Set(false);
        pistonState = false;  // (piston is open)

        userDisplay  = DriverStationLCD::GetInstance();
        userDisplay->Clear();
        userDisplay->Printf(DriverStationLCD::kUser_Line1, 1, "Robot Initialize");
        userDisplay->UpdateLCD();

        Comp.Start(); // Starts the compressor when the robot is initialized
        
        Wait(0.5);  // Wait for camera to boot up
        AxisCamera &Camera = AxisCamera::GetInstance("10.28.79.11");
    }

    void clearline1(void) { // Clears line #1
        userDisplay->Printf(DriverStationLCD::kUser_Line1, 1, "                   ");
        userDisplay->UpdateLCD();
    }
    void clearline2(void) { // Clears line #2
        userDisplay->Printf(DriverStationLCD::kUser_Line2, 1, "                   ");
        userDisplay->UpdateLCD();
    }
    void clearline3(void) { // Clears line #3
        userDisplay->Printf(DriverStationLCD::kUser_Line3, 1, "                   ");
        userDisplay->UpdateLCD();
    }
    void clearline4(void) {  // Clears line #4
        userDisplay->Printf(DriverStationLCD::kUser_Line4, 1, "                   ");
        userDisplay->UpdateLCD();
    }
    void clearline5(void) { // Clears line #5
        userDisplay->Printf(DriverStationLCD::kUser_Line5, 1, "                   ");
        userDisplay->UpdateLCD();
    }
    void clearline6(void) { // Clears line #6
        userDisplay->Printf(DriverStationLCD::kUser_Line6, 1, "                   ");
        userDisplay->UpdateLCD();
    }

    void reload(void) {
        if(pistonState) {
        PistonToggle();
        PistonUpdate();
        } else if(!pistonState) {
          clearline3();
          userDisplay->Printf(DriverStationLCD::kUser_Line3, 1, "Piston Error (r)");
          userDisplay->UpdateLCD();
        }
        Wait(0.05);
        feed.Set(0.26);
        Wait(0.15);
        feed.Set(0.00);
    }

    void PistonToggle(void) {
        // Toggles the value of pistonState, and therefore the position of the piston.
        if(!pistonState) {
           pistonState = true;
           pistonUpdated = false;
        } else {
            pistonState = false;
            pistonUpdated = false;
        }
    }

    void PistonUpdate(void) {
        if(pistonState && !pistonUpdated) {
           Billy.Set(true);
           Wait(0.15);
           Billy.Set(false);
        } else if(!pistonState && !pistonUpdated) {
           Sally.Set(true);
           Wait(0.15);
           Sally.Set(false);
        }
        pistonUpdated = true;
    }
        

    void shooterupdate(void) {
        if(shooter) {
            shoot_one.Set(.56);
            shoot_two.Set(.56);
        } else {
            shoot_one.Set(0);
            shoot_two.Set(0);
        }
    }

    //Runs in autonomus mode
    void Autonomous(void)
    {
        //no auton mode as of yet
    }

    //runs in operator control mode
    void OperatorControl(void) {
        River_Drive.SetSafetyEnabled(true);
        shooter=false;
        shooterupdate();
       
        while (IsOperatorControl())
        {
            if (lastbXstate == false && stick.GetRawButton(Button_X) == true) {
                reload(); //reload frisbee
            }

            if (lastbYstate == false && stick.GetRawButton(Button_Y) == true) {
                //toggle shooter
                if (shooter) {
                    shooter=false;
                } else {
                    shooter=true;
                }
                shooterupdate();
            }
            /*
            if (lastRTstate == false && stick.GetRawButton(Button_RIGHT_TRIGGER) == true) {
                // Shoots a frisbee
               if(!pistonState) {
                PistonToggle();
                PistonUpdate();
               } else if(pistonState) {
                clearline3();
                userDisplay->Printf(DriverStationLCD::kUser_Line3, 1, "Piston Error (F)");
                userDisplay->UpdateLCD();
               }

            }
            */

            if(stick.GetRawButton(Button_RIGHT_TRIGGER)) {
                Billy.Set(true);
            } else {
                Sally.Set(true);
            }   
    

            

            // Sets squared inputs for driving
            if (stick.GetRawButton(Button_LEFT_BUMPER)) {
                SquaredInputs=true;
            }
            else {
                SquaredInputs=false;
            }


            // Updates variables
            if (SquaredInputs) {
                moveL = (stick.GetRawAxis(Stick_LEFT_Y));  //axis 2
                spinL = (stick.GetRawAxis(Stick_LEFT_X));  //axis 1
            }
            else {
                moveL = ((stick.GetRawAxis(Stick_LEFT_Y)) / 2);
                spinL = -((stick.GetRawAxis(Stick_LEFT_X)) / 2);
            }

            // Updates LCD Display with variables
            userDisplay->Clear();
            userDisplay->Printf(DriverStationLCD::kUser_Line1, 1, "MoveL: %d", (int) (moveL*100));
            userDisplay->Printf(DriverStationLCD::kUser_Line2, 1, "SpinL: %d", (int) (spinL*100));
            if (shooter) {
                userDisplay->Printf(DriverStationLCD::kUser_Line5, 1, "shooter is on");
            } else {
                userDisplay->Printf(DriverStationLCD::kUser_Line5, 1, "shooter is off");
            }
            if (SquaredInputs) {
                userDisplay->Printf(DriverStationLCD::kUser_Line6, 1, "squared inputs active");
            }
            else {
                clearline6();
            }
            userDisplay->UpdateLCD();


            // Drives Robot
            River_Drive.ArcadeDrive(moveL, spinL, SquaredInputs);
            Wait(0.005); // wait for a motor update time

            //get button state at end of loop
            lastRTstate = stick.GetRawButton(Button_RIGHT_TRIGGER);
            lastbYstate = stick.GetRawButton(Button_Y);
            lastbXstate = stick.GetRawButton(Button_X);
        }
    } //end of operator control code

    //runs in test mode
    void Test() {
        while (IsTest()) {
            userDisplay->Clear();
            userDisplay->Printf(DriverStationLCD::kUser_Line1, 1, "Test Mode");
            userDisplay->UpdateLCD();
            Wait(0.005); // wait for a motor update time
        }
    }
};

START_ROBOT_CLASS(River);
