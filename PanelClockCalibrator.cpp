#include "PanelClockCalibrator.h"

PanelClockCalibrator::PanelClockCalibrator(int pinEncA, int pinEncB, int pinButton) : encoder (pinEncA, pinEncB,5,6,3000){
	this->_pinButton = pinButton;
	pinMode(pinButton, INPUT);
	digitalWrite(pinButton, HIGH);
}
void PanelClockCalibrator::calibrate(uint32_t calibrationTable[][2], uint8_t calibrationPoints, int pin){
	uint16_t pwmStep = 1023 / (calibrationPoints - 1);
	uint16_t min = 0;
	for (uint8_t point = 0; point < calibrationPoints; point++) {
		PCC_DEBUG("Waiting for " + String(calibrationTable[point][0]) + " calibration, click to continue");
		calibrationTable[point][1] = waitForValue(pin, pwmStep * point, min, ANALOG_WRITE_MAX);
		min = calibrationTable[point][1];
		delay(200);
	}
}

uint16_t PanelClockCalibrator::waitForValue(int pin, uint16_t defaultValue, int16_t minVal, int16_t maxVal){
	int16_t value = defaultValue;
	bool stop = false;
	int16_t encoderVal;
	while(!stop){
		analogWrite(pin, value);
		value += encoder.readEncoder();
		//make sure value is in bounds
		value = min(value, maxVal);
		value = max(value, minVal);
		if (!digitalRead(this->_pinButton)){
			while(!digitalRead(this->_pinButton)){
				delay(1);
			}
			stop = true;
		}
		delay(1);
	}
	return value;
}


