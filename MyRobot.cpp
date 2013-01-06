/*
*     _________          _____________     _________________    ______________
*    /          \       |    _____    |   |                 /  |   _______    |   
*   /    ___     \      |   |     |   |   |                /   |  |       |   |
*  /    /   \     \     |   |     |   |   |________       /    |  |       |   |
* |_____|   /     /     |   |     |   |           /      /     |  |       |   |
*          /     /       \   \___/    /          /      /      |  |_______|   |
*         /     /         \          /          /      /       |__________    |
*        /     /          /   ____   \         /      /                   |   |
*       /     /          /   /    \   \       /      /                    |   |
*      /     /          |   |      |   |     /      /                     |   |
*     /     /_______    |   |      |   |    /      /                      |   | 
*    |              |   |   |______|   |   /      /                       |   |
*    |______________|   |______________|  /______/                        |___|  
*/

/*
* TEAM 2879 robot code
* Version unknown - continuing pre-kickoff development
* 
*/

/*
*   HARDWARE CONFIGURATION
*   
*   Device:                      Connection:
*   --------------------------------------------------
*   (jag) Front Right            (Sidecar) PWM port #1 
*   (jag) Front Left             (Sidecar) PWM port #2
*   (jag) Back Right             (Sidecar) PWM port #3
*   (jag) Back Left              (Sidecar) PWM port #4
*   (spike) Compressor           (Sidecar) Relay port #1
*   Logitech Attack3 joystick    (Laptop)  USB port #1
*/


#include "WPILib.h"

class River : public SimpleRobot
{
     // Misc Variables
     bool squaredInputs;
     float throttle;
     float move;
     float spin;
     
     // Relays
     
     // Motor Controllers
     Jaguar frontRight;
     Jaguar frontLeft;
     Jaguar backRight;
     Jaguar backLeft;
     
     // HIDs
     Joystick driveStick;
     
     // Important Stuff
     RobotDrive River_Drive;
     DriverStationLCD* userDisplay;
     
public:
    River(void):
    
         frontRight(1),
         frontLeft(2),
         backRight(3),
         backLeft(4),
         driveStick(1),
         River_Drive(&frontLeft, &backLeft, &frontRight, &backRight)
    {
         GetWatchdog().SetExpiration(0.1);
         River_Drive.SetExpiration(0.1);
         
         throttle=((.5 * driveStick.GetThrottle()) + .5);
         userDisplay = DriverStationLCD::GetInstance();
         userDisplay->Clear();
    }
    
    //~~~~~~~~~~~~~~ Display Utities ~~~~~~~~~~~~~~~~~~
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
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    
    //~~~~~~~~~~~~ Motor Control ~~~~~~~~~~~~~~~~~~~~~~~
    void allJags (float speed) {
        frontRight.Set (speed);
        frontLeft.Set (speed);
        backRight.Set (speed);
        backLeft.Set (speed);
    }
    void rightJags (float speed) {
        frontRight.Set (speed);
        backRight.Set (speed);
    }
    void leftJags (float speed) {
        frontLeft.Set (speed);
        backLeft.Set (speed);
    }
    void rotateRight (float speed) {
        rightJags (-speed);
        leftJags (speed);
    }
    void roateLeft (float speed) {
        rightJags (speed);
        leftJags (-speed);
    }
    void strafeRight(float speed)
        {
         frontRight.Set(speed);
         frontLeft.Set(-speed);
         backRight.Set(-speed);
         backLeft.Set(speed);
        }
        
        void strafeLeft(float speed)
        {
         frontRight.Set(-speed);
         frontLeft.Set(speed);
         backRight.Set(speed);
         backLeft.Set(-speed);
        }
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    
    void Autonomous(void)
    {
       rotateRight(1.0);
       Wait(10.0);
       allJags(0.0);
    }
    
    void OperatorControl(void)
    {
         GetWatchdog().SetEnabled(true);
         while (IsOperatorControl()) {
             
             GetWatchdog().Feed();
             
             //~~~~~~~~~~~~~ Updated Variabled and Diagnostics ~~~~~~~~~~~~~~~~~
             throttle = ((.5 * driveStick.GetThrottle()) + .5);
             move = (driveStick.GetRawAxis(2));
             spin = (driveStick.GetRawAxis(1));
             
             clearlineOne();
             clearlineTwo();
             userDisplay->Printf(DriverStationLCD::kUser_Line2, 1, "Move: %d", (int) (move*100));
             userDisplay->Printf(DriverStationLCD::kUser_Line2, 15, "Spin: %d", (int) (spin*100));
             userDisplay->Printf(DriverStationLCD::kUser_Line1, 1, "LOLz: %d", (int) (throttle*100));
             userDisplay->UpdateLCD();
             //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
             if (driveStick.GetTrigger()) {
                 River_Drive.MecanumDrive_Cartesian(driveStick.GetX(), driveStick.GetY(), 0.0, 0.0);
             }
             else if (driveStick.GetRawButton(3)) {
                 allJags(throttle);
             }
             else if (driveStick.GetRawButton(2)){
                 allJags(-throttle);
             }
             else if (driveStick.GetRawButton(4)) {
                 strafeLeft(throttle);
             }
             else if (driveStick.GetRawButton(5)) {
                 strafeRight(throttle);
             }
             else {
             River_Drive.ArcadeDrive(spin, move, false);
             }
            
         }
        
    }
    
};

START_ROBOT_CLASS(River);