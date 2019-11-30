/*
* Just Few Comment Lines for the Author
*/

#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1); //Display Intialisation, display instance.
#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
Servo gateservo;
int auth = 0;
int pos = 0;
void setup()
{
  Serial.begin(9600);   // Initiate a serial communication
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(A0, INPUT);
  gateservo.attach(5);
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  Serial.println("Approximate your card to the reader...");
  Serial.println();
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  delay(100);
  display.clearDisplay();

}
void loop()
{
  int cardReader;
  if (digitalRead(A0) == 0)
  {
    Serial.println();
    int i;
    for (i = 0; i < 21; i++)
    {
      Serial.println(i);
      delay(1000);
      if ( ! mfrc522.PICC_IsNewCardPresent() || ! mfrc522.PICC_ReadCardSerial()) //checks if Card is Present
      {
        return; // Returns False if the card is not present --- this is causing the issue
      }
      cardReader = cardRead();
      if (cardReader == 1)
      {
        i = i + 22;
      }
    }
  }
}

int cardRead()
{
  Serial.print("UID tag :");
  String content = "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  if (content.substring(1) == "93 05 DB 1D") //change here the UID of the card/cards that you want to give access
  {
    Serial.println("Authorized access");
    Serial.println();
    digitalWrite(3, HIGH);
    gateservo.write(180);
    auth = 1;
    delay(3000);
    digitalWrite(3, LOW);
    gateservo.write(0);
    return 1;
  }

  else   {
    Serial.println(" Access denied");
    digitalWrite(4, HIGH);
    auth = 0;
    delay(3000);
    digitalWrite(4, LOW);
    return 0;
  }
}
