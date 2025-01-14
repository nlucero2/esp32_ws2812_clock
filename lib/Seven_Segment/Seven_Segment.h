/*!
 * @file Seven_Segment.h
 *
 * This provides the functions to set the time on a 4 digit 7 segment 
 * display comprised of Adafruit NeoPixels. 
 *
 */


#ifndef SEVEN_SEHMENT_H
#define SEVEN_SEHMENT_H


/* the digit layout and led data flow direction
 * 
 *	  c		|	   2→    
 *	b   d	|	↑1    ↓3 
 *	  a		|	   ←0    
 *	g   e	|	↑6    ↓4 
 *	  f		|	   ←5    
*/

/*
 *  dig-3   dig-2     dig-1   dig-0         digit-3   digit-2      digit-1   digit-0
 *	  c       c	 	    c       c  		|	   2→        2→           2→        2→   
 *	b   d   b   d  🞿  b   d   b   d		|	↑1    ↓3  ↑1    ↓3  🞿  ↑1    ↓3  ↑1    ↓3
 *	  a       a		    a       a  		|	   ←0        ←0           ←0        ←0   
 *	g   e   g   e  🞿  g   e   g   e		|	↑6    ↓4  ↑6    ↓4  🞿  ↑6    ↓4  ↑6    ↓4
 *	  f       f		    f       f  		|	   ←5        ←5           ←5        ←5   
*/


// ***** includes *****

#include <Adafruit_NeoPixel.h>


// ***** macros *****

// determines which segments to turn off
#define OFF_SEGMENTS(oldNum, newNum) ((oldNum ^ newNum) & oldNum)

// determines which segments to turn off
#define ON_SEGMENTS(oldNum, newNum)  ((oldNum ^ newNum) & newNum)

// determines the value of the second digit for minutes
#define TENS_MINUTE(val) ((val > 9) ? (val / 10) : 0)

// determines the value of the second digit for hours
#define TENS_HOUR(val) ((val > 9) ? (val / 10) : 10)


// ***** defines *****

// the number of leds used to display the colon
#define COLON_LEDS		2

// the number of leds per segment
#define LEDS_PER_SEG	3

// the number of segments in each digit
#define SEGS_PER_DIGIT	7

// the number of digits on the clock (2 for hour and 2 for minute)
#define NUM_OF_DIGITS	4

// the number of leds in each digit
#define LEDS_PER_DIGIT	(LEDS_PER_SEG * SEGS_PER_DIGIT)

// the total number of digit leds and the index of the first colon led
#define COLON_INDEX		(NUM_OF_DIGITS * LEDS_PER_DIGIT)

// the total number of leds
#define TOTAL_LEDS		(COLON_INDEX + COLON_LEDS)

// the bitmasks for each segment
//					gfedcba
#define SEGMENT_A 0b0000001		// Segment a
#define SEGMENT_B 0b0000010		// Segment b
#define SEGMENT_C 0b0000100		// Segment c
#define SEGMENT_D 0b0001000		// Segment d
#define SEGMENT_E 0b0010000		// Segment e
#define SEGMENT_F 0b0100000		// Segment f
#define SEGMENT_G 0b1000000		// Segment g


// the bitmasks for each digit and one for all segments off
//				  gfedcba			[gfedcba]	[   segments  ]
#define DIGIT_0 0b1111110	// 0	[gfedcb ]	[  b c d e f g]
#define DIGIT_1 0b0011000	// 1	[  ed   ]	[      d e    ]
#define DIGIT_2 0b1101101	// 2	[gf dc a]	[a   c d   f g]
#define DIGIT_3 0b0111101	// 3	[ fedc a]	[a   c d e f  ]
#define DIGIT_4 0b0011011	// 4	[  ed ba]	[a b   d e    ]
#define DIGIT_5 0b0110111	// 5	[ fe cba]	[a b c   e f  ]
#define DIGIT_6 0b1110111	// 6	[gfe cba]	[a b c   e f g]
#define DIGIT_7 0b0011100	// 7	[  edc  ]	[    c d e    ]
#define DIGIT_8 0b1111111	// 8	[gfedcba]	[a b c d e f g]
#define DIGIT_9 0b0011111	// 9	[  edcba]	[a b c d e    ]
#define DIGIT_X 0b0000000	// OFF	[       ]	[             ]

// ***** bitmasks *****

// segment bitmask array
static const uint8_t segments[SEGS_PER_DIGIT] = {
	SEGMENT_A,
	SEGMENT_B,
	SEGMENT_C,
	SEGMENT_D,
	SEGMENT_E,
	SEGMENT_F,
	SEGMENT_G
};

// digit bitmask array
static const uint8_t digits[11] = {
	DIGIT_0,
	DIGIT_1,
	DIGIT_2,
	DIGIT_3,
	DIGIT_4,
	DIGIT_5,
	DIGIT_6,
	DIGIT_7,
	DIGIT_8,
	DIGIT_9,
	DIGIT_X
};


/*!
    @brief  Class that stores state and functions for interacting with
            Adafruit NeoPixels and compatible devices.
*/
class Seven_Segment {
        
private:

    // the gpio pin number connected to the seven segment display
    uint8_t pin;

    // whether the time os displayed in 12-hour or 24-hour format
    bool is12hr;

    // the neopixel object
    Adafruit_NeoPixel leds;


public:

    // constructor: gpio pin number connected to the seven segment display, AM/PM or 24-hour format
    Seven_Segment(uint8_t pinNumber, bool isAmPm);
    Seven_Segment(void);
    ~Seven_Segment();


    void setTime(uint32_t hour, uint32_t minute, uint32_t color);

    void updateTime(uint32_t hour, uint32_t minute, uint32_t color);

    void setHour(uint8_t hour, uint32_t color);

    void updateHour(uint8_t hour, uint32_t color);

    void setMinute(uint8_t minute, uint32_t color);

    void updateMinute(uint8_t minute, uint32_t color);

    void setDigit(uint8_t digitNum, uint8_t digitMask, uint32_t color);

    void setSegment(uint8_t digitNum, uint8_t segmentNum, uint32_t color);

    void setColon(uint32_t color);

    void setBrightness(uint8_t brightness);

}; // end Seven_Segment class

#endif // SEVEN_SEHMENT_H
