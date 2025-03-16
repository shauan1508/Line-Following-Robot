#include <Servo.h>
Servo myservo;

#define echo A4
#define trig A5
#define Mot 3
#define ENB 5
#define IN1 7
#define IN2 8
#define IN3 9
#define IN4 11
#define ENA 6
#define LeftS A2
#define MiddleS A1
#define RightS A0
#define min_threshold 38
#define max_threshold 800
#define MAX 700
#define MIN 100

int rightDistance = 0;
int leftDistance = 0;
int middleDistance = 0;

void setup()
{
    pinMode(ENA, OUTPUT);
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
    pinMode(ENB, OUTPUT);
    pinMode(trig, OUTPUT);
    pinMode(echo, INPUT);
    pinMode(LeftS, INPUT);
    pinMode(MiddleS, INPUT);
    pinMode(RightS, INPUT);
}

void loop()
{
    linefollow();
    hardcode();
    avoid();
}

void forward(int speed)
{
    analogWrite(ENA, speed);
    analogWrite(ENB, speed);
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    Serial.println("Forward");
}

void back(int speed)
{
    analogWrite(ENA, speed);
    analogWrite(ENB, speed);
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    Serial.println("Back");
}

void left(int speed, int del)
{
    analogWrite(ENA, speed);
    analogWrite(ENB, speed);
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    Serial.println("Left");
    delay(del);
    digitalWrite(Mot, LOW);
}

void right(int speed, int del)
{
    analogWrite(ENA, speed);
    analogWrite(ENB, speed);
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    Serial.println("Right");
    delay(del);
    digitalWrite(Mot, LOW);
}

void stop()
{
    digitalWrite(ENA, LOW);
    digitalWrite(ENB, LOW);
    Serial.println("Stop");
}

void readIR()
{
    float sensorL = analogRead(LeftS);
    float sensorM = analogRead(MiddleS);
    float sensorR = analogRead(RightS);
    
    Serial.print("L: ");
    Serial.println(sensorL);
    Serial.print("M: ");
    Serial.println(sensorM);
    Serial.print("R: ");
    Serial.println(sensorR);
}

void linefollow()
{
    while (true)
    {
        float sensorL = analogRead(LeftS);
        float sensorM = analogRead(MiddleS);
        float sensorR = analogRead(RightS);

        if (sensorL > MIN && sensorL < MAX && sensorM > MIN && sensorM < MAX && sensorR > MIN && sensorR < MAX)
        {
            digitalWrite(Mot, LOW);
            delay(2000);
            break;
        }

        if (sensorL > min_threshold && sensorL < max_threshold)
        {
            forward(50);
        }
        else if (sensorM > min_threshold && sensorM < max_threshold && sensorR < min_threshold)
        {
            forward(30);
        }
        else if (sensorM > min_threshold && sensorM < max_threshold && sensorR > min_threshold && sensorR < max_threshold)
        {
            forward(30);
        }
    }
}

int getDistance()
{
    digitalWrite(trig, LOW);
    delayMicroseconds(2);
    digitalWrite(trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig, LOW);
    int distance = (unsigned int)pulseIn(echo, HIGH) / 58;
    return distance;
}

void hardcode()
{
    forward(70);
    middleDistance = getDistance();
    if (middleDistance < 8 && middleDistance != 0)
    {
        digitalWrite(Mot, LOW);
        delay(1000);
        left(255, 145);
        delay(500);
        forward(70);
        middleDistance = getDistance();
        
        if (middleDistance < 8 && middleDistance != 0)
        {
            digitalWrite(Mot, LOW);
            delay(1000);
            left(255, 145);
            delay(500);
            forward(70);
            middleDistance = getDistance();
            
            if (middleDistance < 8 && middleDistance != 0)
            {
                digitalWrite(Mot, LOW);
                delay(1000);
                right(255, 150);
                delay(500);
                forward(70);
                middleDistance = getDistance();
                
                if (middleDistance < 8 && middleDistance != 0)
                {
                    digitalWrite(Mot, LOW);
                    delay(1000);
                    right(255, 150);
                    delay(500);
                    forward(70);
                    middleDistance = getDistance();
                    
                    if (middleDistance < 8 && middleDistance != 0)
                    {
                        digitalWrite(Mot, LOW);
                        delay(1000);
                        left(255, 145);
                        middleDistance = getDistance();
                        
                        if (middleDistance < 8 && middleDistance != 0)
                        {
                            digitalWrite(Mot, LOW);
                            delay(1000);
                            left(255, 145);
                            forward(70);
                            delay(1250);
                            digitalWrite(Mot, LOW);
                            delay(10000);
                        }
                    }
                }
            }
        }
    }
}

void avoid()
{
    while (true)
    {
        forward(90);
        middleDistance = getDistance();

        if (middleDistance <= 8 && middleDistance != 0)
        {
            digitalWrite(Mot, LOW);
            delay(1000);
            right(255, 150);
            delay(1000);
            rightDistance = getDistance();
            delay(1000);
            left(255, 145);
            delay(1000);
            left(255, 145);
            delay(1000);
            leftDistance = getDistance();
            delay(1000);
        }

        if (rightDistance > leftDistance)
        {
            right(255, 150);
            delay(2000);
            right(255, 150);
            delay(1000);
        }
        if (leftDistance < 10 && rightDistance < 10)
        {
            left(255, 145);
            delay(1000);
        }
        else
        {
            forward(90);
        }
    }
}
