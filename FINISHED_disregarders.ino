////
//
// General code from http://www.pushingbox.com for Arduino + Ethernet Shield (official) v1.2
//
////

#include <SPI.h>
#include <Ethernet.h>
#include <LiquidCrystal.h>

  /////////////////
 // MODIFY HERE //
/////////////////
byte mac[] = { 0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x19 };   // Be sure this address is unique in your network

//Your secret DevID from PushingBox.com. You can use multiple DevID  on multiple Pin if you want
char DEVID1[] = "v0B83D3FEFE673CF";        //Scenario : "The mailbox is open"

//Numeric Pin where you connect your switch
uint8_t pinDevid1 = 7; // Example : the mailbox switch is connect to the Pin 3

// Debug mode
boolean DEBUG = true;
  //////////////
 //   End    //
//////////////

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(9, 8, 5, 4, 3, 2);
const int ledPin = 6;


char serverName[] = "api.pushingbox.com";
boolean pinDevid1State = false;                // Save the last state of the Pin for DEVID1
boolean lastConnected = false;                 // State of the connection last time through the main loop
long lastPress;

// Initialize the Ethernet client library
// with the IP address and port of the server 
// that you want to connect to (port 80 is default for HTTP):
EthernetClient client;

void setup() {
  Serial.begin(9600);
  pinMode(pinDevid1, INPUT);
  
  // start the Ethernet connection:
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // no point in carrying on, so do nothing forevermore:
    while(true);
  }
  else{
    Serial.println("Ethernet ready");
    // print the Ethernet board/shield's IP address:
    Serial.print("My IP address: ");
    Serial.println(Ethernet.localIP());
  }
  lcd.begin(16, 2);
   lcd.setCursor(0, 0); 
 lcd.print("     Do not     "); 
  lcd.setCursor(0, 1);
  // print on second line 

 lcd.print("     touch!     "); 
  
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  // give the Ethernet shield a second to initialize:
  delay(1000);
}

void loop()
{
      ////
      // Listening for the pinDevid1 state
      ////
      if (digitalRead(pinDevid1) == HIGH && pinDevid1State == false) // switch on pinDevid1 is ON 
      {
        if(DEBUG){Serial.println("pinDevid1 is HIGH");}
        pinDevid1State = true;
        //Sending request to PushingBox when the pin is HIGH
        
         lcd.setCursor(0, 0); 
 lcd.print(" Smile, you are"); 
  lcd.setCursor(0, 1);
  // print on second line 

lcd.print("   on camera!"); 
        digitalWrite(ledPin, HIGH);
        sendToPushingBox(DEVID1);
        lastPress = millis();
      }
     /*  if (digitalRead(pinDevid1) == LOW && pinDevid1State == true) // switch on pinDevid1 is OFF
      {
        if(DEBUG){Serial.println("pinDevid1 is LOW");}
        pinDevid1State = false;
        updateLCD("Button up   ");
        
        //Sending request to PushingBox when the pin is LOW
        //sendToPushingBox(DEVID1);    //Here you can run an other scenario by creating a DEVID2 variable
      }*/
      
      if (pinDevid1State && millis() > lastPress + 5000) {
        digitalWrite(ledPin, LOW);
        pinDevid1State = false;
        lcd.setCursor(0, 0);
  // print on second line 

lcd.print("     Do not    ");

lcd.setCursor(0, 1);
  // print on second line 

lcd.print("     touch!    ");
      }
      
      
      //DEBUG part
      // this write the respons from PushingBox Server.
      // You should see a "200 OK"
      if (client.available()) {
        char c = client.read();
        if(DEBUG){Serial.print(c);}
      }
      
      // if there's no net connection, but there was one last time
      // through the loop, then stop the client:
      if (!client.connected() && lastConnected) {
        if(DEBUG){Serial.println();}
        if(DEBUG){Serial.println("disconnecting.");}
        client.stop();
      }
      lastConnected = client.connected();
}


//Function for sending the request to PushingBox
void sendToPushingBox(char devid[]){
  client.stop();
  if(DEBUG){Serial.println("connecting...");}

  if (client.connect(serverName, 80)) {
    if(DEBUG){Serial.println("connected");}

    if(DEBUG){Serial.println("sending request");}
    client.print("GET /pushingbox?devid=");
    client.print(devid);
    client.println(" HTTP/1.1");
    client.print("Host: ");
    client.println(serverName);
    client.println("User-Agent: Arduino");
    client.println();
  } 
  else {
    if(DEBUG){Serial.println("connection failed");}
  }
}

void updateLCD(char displStr[]){
  lcd.setCursor(0, 0);
  lcd.print(displStr);
}
