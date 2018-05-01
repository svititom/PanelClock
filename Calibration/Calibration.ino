#include <PanelClockCalibrator.h>

PanelClockCalibrator cal;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  uint32_t calTable[5][2] = {{0,0}, {6,0},{12,0},{18,0},{24,0}};
  cal.calibrate(calTable, 5, D0);

  Serial.println("Cal points:");
  for (uint8_t i; i < 5; i++){
    Serial.println(String(calTable[i][0]) + " -> " + String(calTable[i][1]));
  }

}

void loop() {
  // put your main code here, to run repeatedly:

}
