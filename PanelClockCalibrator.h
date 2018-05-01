#pragma once
#include <stdint.h>
#include <RotaryEncoder.h>
#include <arduino.h>

#define ANALOG_WRITE_MAX (1023)

class PanelClockCalibrator{
	public:
		PanelClockCalibrator() : PanelClockCalibrator(D5, D6, D7){}
		PanelClockCalibrator(int pinEncA, int pinEncB, int pinButton);
		void calibrate(uint32_t calibrationTable[][2], uint8_t calibrationPoints, int pin);
	
	private:
		int _pinEncA;
		int _pinEncB;
		int _pinButton;

		RotaryEncoder encoder;
		
		uint16_t waitForValue(int pin, uint16_t defaultValue, int16_t minVal, int16_t maxVal);
		template <typename Generic>
		inline void PCC_DEBUG(Generic text){
			Serial.print("**ClockCal: ");
			Serial.println(text);
		}
		
};