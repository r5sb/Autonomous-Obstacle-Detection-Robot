/*******Wirelessly Controlled Rover using Arduino and RF Modules************
*************Coded and tested successfully by Sidharth Makhija******
**************https://droidhangar.wordpress.com/contact/***********
******************** Version 1 28/2/2014***************************
*********************************************************************
*********************************************************************/    
#include <Ultrasonic.h> 
#include <VirtualWire.h>
#include <AFMotor.h>


AF_DCMotor motor1(1, MOTOR12_1KHZ);
AF_DCMotor motor2(2, MOTOR12_1KHZ);
AF_DCMotor motor3(3, MOTOR34_1KHZ); // create motor #2, 64KHz pwm
AF_DCMotor motor4(4, MOTOR34_1KHZ);




int distLeft = 0;
int distRight = 0;
int distFront = 0;
int trigerpin = 14;
int echopin = 15; 
                    
int distLimit = 30;    //distance limit for obstacle = 30cm       
int readDistane();
unsigned long pulsetime = 0;  
unsigned long distance = 0;   



void setup()
{
  Serial.begin(9600);
  Serial.println("setup");

    // Initialise the IO and ISR
  vw_set_ptt_inverted(true); // Required for DR3100
  vw_setup(2000);	 // Bits per sec
  vw_set_rx_pin(2);
  vw_rx_start();       
 
  motor1.setSpeed(200);
  motor2.setSpeed(200); 
  motor3.setSpeed(200);
  motor4.setSpeed(200);
 


  pinMode(trigerpin, OUTPUT);  
  pinMode(echopin, INPUT);     
          
  stopMotor();               
  Serial.println("STOP");    
  delay(2000);                 
  
}


void loop()
{

 uint8_t buf[VW_MAX_MESSAGE_LEN];
    uint8_t buflen = VW_MAX_MESSAGE_LEN;

    if (vw_get_message(buf, &buflen)) // Non-blocking
    {
	int i;

        digitalWrite(13, true); // Flash a light to show received good message
	// Message with a good checksum received, dump it.
	
	
	for (i = 0; i < buflen; i++)
	{
	    
        if(buf[i] == '1')
      {
        forward();
        Serial.println("forward");
      }
    if(buf[i] == '2')
      {
        backward();
        Serial.println("backward");
      }
        if(buf[i] == '4')
      {
        left();
        Serial.println("left");
      }
        if(buf[i] == '3')
      {
        right();
        Serial.println("right");
      }
        if(buf[i] == '5')
       {
         stopMotor();
         Serial.println("stopped");
       }
       if(buf[i] == '6')
       {
         ObstacleAvoiderMode();
         Serial.println("Obstacle Avoidance Selected");
       }
	    Serial.print(" ");
	}
	Serial.println("");
        digitalWrite(13, false); 
        
    }
  } 



///////////////////////////////////////////////////////////////////////////////////////////////////

void ObstacleAvoiderMode()
{
  int distLimit = 30; 
   distLeft = 0;
   distRight = 0;
   distFront = 0;
  
  
  Serial.println("processing.");  
  delay(1000); 

                 
  distFront = readDistance();  
  Serial.print("DistF: ");     
  Serial.print(distFront);
  Serial.println(" cm.");  
  delay(100);

  distLeft = readLeft();
  Serial.print("DistL: ");     
  Serial.print(distLeft);
  Serial.println(" cm.");  
  delay(100);
  
  distRight = readRight();
  Serial.print("DistR: ");     
  Serial.print(distRight);
  Serial.println(" cm.");  
  delay(100);
    
 /* 
  myservo.write(180);             
  delay(1000);                    
  distLeft = readDistance();   
  Serial.print("DistL: ");     
  Serial.print(distLeft);
  Serial.println(" cm.");
  
  myservo.write(90);           
  delay(1000);                 
  distFront = readDistance();  
  Serial.print("DistF: ");     
  Serial.print(distFront);
  Serial.println(" cm.");
  
  
  myservo.write(0);               
  delay(1000);                     
  distRight = readDistance();    
  Serial.print("DistR: ");        
  Serial.print(distRight);
  Serial.println(" cm.");
  
  myservo.write(90);
  delay(1000);*/                       
  if(distFront<distLimit)
  {
  if(distFront < distLeft && distFront < distRight)
  {
    if(distLeft==0 && distRight==1 && distLeft>distFront)
    {
      Serial.println("turning left");     
      left();                        
      delay(100);
    }
    else
   
   {
      if(distLeft == distRight && distLeft == 1 && distRight == 1)                 // Case in which the distance left and right are equal
      { 
        Serial.println("Avoiding");  
        backward();
        delay(500);
        
      }
   
      else if(distRight == 0 && distLeft == 1 && distRight > distFront)
      {
        Serial.println("turning right");    
        right();                     
        delay(100);
      }
    }
  }
  }
  else  {
    forward();
  }

  
  distLeft = 0;
  distRight = 0;
  distFront = 0;
                                              // robot goes forward until he detects an object at a distance less than the distance limit
  int x = 1;                               
  while(x)
  {                                
    Serial.println("in front");          
    forward();                                //moves forward
    distFront = readDistance();           
    Serial.print("DistF: ");               
    Serial.print(distFront);
    Serial.println(" cm.");
    if(distFront < distLimit)                // if the distance is less than the limit distance
    {           
       stopMotor (); 
      delay(200);                          
      x = 0;                               
                                            
                                           

      Serial.println("stationary");
}

  }
  }

void forward()
{
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);
  
}

void backward()
{
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
  motor3.run(BACKWARD);
  motor4.run(BACKWARD);
}

void right()
{
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(BACKWARD);
  motor4.run(BACKWARD);
}

void left()
{
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);
}

void stopMotor()
{
  motor1.run(RELEASE);
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);
}

  

int readDistance()
{
  
  digitalWrite(trigerpin, LOW); 
  delayMicroseconds(2);
  digitalWrite(trigerpin, HIGH);       
  delayMicroseconds(10);               
  digitalWrite(trigerpin,LOW);         
  pulsetime = pulseIn(echopin, HIGH);  
  return pulsetime/58.2;                
                                       
}

int readRight()
{
  int reading = digitalRead(16);
  if(reading==1)
  {
    distRight = 0;
  Serial.println("blocked");
  }
  else
  {
    distRight = distFront + 30;
  Serial.println("unblocked");
  }
   
  delay(100);
}

int readLeft()
{
  int reading = digitalRead(17);
  if(reading==1)
  {
    distLeft = 0;
  Serial.println("blocked");
  
  }
  else
  {
    distLeft = distFront + 30;
  Serial.println("unblocked");
  }
  
  delay(100);
}

//end
