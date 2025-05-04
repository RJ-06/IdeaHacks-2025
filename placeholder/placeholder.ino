
#include <Servo.h>
#include <vector>
//initialize servo (servo number corresponds with string number)
Servo servo1; // Low E
Servo servo2; // A
Servo servo3; // D
Servo servo4; // G
Servo servo5; // B
Servo servo6; // High E

-----------------------------------------------------------------------------------------------

//System Variables
int reset = 0; //set servo position to 0
int press; // degrees rotating to press into string
int wait = 10

//angle rotations for each fret
int base_fret = 45;
-----------------------------------------------------------------------------------------------
//Essential Functions

//update: function parameter will be a vector
void read_chords(){ //vector<int>
  array[1] = "chord type" //temporary 
  array[2] = "string number" //temporary
}

-----------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------

//definitions for single-run
void setup(){
  //assign each servo to a port - tentative bc may need to change ports
  servo1.attach(1); 
  servo2.attach(2);
  servo3.attach(3);
  servo4.attach(4);
  servo5.attach(5);
  servo6.attach(6);
} 


//function take in string, s you know which servo to access: 
-----------------------------------------------------------------------------------------------
void loop() {
  
  N(int string, int fret);
  switch(int string)
  { //identifies fret for each string and moves finger down w/ servo to press string
    case 1: //string 1 
      servo1.write(base_fret * fret); 
      delay(wait);
    case 2: 
      servo2.write(basefret * fret);
      delay(wait);
    case 3: 
      servo3.write(base_fret * fret);
      delay(wait);
    case 4:
      servo4.write(base_fret * fret);
      delay(wait);
  default:
    servo1.write(reset);
    servo2.write(reset);
    servo3.write(reset);
    servo4.write(reset);
    servo5.write(reset);
    servo6.write(reset);
  }
  }
  
  
  // put your main code here, to run repeatedly:

}