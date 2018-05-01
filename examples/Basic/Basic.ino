#include <PanelClock.h>

PanelClock clock(D0, D1, D2);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  clock.setDiscrete(true);
}

void loop() {
//  clock.updateTime(0,0,0);
//  delay(5000);
//  clock.updateTime(24,60,60);
//  delay(5000);
  clock.updateTime(12,30,30);
  delay(5000);
  
  analogWrite(D0, 511);
  analogWrite(D1, 511);
  analogWrite(D2, 511);
  delay(5000);
}
