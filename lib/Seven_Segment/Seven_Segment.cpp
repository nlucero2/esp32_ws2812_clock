/*!
 * @file Seven_Segment.cpp
 *
 * This provides the functions to set the time on a 4 digit 7 segment 
 * display comprised of Adafruit NeoPixels. 
 *
 */


// ***** includes *****

#include "Seven_Segment.h"

// ***** functions *****

/*!
 * @brief	constructor for the Seven_Segment class.
 * @param	pinNumber	the gpio pin number connected to the seven segment display.
 * @param	isAmPm		whether the time is displayed in 12-hour or 24-hour format.
 * @return	the Seven_Segment object.
 */
Seven_Segment::Seven_Segment(uint8_t pinNumber, bool isAmPm) {

    // set the pin number to default pin 48 if pinNumber is invalid
    pin =   ((pinNumber > 0 && pinNumber < 3)   || (pinNumber > 4 && pinNumber < 19) || 
             (pinNumber > 20 && pinNumber < 22) || (pinNumber > 37 && pinNumber < 43) || 
             (pinNumber > 46 && pinNumber < 49)) 
            ? pinNumber : 48;

	// set the given time format
	is12hr = isAmPm;
    
    // create the neopixel object, only 800 KHz is enabled in Adafruit_NeoPixel.h
    leds = Adafruit_NeoPixel(TOTAL_LEDS, pinNumber, NEO_GRB);

	// call the Adafruit_NeoPixel begin function
	leds.begin();

}// end constructor

/*!
 * @brief	empty constructor for the Seven_Segment class.
 * @return	the Seven_Segment object.
 */
Seven_Segment::Seven_Segment() {

    // set the pin number to default pin 48
    pin = 48;

	// set the time format to 12-hour
	is12hr = true;
    
    // create the neopixel object, only 800 KHz is enabled in Adafruit_NeoPixel.h
    leds = Adafruit_NeoPixel(TOTAL_LEDS, pin, NEO_GRB);

	// call the Adafruit_NeoPixel begin function
	leds.begin();

}// end constructor

/*!
 * @brief	destructor for the Seven_Segment class.
 */
Seven_Segment::~Seven_Segment() {

    // call the destructor for the neopixel object
    leds.~Adafruit_NeoPixel();

}// end destructor


/*!
 * @brief	sets the time on the 7 segment display.
 * @param	hour	the hour to set.
 * @param	minute	the minute to set.
 * @param	color	the color to set the segments to.
 * @note	call this function when setting the time for the first time.
 */
void Seven_Segment::setTime(uint32_t hour, uint32_t minute, uint32_t color) {
	
	// set all leds to off before setting the time
	leds.clear();
	
	// set the minutes
	setMinute(minute, color);
	
	// set the hours
	setHour(minute, color);
	
	// set the colon
	setColon(color);
	
	// only call show() after all leds have been set
	leds.show();
	
}// end setTime


/*!
 * @brief	takes the hours, minutes, color and only updates the segments on the display 
 *				that need modified from the previous time.
 * @param	hour	the hour to set.
 * @param	minute	the minute to set.
 * @param	color	the color to set the segments to.
 * @note	call this function when updating the time every minute.
 */
void Seven_Segment::updateTime(uint32_t hour, uint32_t minute, uint32_t color) {
	
	// update the minutes
	updateMinute(minute, color);
	
	// update the hours
	updateHour(minute, color);
	
	// set the colon
	setColon(color);
	
	// only call show() after all leds have been set
	leds.show();
	
}// end updateTime


/*!
 * @brief	takes the hours and sets the appropriate digits to the given color.
 * @param	hour	the hour to set.
 * @param	color	the color to set the segments to.
 */
void Seven_Segment::setHour(uint8_t hour, uint32_t color) {
	
	// get the value to set each digit to
	uint8_t hourDigits[2];
	
	// the least signigicant hour digit / digit-2
	hourDigits[0] = hour % 10;
	
	// the most signigicant hour digit / digit-3
	hourDigits[1] = TENS_HOUR(hour);
	
	// set digit-2 to the given color
	setDigit(2, digits[hourDigits[0]], color);
	
	// set digit-3 to the given color
	setDigit(3, digits[hourDigits[1]], color);
	
}// end setHour


/*!
 * @brief	takes the hours and only sets the segments that are needed, to 
 *				the given color.
 * @param	hour	the hour to set.
 * @param	color	the color to set the segments to.
 */
void Seven_Segment::updateHour(uint8_t hour, uint32_t color) {
	
	// set the max and min amount of hours
	uint8_t maxHour = is12hr ? 12 : 23;
	uint8_t minHour = is12hr ?  1 :  0;
	
	// determine the previous hour
	uint8_t prev = hour == minHour ? maxHour : hour - 1;
	
	// separate each digit
	uint8_t currDigits[2];
	uint8_t prevDigits[2];
	
	currDigits[0] = hour % 10;
	currDigits[1] = TENS_HOUR(hour);
	prevDigits[0] = prev % 10;
	prevDigits[1] = TENS_HOUR(prev);
	
	// loop through the 2 digits
	for (uint8_t i = 0; i < 2; ++i) {
		
		// get the digit bitmask
		uint8_t currMask = digits[currDigits[i]];
		uint8_t prevMask = digits[prevDigits[i]];
		
		// get the mask of the segments that need turned off
		uint8_t offMask = OFF_SEGMENTS(prevMask, currMask);
		
		// get the mask of the segments that need turned on
		uint8_t onMask = ON_SEGMENTS(prevMask, currMask);
		
		// if segments need turned off
		if (offMask & DIGIT_8) {
			
			// turn off the segments that need turned off
			setDigit(2 + i, offMask, 0);
		
		}// end if
		
		// if segments need turned on
		if (onMask & DIGIT_8) {
			
			// turn on the segments to the given color that need turned on
			setDigit(2 + i, onMask, color);
		
		}// end if
		
	}// end for
	
}// end updateHour


