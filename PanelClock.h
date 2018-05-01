#pragma once
#include <arduino.h>
#include <stdint.h>
#include <ESPRotary.h>

#define SECONDS_IN_MINUTE 	(60UL)
#define MINUTES_IN_HOUR	  	(60UL)
#define HOURS_IN_DAY		(24UL)
#define SECONDS_IN_HOUR 	(SECONDS_IN_MINUTE * MINUTES_IN_HOUR)
#define SECONDS_IN_DAY 		(SECONDS_IN_HOUR * HOURS_IN_DAY)


#define PANEL_CLOCK_SUCCESS  (0)
#define PANEL_CLOCK_ERROR  	 (1)

class PanelClock {
  public:
    PanelClock() : PanelClock(D0, D1){} // default
    PanelClock(int pinHour, int pinMinute);
	PanelClock(int pinHour, int pinMinute, int pinSecond);
	void setDiscrete(bool discrete);
    uint16_t aproximateValue(uint32_t calibrationTable[][2], uint8_t tableSize, uint32_t value);
	uint8_t updateTime(int hours, int minutes, int seconds = 0);
	void showTime();
	
	inline void calibrateHours(uint32_t hourCal[][2], uint8_t hourCalSize){
		memcpy(this->hourCalibration, hourCal, hourCalSize * sizeof(uint32_t) * 2);
		this->hourCalSize = hourCalSize;
		for(uint8_t i = 0; i < 5; i++){
			PC_DEBUG(String(this->hourCalibration[i][0]) + " -> " + String(this->hourCalibration[i][1]));
		}
	}
	inline void calibrateMinutes(uint32_t minuteCal[5][2], uint8_t minuteCalSize){
		memcpy(this->minuteCalibration, minuteCal, minuteCalSize * sizeof(uint32_t) * 2);
		this->minuteCalSize = minuteCalSize;
	}
	inline void calibrateSeconds(uint32_t secondCal[5][2], uint8_t secondCalSize){
		memcpy(this->secondCalibration, secondCal, secondCalSize * sizeof(uint32_t) * 2);
		this->secondCalSize = secondCalSize;
	}
	
    template <typename Generic>
	inline void PC_DEBUG(Generic text){
		if(this->_debug) {
			Serial.print("**Clock: ");
			Serial.println(text);
		}
	}
	
    
  private:
    int _pinHour;
    int _pinMinute;
	int _pinSecond;
	
    int hours;
    int minutes;
	int seconds;
	bool discrete = false;
	bool _debug = true; 

	uint32_t hourCalibration[5][2] 		= {{0,0}, {SECONDS_IN_DAY, 1023}};
	uint8_t hourCalSize = 2;
	uint32_t minuteCalibration[5][2] 	= {{0,0}, {SECONDS_IN_HOUR, 1023}};
	uint8_t minuteCalSize = 2;
	uint32_t secondCalibration[2][2]	= {{0,0}, {60,1023}};
	uint8_t secondCalSize = 2;
	



	
};
