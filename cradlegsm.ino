/* This code works with MLX90614 (GY906) and OLED screen
 * It measures both ambient and object temperature in Fahrenheit and display it on the screen
 * Please visit www.surtrtech.com for more details
 */

#include <SoftwareSerial.h>
//Create software serial object to communicate with SIM800L
SoftwareSerial mySerial(3, 2); //SIM800L Tx & Rx is connected to Arduino #3 & #2
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <Adafruit_MLX90614.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET    -1 // Reset pin # (or -1 if sharing Arduino reset pin)

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET); //Declaring the display name (display)
Adafruit_MLX90614 mlx = Adafruit_MLX90614();

void setup() {  
  mlx.begin(); 
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); //Start the OLED display
  display.clearDisplay();
  display.display();

  mySerial.begin(9600);
  Serial.println("Initializing..."); 
  delay(1000);
}

void loop() {
  float ambtemp = mlx.readAmbientTempC();
  float objtemp = mlx.readObjectTempC();
  display.clearDisplay();
  
  display.setTextSize(1);                    
  display.setTextColor(WHITE);             
  display.setCursor(0,4);                
  display.println("Ambient"); 
  
  display.setTextSize(2);
  display.setCursor(50,0);
  display.println(ambtemp,1);
 
  display.setCursor(110,0);
  display.println("C");
  
  display.setTextSize(1);                    
  display.setTextColor(WHITE);             
  display.setCursor(0,20);                
  display.println("Target"); 
  
  display.setTextSize(2);
  display.setCursor(50,17);
  display.println(objtemp,1);
  
  display.setCursor(110,17);
  display.println("C");
  
  display.display();
  if(ambtemp>40|| objtemp>40){
    sendCall();
  }
  delay(1000);

}


void updateSerial()
{
  delay(500);
  while (Serial.available()) 
  {
    mySerial.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while(mySerial.available()) 
  {
    Serial.write(mySerial.read());
  }
}

void SendSMS(String msg)
{
  mySerial.println("AT"); //Once the handshake test is successful, it will back to OK
  updateSerial();

  mySerial.println("AT+CMGF=1"); // Configuring TEXT mode
  updateSerial();
  mySerial.println("AT+CMGS=\"+9647834202829\"");//change ZZ with country code and xxxxxxxxxxx with phone number to sms
  updateSerial();
  mySerial.print(msg); //text content
  updateSerial();
  mySerial.write(26);

}
void sendCall()
{
  mySerial.println("AT"); //Once the handshake test is successful, i t will back to OK
  updateSerial();
  
  mySerial.println("ATD+9647834202829;"); //  change ZZ with country code and xxxxxxxxxxx with phone number to dial
  updateSerial();
  delay(15000); // wait for 20 seconds...
  mySerial.println("ATH"); //hang up
  updateSerial();
}
