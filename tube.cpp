#include<arduino.h>
#include"tube.h"
void Tube::tube_init()
{
  for (int i = 6; i < 18; i++)
    pinMode(i, OUTPUT);
  pinMode(20, INPUT);
}
void Tube::tube_turnoff()
{
  for (int i = 6; i < 14; i++)
    digitalWrite(i, HIGH);
}