/*!
 * @brief	takes the minutes and sets the appropriate digits to the given color.
 * @param	minute	the minute to set.
 * @param	color	the color to set the segments to.
 */
void Seven_Segment::setMinute(uint8_t minute, uint32_t color) {
	
	// get the value to set each digit to
	uint8_t minuteDigits[2];
	
	minuteDigits[0] = minute % 10;
	minuteDigits[1] = TENS_MINUTE(minute);
	
	// set digit-0 to the given color
	setDigit(0, digits[minuteDigits[0]], color);
	
	// set digit-1 to the given color
	setDigit(1, digits[minuteDigits[1]], color);
	
}// end setMinute


/*!
 * @brief	takes the minutes and only sets the segments that are needed, to 
 *				the given color.
 * @param	minute	the minute to set.
 * @param	color	the color to set the segments to.
 */
void Seven_Segment::updateMinute(uint8_t minute, uint32_t color) {
	
	// determine the previous minute
	uint8_t prev = minute == 0 ? 59 : minute - 1;
	
	// create variables for the digits
	uint8_t currDigits[2];
	uint8_t prevDigits[2];
	
	// separate each digit
	currDigits[0] = minute % 10;
	currDigits[1] = TENS_MINUTE(minute);
	prevDigits[0] = prev % 10;
	prevDigits[1] = TENS_MINUTE(prev);
	
	// loop through the 2 digits
	for (uint8_t i = 0; i < 2; ++i) {
		
		// get the digit bitmask
		uint8_t currMask = digits[currDigits[i]];
		uint8_t prevMask = digits[prevDigits[i]];
		
		// get the mask of the segments that need turned off
		uint8_t offMask = OFF_SEGMENTS(prevMask, currMask);
		
		// get the mask of the segments that need turned on
		uint8_t onMask = ON_SEGMENTS(prevMask, currMask);
		
		// if segments need turned off
		if (offMask & DIGIT_8) {
			
			// turn off the segments that need turned off
			setDigit(i, offMask, 0);
		
		}// end if
		
		// if segments need turned on
		if (onMask & DIGIT_8) {
			
			// turn on the segments to the given color that need turned on
			setDigit(i, onMask, color);
		
		}// end if
		
	}// end for
	
}// end updateMinute


/*!
 * @brief	takes a digit number and a bitmask of segments and sets them to 
 *				the given color.
 * @param	digitNum	the digit number to set.
 * @param	digitMask	the bitmask of segments to set.
 * @param	color		the color to set the digit segments to.
 */
void Seven_Segment::setDigit(uint8_t digitNum, uint8_t digitMask, uint32_t color) {
	
	// loop through the segments in the digitMask
	for (uint8_t i = 0; i < SEGS_PER_DIGIT; ++i) {
		
		// create bitmask to match segments
		uint8_t segMask = 0x1 << i;
		
		// set each segment that's in digitMask
		switch (segMask & digitMask) {
			
			case SEGMENT_A:
				setSegment(digitNum, i, color);
				break;
			
			case SEGMENT_B:
				setSegment(digitNum, i, color);
				break;
			
			case SEGMENT_C:
				setSegment(digitNum, i, color);
				break;
			
			case SEGMENT_D:
				setSegment(digitNum, i, color);
				break;
			
			case SEGMENT_E:
				setSegment(digitNum, i, color);
				break;
			
			case SEGMENT_F:
				setSegment(digitNum, i, color);
				break;
			
			case SEGMENT_G:
				setSegment(digitNum, i, color);
				break;
				
		}// end switch
		
	}// end for
	
}// end setDigit


/*!
 * @brief	takes a digit number and a segment number and sets the segment leds to 
 *				the given color.
 * @param	digitNum	the digit number to set.
 * @param	segmentNum	the segment number to set.
 * @param	color		the color to set the segments to.
 */
void Seven_Segment::setSegment(uint8_t digitNum, uint8_t segmentNum, uint32_t color) {
	
	// calculate the index of the first led in the segment
	uint8_t index = digitNum * LEDS_PER_DIGIT + segmentNum * LEDS_PER_SEG;
	
	// loop through the leds in the segment
	for (int8_t i = 0; i < LEDS_PER_SEG; ++i) {
		
		// set the leds in the segment to the given color
		leds.setPixelColor(index + i, color);
		
	}// end for
	
}// end setSegment


/*!
 * @brief	takes a color and sets the colon leds to that color.
 * @param	color	the color to set the colon leds to.
 */
void Seven_Segment::setColon(uint32_t color) {
	
	// loop through the colon leds
	for (int8_t i = 0; i < COLON_LEDS; ++i) {
		
		// set the leds in the segment to the given color
		leds.setPixelColor(COLON_INDEX + i, color);
		
	}// end for
	
}// end setColon


/*!
 * @brief	takes a brightness value, from 0 to 255, and sets the brightness of the leds.
 */
void Seven_Segment::setBrightness(uint8_t brightness) {
	
	// set the brightness of the leds
	leds.setBrightness(brightness);
	
}// end setBrightness
