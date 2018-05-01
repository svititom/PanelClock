#include "PanelClock.h"
PanelClock::PanelClock(int pinHour, int pinMinute){
	this->_pinHour = pinHour;
	this->_pinMinute = pinMinute;	
}

PanelClock::PanelClock(int pinHour, int pinMinute, int pinSecond){
	this->_pinHour = pinHour;
	this->_pinMinute = pinMinute;
	this->_pinSecond = pinSecond;
}

void PanelClock::setDiscrete(bool discrete){
	this->discrete = discrete;
}



uint8_t PanelClock::updateTime(int hours, int minutes, int seconds){
	uint8_t result = PANEL_CLOCK_SUCCESS;
	if (hours > 24 || hours < 0) {
		return PANEL_CLOCK_ERROR;
	}
	if (minutes > 60 || minutes < 0) {
		return PANEL_CLOCK_ERROR;
	}
	if (seconds > 60 || seconds < 0) {
		return PANEL_CLOCK_ERROR;
	}
	
	this->hours = hours;
	this->minutes = minutes;
	this->seconds = seconds;
	this->showTime();
	return result;
}
uint32_t getMinutesInSeconds(uint32_t minutes, uint32_t seconds){
	return minutes * (uint32_t) SECONDS_IN_MINUTE + seconds;
}

uint32_t getHoursInSeconds(uint32_t hours, uint32_t minutes, uint32_t seconds){
	return hours * (uint32_t) SECONDS_IN_HOUR + getMinutesInSeconds(minutes, seconds);
}

/*
	@Brief returns value in calibration table, or linearly aproximates the mapping, rounding down
	E.g. calTalb = {{0,0}{10, 1023}}, val 10 returns 1023, val 5 returns 511
	Calibration
*/
uint16_t PanelClock::aproximateValue(uint32_t calibrationTable[][2], uint8_t tableSize, uint32_t value){
	if (tableSize < 2 || value < calibrationTable[0][0]) {
		return 0;
	}
	// Find nearest value in table
	if(value == calibrationTable[0][0]){
		return calibrationTable[0][1];
	}
	uint8_t index;
	for (index = 1; index < tableSize; index++){
		PC_DEBUG(String(calibrationTable[index][0]) + " -> " + String(calibrationTable[index][1]));
		if(value <= calibrationTable[index][0]){
			break;
		}
	} 
	PC_DEBUG("Index: " + String(index));
	// We now have an index > 0, and we know that the value is less
	uint32_t lowerBoundPre = calibrationTable[index - 1][0];
	uint32_t upperBoundPre = calibrationTable[index][0];
	// we need to work with doubles, because the stepSize is very likely to have a decimal part
	uint16_t lowerBoundMap = calibrationTable[index - 1][1];
	uint16_t upperBoundMap = calibrationTable[index][1];
	PC_DEBUG(String(lowerBoundPre) + ":"+String(upperBoundPre)+" -> "+String(lowerBoundMap) + ":"+String(upperBoundMap));
	uint16_t mappedValue = map(value, lowerBoundPre, upperBoundPre, lowerBoundMap, upperBoundMap);
	PC_DEBUG("Mapped: " + String(value) + " to: " + String(mappedValue));
	
	return mappedValue;
}

void PanelClock::showTime(){
	uint32_t hourSeconds;
	uint32_t minuteSeconds;
	if (discrete) {
		hourSeconds = getHoursInSeconds(this->hours, 0, 0);
		minuteSeconds = getMinutesInSeconds(this->minutes, 0);
	} else {
		hourSeconds = getHoursInSeconds(this->hours, this->minutes, this->seconds);
		minuteSeconds = getMinutesInSeconds(this->minutes, this->seconds);
	}
	
	/*
		Perform mapping from time to analog out
	*/
	
	
	uint16_t mappedHours = aproximateValue(this->hourCalibration, this->hourCalSize, hourSeconds);
	uint16_t mappedMinutes = aproximateValue(this->minuteCalibration, this->minuteCalSize, minuteSeconds);
	uint16_t mappedSeconds = aproximateValue(this->secondCalibration, this->secondCalSize, this->seconds);
	
	analogWrite(this->_pinHour, mappedHours);
	analogWrite(this->_pinMinute, mappedMinutes);
	analogWrite(this->_pinSecond, mappedSeconds);

}

/* 

void PanelClock::calibrate(uint8_t points){
	this->calibrateDigit(this->_pinHour, 24, 5, hourCalibration);
}


uint8_t PanelClock::calibrateDigit(uint8_t pin, uint8_t maxVal, uint8_t points, uint16_t (&calibrationTable)[5][2]){
	if (points > 5 || points < 2) {
		return PANEL_CLOCK_ERROR;
	}
	uint16_t pwmStep = 1023 / (points - 1);
	uint8_t digitStep = maxVal / (points - 1);
	for (uint8_t point = 0; point < points; point++) {
		uint16_t value = waitForValue(pin, pwmStep * point);
		calibrationTable[0][point] = digitStep * point;
		calibrationTable[1][point] = value;
	}
	// Copy the last data point into all consequent entries (3 points calibration, last 3 entries will be the same)
	for (uint8_t point = points; point < 5; point++){
		calibrationTable[0][point] = calibrationTable[0][points];
		calibrationTable[1][point] = calibrationTable[1][points];
	}
	return PANEL_CLOCK_SUCCESS;
}

uint16_t PanelClock::waitForValue(uint8_t pin, uint16_t initialValue){
	if (initialValue > 1023) {
		initialValue = 1023;
	}
	analogWrite(pin, initialValue);
	this->calibrationValue = initialValue;
	this->registerRotaryCallbacks(pin);
	while(digitalRead(this->_pinConfirm)){
		this->_rotaryInput.loop();
		yield;
	}
	this->unRegisterRotaryCallbacks();
	uint16_t value = calibrationValue;
	return value;
}

void PanelClock::leftTurn(ESPRotary& e) {
	
}
void PanelClock::rightTurn(ESPRotary& e) {
	
}


void PanelClock::registerRotaryCallbacks(int pin) {
	this->controlledPin = pin;
	this->_rotaryInput.setRightRotationHandler((void *) (this->leftTurn)());
}

void PanelClock::unRegisterRotaryCallbacks() {
	this->_rotaryInput.restoreCallbacks();
}

 */