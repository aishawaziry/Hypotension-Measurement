

// heart rata

#define USE_ARDUINO_INTERRUPTS true    // Set-up low-level interrupts for most acurate BPM math.
#include <PulseSensorPlayground.h>     // Includes the PulseSensorPlayground Library.   

//  Variables
const int PulseWire = 1;       // PulseSensor PURPLE WIRE connected to ANALOG PIN 1
const int LED13 = 13;          // The on-board Arduino LED, close to PIN 13.
int Threshold = 550;           
                                
                               
PulseSensorPlayground pulseSensor;  // Creates an instance of the PulseSensorPlayground object called "pulseSensor"


//==================================================================================

// thermistor
// Include the libraries we need
#include <OneWire.h>
#include <DallasTemperature.h>
// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 8
// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);
// arrays to hold device address
DeviceAddress insideThermometer;

//====================================================================================

//stepper motor

#define A 2
#define B 3
#define C 4
#define D 5
 
#define NUMBER_OF_STEPS_PER_REV 512



int buzzer=10;                //Defining variable and the GPIO pin on Arduino
int led=7;



void setup(){
pinMode(A,OUTPUT);
pinMode(B,OUTPUT);
pinMode(C,OUTPUT);
pinMode(D,OUTPUT);
pinMode(buzzer, OUTPUT);    //Defining the pin as OUTPUT
pinMode(led, OUTPUT); 



//====================================================

// heart rata

Serial.begin(9600);          // For Serial Monitor

  // Configure the PulseSensor object, by assigning our variables to it. 
  pulseSensor.analogInput(PulseWire);   
  pulseSensor.blinkOnPulse(LED13);       //auto-magically blink Arduino's LED with heartbeat.
  pulseSensor.setThreshold(Threshold);   

  // Double-check the "pulseSensor" object was created and "began" seeing a signal. 
   if (pulseSensor.begin()) {
    Serial.println("We created a pulseSensor Object !");  //This prints one time at Arduino power-up,  or on Arduino reset.  
  }

//===================================================================

//thermistor

  Serial.println("Dallas Temperature IC Control Library Demo");
  // locate devices on the bus
  Serial.print("Locating devices...");
  sensors.begin();
  Serial.print("Found ");
  Serial.print(sensors.getDeviceCount(), DEC);
  Serial.println(" devices.");
  // report parasite power requirements
  Serial.print("Parasite power is: "); 
  if (sensors.isParasitePowerMode()) Serial.println("ON");
  else Serial.println("OFF");
  
  
  if (!sensors.getAddress(insideThermometer, 0)) Serial.println("Unable to find address for Device 0"); 
  
  
  // show the addresses we found on the bus
  Serial.print("Device 0 Address: ");
  printAddress(insideThermometer);
  Serial.println();
  // set the resolution to 9 bit (Each Dallas/Maxim device is capable of several different resolutions)
  sensors.setResolution(insideThermometer, 9);
 
  Serial.print("Device 0 Resolution: ");
  Serial.print(sensors.getResolution(insideThermometer), DEC); 
  Serial.println();


}

void write(int a,int b,int c,int d){
digitalWrite(A,a);
digitalWrite(B,b);
digitalWrite(C,c);
digitalWrite(D,d);
}



void onestep(){
write(1,0,0,0);
delay(1);
write(1,1,0,0);
delay(1);
write(0,1,0,0);
delay(1);
write(0,1,1,0);
delay(1);
write(0,0,1,0);
delay(1);
write(0,0,1,1);
delay(1);
write(0,0,0,1);
delay(1);
write(1,0,0,1);
delay(1);
}



// function to print the temperature for a device
void printTemperature(DeviceAddress deviceAddress)
{
  // method 1 - slower
  //Serial.print("Temp C: ");
  //Serial.print(sensors.getTempC(deviceAddress));
  //Serial.print(" Temp F: ");
  //Serial.print(sensors.getTempF(deviceAddress)); // Makes a second call to getTempC and then converts to Fahrenheit
  // method 2 - faster
  float tempC = sensors.getTempC(deviceAddress);
  if (tempC > 22){
    digitalWrite(buzzer, HIGH);   //Setting the pin HIGH
    digitalWrite(led, HIGH); 
    delay(1000);
    digitalWrite(buzzer, LOW);    //Setting the pin HIGH
    digitalWrite(led, LOW); 
    delay(1000);
  }
  if(tempC == DEVICE_DISCONNECTED_C) 
  {
    Serial.println("Error: Could not read temperature data");
    return;
  }
  Serial.print("Temp C: ");
  Serial.println(tempC);
  delay(1000);
  if(tempC > 21) {
    digitalWrite(buzzer, HIGH);   //Setting the pin HIGH
    digitalWrite(led, HIGH); 
    delay(1000);
    digitalWrite(buzzer, LOW);    //Setting the pin HIGH
    digitalWrite(led, LOW); 
//    delay(1000);
    int i;
    i=0;
    while(i<NUMBER_OF_STEPS_PER_REV){
      onestep();
      
    
    i++;
    }

  }
//  Serial.print(" Temp F: ");
//  Serial.println(DallasTemperature::toFahrenheit(tempC)); // Converts tempC to Fahrenheit
  


}








void loop(){

//digitalWrite(buzzer, HIGH);   //Setting the pin HIGH
//digitalWrite(led, HIGH); 
//delay(1000);
//digitalWrite(buzzer, LOW);    //Setting the pin HIGH
//digitalWrite(led, LOW); 
//delay(1000);


  


int myBPM = pulseSensor.getBeatsPerMinute();  // Calls function on our pulseSensor object that returns BPM as an "int".
                                               // "myBPM" hold this BPM value now. 

if (pulseSensor.sawStartOfBeat()) {            // Constantly test to see if "a beat happened". 
// Serial.println("  A HeartBeat Happened ! "); // If test is "true", print a message "a heartbeat happened".
 Serial.print("BPM: ");                        // Print phrase "BPM: " 
 Serial.println(myBPM);                        // Print the value inside of myBPM. 
}

  delay(20);                    // considered best practice in a simple sketch.

//int i;
//i=0;
//while(i<NUMBER_OF_STEPS_PER_REV){
//  onestep();
//  
//
//i++;
//}




//thermistor code

// request to all devices on the bus
//  Serial.print("Requesting temperatures...");
  sensors.requestTemperatures(); // Send the command to get temperatures
//  Serial.prinln("DONE");
  
  // It responds almost immediately. Let's print out the data
  printTemperature(insideThermometer); // Use a simple function to print out the data






}





void printAddress(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}
