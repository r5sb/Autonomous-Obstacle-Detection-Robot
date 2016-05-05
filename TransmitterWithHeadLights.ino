/********Wirelessly Controlled Rover using Arduino and RF Modules
*************Coded and tested successfully by Sidharth Makhija***********
******************** Version 1 28/2/2014***************************

Working : This is the Transmitter code of the Rover.

The four directions for the rover to move are given via the joystick,
there is also a brake button..but that isn't necessarily needed
since the rover does not move until the joystick is moved.

The additional button is used to switch the headlights on and off.

This code uses the VirtualWire and AFMotor(motorshield library).

The motor shield used is self soldered by using Adafruit's motorshield
schemaitic which they have provided on their site since they're awesome!

More pictures,videos and info on the rover here  :
http://letsmakerobots.com/node/40002
http://www.instructables.com/id/The-WirelessObstacle-DetectingBeeping-Robot-using-/

For any doubts,corrections,etc. shoot a mail here :
https://droidhangar.wordpress.com/contact/
*******************************************************************************
*********************************************************************
*/

#include <VirtualWire.h>

//Initialize the variables
int UD = 0;
int LR = 0;
int brake = 16;
int vert = 0;
int hor = 0;

int hLight = 4;
//Variable to keep track of the number of button presses.
int counter = 1;

void setup() 
{
  Serial.begin(9600);//Only for Debugging
  
  pinMode(hLight,INPUT);
  pinMode(brake,INPUT);
  
  digitalWrite(12,HIGH);
  digitalWrite(brake,HIGH);
  digitalWrite(hLight,HIGH);
  
  //Start Transmitting	
  startTx();  
       
}

void loop()
{
  //Reads the joytick movement
  JoystickReader();
  
  //Character that will be sent
  char *msg2;
  
  //Conditions when joystick is stationary
  if(vert == 5 && hor == 4 )
  {
    char *msg2 = "5";
    digitalWrite(13, true); // Flash a light to show transmitting
    vw_send((uint8_t *)msg2, strlen(msg2));
    vw_wait_tx(); // Wait until the whole message is gone
    digitalWrite(13, false);
    //Debugging
    Serial.println("brake");
    Serial.println("  ");
  }
  
  //When joystick is moved forward.
  if(hor==4 && vert<5)
  {
    char *msg2 = "1";
    digitalWrite(13, true); // Flash a light to show transmitting
    vw_send((uint8_t *)msg2, strlen(msg2));
    vw_wait_tx(); // Wait until the whole message is gone
    digitalWrite(13, false);
   //Debugging 
    Serial.println("Forward");
    Serial.println("  ");
  }
  
  //When joystick is moved backward.
  if(hor==4 && vert>5)
  {
    char *msg2 = "2";
    digitalWrite(13, true); // Flash a light to show transmitting
    vw_send((uint8_t *)msg2, strlen(msg2));
    vw_wait_tx(); // Wait until the whole message is gone
    digitalWrite(13, false);
    //Debugging
    Serial.println(" Backward ");
    Serial.println("  ");
  }
  
  //When joytick is moved right.
  if(vert==5 && hor<4)
  {
    char *msg2 = "3";
    digitalWrite(13, true); // Flash a light to show transmitting
    vw_send((uint8_t *)msg2, strlen(msg2));
    vw_wait_tx(); // Wait until the whole message is gone
    digitalWrite(13, false);
    
    Serial.println("Right ");
    Serial.println("  ");
  }
  
  //When joystick is moved left.
  if(vert==5 && hor>4)
  {
   char *msg2 = "4";
   digitalWrite(13, true); // Flash a light to show transmitting
   vw_send((uint8_t *)msg2, strlen(msg2));
   vw_wait_tx(); // Wait until the whole message is gone
   digitalWrite(13, false);
    
    Serial.println("Left ");
    Serial.println("  ");
  }
  
  //When joystick button is pressed.
  if(digitalRead(brake)==LOW) 
   {
    char *msg2 = "5";
    digitalWrite(13, true); // Flash a light to show transmitting
    vw_send((uint8_t *)msg2, strlen(msg2));
    vw_wait_tx(); // Wait until the whole message is gone
    digitalWrite(13, false);
    
    Serial.println("Brake");
    Serial.println("  ");
   }
   
   //Function that checks if the headlight button is pressed.
   headLights();
   
}

/////////////////////////Functions!Functions!Functions!//////////

void JoystickReader()
{
  /*This function reads the analog values given by the joystick
  and translates it as movement.*/
  
  /*Note : The horizontal and vertical of your joystick depends on how you have
  soldered it on to the pcb...adjust as needed*/
  
  //Up Down
  UD = analogRead(A1);
  //Left Right
  LR = analogRead(A0);
  
  /*This maps out the reading of the joystick from 0-1023 to 0-10
  This makes it easier to work with while converting the readings 
  into a direction in the if else statements. */
  
  hor = map(UD,0,1023,0,10);
  vert = map(LR,0,1023,0,10);
  
 
}

void startTx()
{
  
  Serial.println("setup");
  vw_setup(2000);
  //Tx at digital pin 3	 
  vw_set_tx_pin(3); 
}

void headLights()
{
  /*This function give the headlight on or off command based on the 
  number of button presses.
  
  The counter variable is the number of times the headlight button is pressed.
  
  I plan on improving this part of the code by using interrupts as
  this method isn't 100% accurate right now.
  
  */
  
  if(digitalRead(hLight)==LOW)
  {
    //Increase the value of counter every time the headlight button is pressed.
    counter++;
  }
  
  //If value of counter is even the Lights On command is sent.
  if(counter%2==0)
  {
    char *msg2 = "6";
    digitalWrite(13, true); // Flash a light to show transmitting
    vw_send((uint8_t *)msg2, strlen(msg2));
    vw_wait_tx(); // Wait until the whole message is gone
    digitalWrite(13, false);
    //Debugging
    Serial.println("Lights On");
    Serial.println("  ");
  }
  
  //If counter is odd the Light Off command is sent.
  else
  {
    char *msg2 = "7";
      digitalWrite(13, true); // Flash a light to show transmitting
    vw_send((uint8_t *)msg2, strlen(msg2));
    vw_wait_tx(); // Wait until the whole message is gone
    digitalWrite(13, false);
    
    Serial.println("Lights OFF");
    Serial.println("  ");
  }
  delay(10);
}

//End
