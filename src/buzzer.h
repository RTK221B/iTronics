/**/

#define LED = 13;
void setup()
{
    pinMode(LED, OUTPUT);
}

void loop()
{
    digitalWrite(LED, HIGH);
    delay(80);
    digitalWrite(LED, LOW);
    delay(80);
    digitalWrite(LED, HIGH);
    delay(80);
    digitalWrite(LED, LOW);
    delay(80);
}