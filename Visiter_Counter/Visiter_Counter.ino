/*Program is written on 15-04-2022
 * By Vrishabh Verma
 * code has been tested in Arduino IDE
 */


#define in 14
#define out 19
#define relay 2

int count=0;

void IN()
{
    count++;
    Serial.print("Person In Room:");
    Serial.print(count);
    Serial.print("\n");
    delay(1000);
}

void OUT()
{
    count--;
    Serial.print("Person In Room:");
    Serial.print(count);
    Serial.print("\n");
    delay(1000);
}

void setup()
{
  
  Serial.begin(9600);
  Serial.print("Visitor Counter");
  delay(2000);
  pinMode(in, INPUT);
  pinMode(out, INPUT);
  pinMode(relay, OUTPUT);
  Serial.print("Person In Room:");
  Serial.print(count);
  Serial.print("\n");

}

void loop()
{  
  
  if(digitalRead(in))
  IN();
  if(digitalRead(out))
  OUT();
  
  if(count<=0)
  {
    
    digitalWrite(relay, LOW);
    Serial.print("Nobody In Room\n");
    delay(200);
    Serial.print("Turning Lights Off....");
    delay(200);
    Serial.print("\n Lights Off.");
    delay(200);
  }
  else
    digitalWrite(relay, HIGH);
  
}
