#define LedRed 2
#define LedYellow 3
#define LedGreen 4

void setup()
{
  pinMode(LedRed, OUTPUT);
  pinMode(LedYellow, OUTPUT);
  pinMode(LedGreen, OUTPUT);
}

void loop()
{
  digitalWrite(LedRed, HIGH);
  delay(5000);
  digitalWrite(LedRed, LOW);
  digitalWrite(LedGreen, HIGH);
  delay(5000);
  digitalWrite(LedGreen, LOW);
  digitalWrite(LedYellow, HIGH);
  delay(2000);
  digitalWrite(LedYellow, LOW);
}