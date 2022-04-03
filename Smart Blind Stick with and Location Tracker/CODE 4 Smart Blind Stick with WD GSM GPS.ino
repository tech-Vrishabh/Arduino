#include <SoftwareSerial.h>

#include <TinyGPS++.h>

const int buttonpin = 12;
int ButtonValue;
float lattitude, longitude;

float a[2];

float *p;

SoftwareSerial gpsSerial(8, 9);

SoftwareSerial gsmSerial(6, 7);

TinyGPSPlus gps;

const int trigPin1 = A1;

const int echoPin1 = A0;

long duration1;

int distance1;

const int trigPin2 = A2;

const int echoPin2 = A3;

long duration2;

int distance2;

void setup()

{

  pinMode(trigPin1, OUTPUT);

  pinMode(echoPin1, INPUT);

  pinMode(5, OUTPUT);

  pinMode(A4, INPUT);

  Serial.begin(9600);

  pinMode(trigPin2, OUTPUT);

  pinMode(echoPin2, INPUT);

  pinMode(buttonpin, INPUT);

  Serial.begin(9600);

  delay(1000);

  gpsSerial.begin(9600);

  delay(1000);

  gsmSerial.begin(9600);

  delay(1000);

  Serial.print("----Tracking----\n");

  Serial.print("***Location***\n");

  gsmSerial.println("AT+CNMI=2,2,0,0,0");

  delay(3000);

  Serial.print("Initializing……\n");

  delay(2000);

  Serial.print("==== System Ready ==== \n ");

  delay(1000);
}

void loop()

{

  digitalWrite(trigPin1, LOW);

  delayMicroseconds(2);

  digitalWrite(trigPin1, HIGH);

  delayMicroseconds(10);

  digitalWrite(trigPin1, LOW);

  duration1 = pulseIn(echoPin1, HIGH);

  distance1 = duration1 * 0.034 / 2;

  Serial.print("Distance1: ");

  Serial.println(distance1);

  digitalWrite(trigPin2, LOW);

  delayMicroseconds(2);

  digitalWrite(trigPin2, HIGH);

  delayMicroseconds(10);

  digitalWrite(trigPin2, LOW);

  duration2 = pulseIn(echoPin2, HIGH);

  distance2 = duration2 * 0.034 / 2;

  Serial.print("Distance2: ");

  Serial.println(distance2);
  ButtonValue = digitalRead(buttonpin);

  if ((distance1 < 30 && distance1 > 20) || (distance2 < 30 && distance2 > 20))
  {

    digitalWrite(5, HIGH);

    delay(1000);

    digitalWrite(5, LOW);

    digitalWrite(5, HIGH);

    delay(1000);

    digitalWrite(5, LOW);

    digitalWrite(5, HIGH);

    delay(1000);

    digitalWrite(5, LOW);
  }

  else if ((distance1 < 20 && distance1 > 10) || (distance2 < 20 && distance2 > 10))
  {

    digitalWrite(5, HIGH);

    delay(500);

    digitalWrite(5, LOW);

    digitalWrite(5, HIGH);

    delay(500);

    digitalWrite(5, LOW);

    digitalWrite(5, HIGH);

    delay(500);

    digitalWrite(5, LOW);
  }

  else if ((distance1 < 10 && distance1 > 0) || (distance2 < 10 && distance2 > 0))
  {

    digitalWrite(5, HIGH);

    delay(100);

    digitalWrite(5, LOW);

    digitalWrite(5, HIGH);

    delay(100);

    digitalWrite(5, LOW);

    digitalWrite(5, HIGH);

    delay(100);

    digitalWrite(5, LOW);
  }

  digitalWrite(5, LOW);

  int sensorValue = digitalRead(A4);

  if (sensorValue == 0)
  {

    digitalWrite(5, HIGH);

    delay(1500);

    digitalWrite(5, LOW);

    digitalWrite(5, HIGH);

    delay(1500);
  }

  
  else if ( ButtonValue == 1 )

  {

    Serial.println("Button Pressed");
    delay(1000);
    Serial.println(ButtonValue);
    delay(2000);

    SendMessage();
  }

  if (gsmSerial.available() > 0)

    Serial.write(gsmSerial.read());

  while (gsmSerial.available())

  {

    gsmSerial.read();
  }

  while (Serial.available())

  {

    Serial.read();
  }

}

float *get_gps()

{

  gpsSerial.listen();

  Serial.println("INSIDE get_gps");

  while (1)
  {

    while (gpsSerial.available() > 0)

    {
      gps.encode(gpsSerial.read());
    }

    if (gps.location.isUpdated())

    {

      Serial.print("LAT=");
      Serial.println(gps.location.lat(), 6);

      Serial.print("LONG=");
      Serial.println(gps.location.lng(), 6);

      lattitude = gps.location.lat();

      longitude = gps.location.lng();

      break;
    }
  }

  a[0] = lattitude;

  a[1] = longitude;

  return a;
}


void SendMessage()

{
  Serial.println("\n==== Sending SMS...... ======");

  gsmSerial.println("AT+CMGF=1\r"); //Sets the GSM Module in Text Mode

  delay(1000); // Delay of 1000 milli seconds or 1 second

  gsmSerial.println("AT+CMGS=\"+917987460461\"\r"); // Replace x with mobile number

  delay(1000);

  gsmSerial.println("I am in problem plz help!! \n"); // The SMS text you want to send

  delay(1000);

  p = get_gps();

  gsmSerial.listen();

  Serial.print("Your position is : ");

  gsmSerial.print("Location is : ");

  Serial.print("LATTITUDE=");
  Serial.print(*p, 6);
  Serial.print("LONGITUDE=");
  Serial.print(*(p + 1), 6);

  gsmSerial.print("LATTITUDE=");
  gsmSerial.print(*p, 6); // The SMS text you want to send
  gsmSerial.print("\nLONGITUDE=");
  gsmSerial.print(*(p + 1), 6);
  
  gsmSerial.print("\nhttps://www.google.com/maps/search/?api=1&query=");
  gsmSerial.print(*p, 6);
  gsmSerial.print(","); // The SMS text you want to send
  gsmSerial.print(*(p + 1), 6); // The SMS text you want to send

  delay(100);

  gsmSerial.println((char)26);
  
  Serial.println("\n+++++ Text Sent. +++++");

}
