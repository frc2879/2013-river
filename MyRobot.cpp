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
* Version Unknown - Xbox controller?
* 
*/

/*  Interesting quotes from development:
 *  --------------------------------------------------
 *   Gregory: "Make the computer thingy"
 * 
 */

/*
*   HARDWARE CONFIGURATION

*   WARNING: The Talons do have their own class, seperate from "jags." However, this class is not implimented yet. In order to use Talons with the Jaguar class, you may need to calibrate the Talons to use the Jaguar PWM conventions.
*   
*   Device:                      Connection:
*   --------------------------------------------------
*   (jag(talon)) Back Right             (Sidecar) PWM port #1 
*   (jag(talon)) Back Left              (Sidecar) PWM port #2
*   (spike) Compressor           (Sidecar) Relay port #1
*   Digital Pressure Sensor      (Sidecar) Digital Input Port #1
*   USB Xbox controller          (Laptop)  USB port #1 (Usin it for real now. This code will not work with a standard joystick)
*/

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

#include "WPILib.h"

class River : public SimpleRobot
{
     // Misc Variables
     bool squaredInputs;  // variable used to set "squared inputs" Not actually used at this point.
     bool AltDrive;  //Boolean used to toggle drive mode / stick configuration.
     bool ToggleOne;  // for use toggling things
     bool ToggleTwo;  // for use toggling things
     float throttle;   // used to represent the alternate "throttle" value.
     float move;  // used to represent the raw Y AXIS. AXIS #2 of the left stick on the gamepad.
     float spin; // used to represent the raw X AXIS. AXIS #1 of the left stick on the gamepad.
     
     // Relays
     Compressor Comp; // spike relay on port one for air compressor.
     
     // Motor Controller
     Jaguar backRight;  // jag(talon) on port #1
     Jaguar backLeft;   // jag(talon) on port #2
     
     // HIDs
     Joystick derpDerp  // Logitech Gamepad NOT a normal joystick
     
     // Important Stuff
     RobotDrive River_Drive;
     DriverStationLCD* userDisplay;
     
public:
    River(void):
         //as they are declared above! 
         Comp(1,1),  // RELAY port #1 and Digital Input Port #1
         backRight(1),
         backLeft(2),
         derpDerp(1),   // logitech gamepad...
         River_Drive(&backLeft, &backRight) // River_Drive uses jags as declared above...
    {
         GetWatchdog().SetExpiration(0.1);   //sets the saftey expiration for watchdog
         River_Drive.SetExpiration(0.1);     //sets safey expiration for River_Drive
         Comp.Start();  //Starts the compressor when the robot is initialized
    
    
    
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
    void allJags (float speed) { // sets all the jags 
        backRight.Set (speed);
        backLeft.Set (speed);
    }
    void rightJags (float speed) { // sets just the right jags 
        backRight.Set (speed);
    }
    void leftJags (float speed) { // sets just the left jags
        backLeft.Set (speed);
    }
    void rotateRight (float speed) { // roates the robot Right
        rightJags (-speed);
        leftJags (speed);
    }
    void roateLeft (float speed) { // rotates the robot Left
        rightJags (speed);
        leftJags (-speed);
    }
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    
    void Autonomous(void)
    {
       GetWatchdog().SetEnabled(false);    // disable the watchdog so it doesn't screw everything up.
       rotateRight(0.5);
       Wait(3.0);   // lololololloololololololol
       allJags(0.0);
    }
    
    void OperatorControl(void)
    {
         GetWatchdog().SetEnabled(true);
         while (IsOperatorControl()) {
             
             GetWatchdog().Feed();
             
             //~~~~~~~~~~~~~ Updated Variabled and Diagnostics ~~~~~~~~~~~~~~~~~
             move = (derpDerp.GetRawAxis(Stick_LEFT_Y));
             spin = (derpDerp.GetRawAxis(Stick_LEFT_X));
             
             if (derpDerp.GetRawButton(Button_RIGHT_TRIGGER)){
                 SquaredInputs=true;
             }
             else {
                 SquaredInputs=false;
             }
             
             if (derpDerp.GetRawButton(Button_LEFT_TRIGGER)){
                 if (throttle < 1)
                 throttle=(throttle + .10);
                 else
                 throttle=(1);
             }
             if (derpDerp.GetRawButton(Button_LEFT_BUMPER)){
                 if (throttle > 0)
                 throttle = (throttle - .10);
                 else
                 throttle = (0);
             }
             
             if ((derpDerp.GetRawButton(Button_START)) & !ToggleOne){     // Start button toggles drive modes. (1 stick, or 2 sticks)
                 ToggleOne = true
                 if (!AltDrive){
                  AltDrive = true
                  clearlineThree();
                  userDisplay->Printf(DriverStationLCD::kUser_Line3, 1, "ALTERNATE DRIVE SCHEME")
                  userDisplay->UpdateLCD();
                 }
                 else if (AltDrive) {
                  AltDrive = false
                  clearlineThree();
                  userDisplay->Printf(DriverStationLCD::kUser_Line3, 1, "NORMAL DRIVE SCHEME")
                  userDisplay->UpdateLCD();
                 }
             }
             
             
             clearlineOne();
             clearlineTwo();
             userDisplay->Printf(DriverStationLCD::kUser_Line2, 1, "Move: %d", (int) (move*100));
             userDisplay->Printf(DriverStationLCD::kUser_Line2, 15, "Spin: %d", (int) (spin*100));
             userDisplay->Printf(DriverStationLCD::kUser_Line1, 1, "LOLz: %d", (int) (throttle*100));
             userDisplay->UpdateLCD();
             //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
             
             if (AltDrive) {
             River_Drive.ArcadeDrive(derpDerp.GetRawAxis(Stick_LEFT_X), derpDerp.GetRawAxis(Stick_RIGHT_Y), SquaredInputs)   // Alternate drive scheme, uses both joystiks.
             }
             else {
             River_Drive.ArcadeDrive(spin, move, SquaredInputs);
             }
                  
             
            wait(0.005);  // refresh rate. The Wifi doesn't even work that fast, so why would we update that fast?
         }
        
    }
    
};

START_ROBOT_CLASS(River);
