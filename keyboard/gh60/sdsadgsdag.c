#include "keymap_common.h"
#include "light_ws2812.c"
#include "light_ws2812.h"
#include <avr/io.h>
#include "backlight.h"
#include <time.h>
#include <stdlib.h>

#define LED_COUNT 48

int bright=256;
unsigned char mRed = 256;
unsigned char mBlue = 256;
unsigned char mGreen = 256;
struct cRGB led[LED_COUNT];
void showColor(unsigned char red, unsigned char green, unsigned char blue)
{
	#ifdef __AVR_ATtiny10__
	CCP=0xD8;		// configuration change protection, write signature
	CLKPSR=0;		// set cpu clock prescaler =1 (8Mhz) (attiny 4/5/9/10)
	#else
	CLKPR=_BV(CLKPCE);
	CLKPR=0;			// set clock prescaler to 1 (attiny 25/45/85/24/44/84/13/13A)
	#endif
	int i;
	mRed = red;
	mGreen = green;
	mBlue = blue;
	for(i=0;i<=LED_COUNT;i++)
	{ 		led[i].r=red;
		led[i].g=green;
		led[i].b=blue;
	}
	ws2812_setleds(led,LED_COUNT);
}



void decrease()
{
	if(bright==6)
		bright=16;
	bright-=10;
}

void increase()
{
	if(bright==256)
		bright=246;
	bright+=10;
}

const uint8_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    KEYMAP(	F1,	F2, GRAVE,	1,	 2,   3,   4,   5,   6,   7,   8,   9,   0,   	MINS,	EQL, 	BSPC, \
			F3, F4,	TAB, 	Q,   W,   E,   R,   T,   Y,   U,   I,   O,   P,   	LBRC,	RBRC,	BSLS, \
			F5, F6, LCTL,	A,   S,   D,   F,   G,   H,   J,   K,   L,   SCLN,	QUOT, 	ENTER, 	INSERT,  \
			F7, F8, LSFT,	Z,   X,   C,   V,   B,   N,   M,   COMM,DOT, SLSH,RSFT,UP,DELETE, \
			F9, F10,CAPS,	LGUI,LALT,SPC, RALT,RGUI,FN0, RCTL,LEFT,DOWN,RIGHT),
	/* 1: FN0 */
	KEYMAP(	FN1, 	FN2, 	ESC,	TRNS,	TRNS,	TRNS,	TRNS,	TRNS,		TRNS,		TRNS,	TRNS,	TRNS,	TRNS,	TRNS,	TRNS,	TRNS,	\
			FN3,	FN4,	TRNS,	TRNS,	TRNS,	TRNS,	TRNS,	TRNS,		TRNS,		TRNS,	TRNS,	TRNS,	TRNS,	TRNS,	TRNS,	TRNS,	\
			FN5,	FN6,	TRNS,	TRNS,	TRNS,	TRNS,	TRNS,	TRNS,		TRNS,		TRNS,	TRNS,	TRNS,	TRNS,	TRNS,	TRNS,	HOME,	\
			FN7,	TRNS,	TRNS,	TRNS,	TRNS,	TRNS,	TRNS,	TRNS,		TRNS,		TRNS,	TRNS,	VOLU,	TRNS,	TRNS,	TRNS,	END,	\
			F11,	F12,	TRNS,	TRNS,	TRNS,	TRNS,	TRNS,	TRNS,		TRNS,		TRNS,	TRNS,	VOLD,	TRNS)
		
		
};
const uint16_t PROGMEM fn_actions[] = {
    [0] = ACTION_LAYER_MOMENTARY(1),
	[1] = ACTION_FUNCTION_TAP(BACKLIGHT_MODE1),
	[2] = ACTION_FUNCTION_TAP(BACKLIGHT_MODE2),
	[3] = ACTION_FUNCTION_TAP(BACKLIGHT_MODE3),
	[4] = ACTION_FUNCTION_TAP(BACKLIGHT_MODE4),
	[5] = ACTION_FUNCTION_TAP(BACKLIGHT_INCREASE),
	[6] = ACTION_FUNCTION_TAP(BACKLIGHT_DECREASE),
	[7] = ACTION_FUNCTION_TAP(BACKLIGHT_OFF)
};

void action_function(keyrecord_t *record, uint8_t id, uint8_t opt)
{
	switch (id)
	{
		case BACKLIGHT_MODE1:
		showColor(69*bright/256,39*bright/256,160*bright/256); //4527A0
		break;
		case BACKLIGHT_MODE2:
		showColor(00*bright/256,204*bright/256,204*bright/256); //HOLO YOLO
		break;
		case BACKLIGHT_MODE3:
		showColor(255*bright/256,255*bright/256,255*bright/256);
		break;
		case BACKLIGHT_MODE4:
		showColor(rand()%255*bright/256,rand()%255*bright/256,rand()%255*bright/256);
		break;
		case BACKLIGHT_INCREASE:
		increase();
		showColor(mRed,mGreen,mBlue);
		break;
		case BACKLIGHT_DECREASE:
		decrease();
		showColor(mRed,mGreen,mBlue);
		break;
		case BACKLIGHT_OFF:
		showColor(0,0,0);
		break;
		default:
		showColor(0,0,0);
		break;
	}
}

/* FN1 = Portal Orange
 * FN2 = Portal Blue
 * FN3 = White
 * FN4 = Random
 * FN5 = Increase brightness
 * FN6 = Decrease brightness
 * FN7 = Off
 */