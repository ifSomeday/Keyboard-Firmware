#include "keymap_common.h"
#include "light_ws2812.c"
#include "light_ws2812.h"
#include <avr/io.h>
#include "backlight.h"
#include <time.h>
#include <stdlib.h>
#define LED_COUNT 48

int bright=256;
int lightMode = 0;
int rainbowOffset = 0;
unsigned char mRed = 256;
unsigned char mBlue = 256;
unsigned char mGreen = 256;
struct cRGB led[LED_COUNT];
int invoCounts[3] = {1,1,1};
int invokerColors[3][3] = {{255,0,255},{0,255,255},{255,128,0}};
int invokerCurr[3] = {0,1,2};
int rainbow[48][3] = {{255,0,0},{255,31,0},{255,63,0},{255,95,0},{255,127,0},{255,159,0},{255,191,0},{255,223,0},{254,255,0},{223,255,0},{191,255,0},{159,255,0},{127,255,0},{95,255,0},{63,255,0},{31,255,0},{0,255,0},{0,255,31},{0,255,63},{0,255,95},{0,255,127},{0,255,159},{0,255,191},{0,255,223},{0,254,255},{0,223,255},{0,191,255},{0,159,255},{0,127,255},{0,95,255},{0,63,255},{0,31,255},{0,0,255},{31,0,255},{63,0,255},{95,0,255},{127,0,255},{159,0,255},{191,0,255},{223,0,255},{255,0,254},{255,0,223},{255,0,191},{255,0,159},{255,0,127},{255,0,95},{255,0,63},{255,0,31}};
int invokerSections[3][17] = {{0,1,2,3,4,5,37,38,39,40,41,42,43,44,45,46,47},{6,7,8,9,10,11,12,30,31,32,33,34,35,36,6,6,6},{13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29}};

typedef struct {
    double r;       // percent
    double g;       // percent
    double b;       // percent
} rgb;

typedef struct {
    double h;       // angle in degrees
    double s;       // percent
    double v;       // percent
} hsv;
static rgb   hsv2rgb(hsv in);

void showColor(unsigned char red, unsigned char green, unsigned char blue)
{
	#ifdef __AVR_ATtiny10__
	CCP=0xD8;		// configuration change protection, write signature
	CLKPSR=0;		// set cpu clock prescaler =1 (8Mhz) (attiny 4/5/9/10)
	#else
	CLKPR=_BV(CLKPCE);
	CLKPR=0; // set clock prescaler to 1 (attiny 25/45/85/24/44/84/13/13A)
	#endif
	int i;
	lightMode = 0;
	red = red*bright/256;
	green = green*bright/256;
	blue = blue*bright/256;
	for(i=0;i<=LED_COUNT;i++)
	{ 	
		led[i].r=red;
		led[i].g=green;
		led[i].b=blue;
	}
	ws2812_setleds(led,LED_COUNT);
}




rgb hsv2rgb(hsv in)
{
    double      hh, p, q, t, ff;
    long        i;
    rgb         out;

    if(in.s <= 0.0) {       // < is bogus, just shuts up warnings
        out.r = in.v;
        out.g = in.v;
        out.b = in.v;
        return out;
    }
    hh = in.h;
    if(hh >= 360.0) hh -= 360.0;
    hh /= 60.0;
    i = (long)hh;
    ff = hh - i;
    p = in.v * (1.0 - in.s);
    q = in.v * (1.0 - (in.s * ff));
    t = in.v * (1.0 - (in.s * (1.0 - ff)));

    switch(i) {
    case 0:
        out.r = in.v;
        out.g = t;
        out.b = p;
        break;
    case 1:
        out.r = q;
        out.g = in.v;
        out.b = p;
        break;
    case 2:
        out.r = p;
        out.g = in.v;
        out.b = t;
        break;

    case 3:
        out.r = p;
        out.g = q;
        out.b = in.v;
        break;
    case 4:
        out.r = t;
        out.g = p;
        out.b = in.v;
        break;
    case 5:
    default:
        out.r = in.v;
        out.g = p;
        out.b = q;
        break;
    }
    return out;
}

void rainbowFade(){
	#ifdef __AVR_ATtiny10__
	CCP=0xD8;		// configuration change protection, write signature
	CLKPSR=0;		// set cpu clock prescaler =1 (8Mhz) (attiny 4/5/9/10)
	#else
	CLKPR=_BV(CLKPCE);
	CLKPR=0;			// set clock prescaler to 1 (attiny 25/45/85/24/44/84/13/13A)
	#endif
	int i;

	for(i=0;i<=LED_COUNT;i++)
	{
		led[i].r=rainbow[i][0]*bright/256;
		led[i].g=rainbow[i][1]*bright/256;
		led[i].b=rainbow[i][2]*bright/256;
	}
	ws2812_setleds(led,LED_COUNT);

}

void rotatingRainbow2(){
	rainbowOffset = rainbowOffset%360;
	for(int i=0;i<=LED_COUNT;i++){
		hsv col = {((7.5*i)+rainbowOffset),1,1};
		rgb col2 = hsv2rgb(col);
		led[i].r=col2.r*255*bright/256;
		led[i].g=col2.g*255*bright/256;
		led[i].b=col2.b*255*bright/256;
	}
	rainbowOffset++;
	ws2812_setleds(led,LED_COUNT);
}


void unicornVomit(){
	
	#ifdef __AVR_ATtiny10__
	CCP=0xD8;		// configuration change protection, write signature
	CLKPSR=0;		// set cpu clock prescaler =1 (8Mhz) (attiny 4/5/9/10)
	#else
	CLKPR=_BV(CLKPCE);
	CLKPR=0;			// set clock prescaler to 1 (attiny 25/45/85/24/44/84/13/13A)
	#endif
	int i;
	lightMode = 1;
	for(i=0;i<=LED_COUNT;i++)
	{ 	
		led[i].r=rand()%255*bright/256;
		led[i].g=rand()%255*bright/256;
		led[i].b=rand()%255*bright/256;
	}
	ws2812_setleds(led,LED_COUNT);

}

void updateSet(int m){
	invokerCurr[2] = invokerCurr[1];
	invokerCurr[1] = invokerCurr[0];
	invokerCurr[0] = m;
}

void invokerMode(int m){
	#ifdef __AVR_ATtiny10__
	CCP=0xD8;		// configuration change protection, write signature
	CLKPSR=0;		// set cpu clock prescaler =1 (8Mhz) (attiny 4/5/9/10)
	#else
	CLKPR=_BV(CLKPCE);
	CLKPR=0;			// set clock prescaler to 1 (attiny 25/45/85/24/44/84/13/13A)
	#endif
	if (m != 4){
		if(invoCounts[m]){
			updateSet(m);
			invoCounts[m] = 0;
		} else {
			invoCounts[m] = 1;
		}

	}
	int i, j, k;
	lightMode = 3;
	for(k = 0; k <= 2; k++){
		for(i = 0; i <=17; i++){
			j = invokerSections[k][i];
			led[j].r = invokerColors[invokerCurr[k]][0]*bright/256;
			led[j].g = invokerColors[invokerCurr[k]][1]*bright/256;
			led[j].b = invokerColors[invokerCurr[k]][2]*bright/256;
		}
	}
	ws2812_setleds(led,LED_COUNT);
}


void decrease()
{
	if(bright==6)
		bright=16;
	bright-=10;
	
	switch(lightMode){
	case 0:
		showColor(mRed,mGreen,mBlue);
		break;
	case 1:
		unicornVomit();
		break;
	case 2:
		rainbowFade();
		break;
	case 3:
		invokerMode(4);
		break;
	}

}

void increase()
{
	if(bright==256)
		bright=246;
	bright+=10;
	
	switch(lightMode){
	case 0:
		showColor(mRed,mGreen,mBlue);
		break;
	case 1:
		unicornVomit();
		break;
	case 2:
		rainbowFade();
		break;
	case 3:
		invokerMode(4);
		break;
	}
}

const uint8_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    KEYMAP(	F1,	F2, 	ESC,	1,	 2,   3,   4,   5,   6,   7,   8,   9,   0,   	MINS,	EQL, 	BSPC, \
			F3, F4,		TAB, 	Q,   W,   E,   R,   T,   Y,   U,   I,   O,   P,   	LBRC,	RBRC,	BSLS, \
			F5, F6, 	LCTL,	A,   S,   D,   F,   G,   H,   J,   K,   L,   SCLN,	QUOT, 	ENTER, 	INSERT,  \
			F7, F8, 	LSFT,	Z,   X,   C,   V,   B,   N,   M,   COMM,DOT, SLSH,RSFT,UP,DELETE, \
			F9, PAUS,	CAPS,	LGUI,LALT,SPC, RALT,RGUI,FN0, RCTL,LEFT,DOWN,RIGHT),
	/* 1: FN0 */
	KEYMAP(	FN1, 	FN2, 	ESC,	FN9,	FN10,	FN11,	TRNS,	TRNS,		TRNS,		TRNS,	TRNS,	TRNS,	TRNS,	TRNS,	EQL,	TRNS,	\
			FN3,	FN4,	TRNS,	TRNS,	TRNS,	TRNS,	TRNS,	TRNS,		TRNS,		TRNS,	TRNS,	TRNS,	TRNS,	TRNS,	TRNS,	TRNS,	\
			FN5,	FN6,	TRNS,	TRNS,	TRNS,	TRNS,	TRNS,	TRNS,		TRNS,		TRNS,	TRNS,	TRNS,	TRNS,	TRNS,	TRNS,	HOME,	\
			FN7,	FN8,	TRNS,	TRNS,	TRNS,	TRNS,	TRNS,	TRNS,		TRNS,		TRNS,	TRNS,	TRNS,	TRNS,	TRNS,	VOLU,	END,	\
			F11,	F12,	TRNS,	TRNS,	TRNS,	TRNS,	TRNS,	TRNS,		TRNS,		TRNS,	TRNS,	VOLD,	TRNS),
	/*invoker mode, fn10*/
	KEYMAP(	TRNS, 	TRNS, 	TRNS,	TRNS,	TRNS,	TRNS,	TRNS,	TRNS,		TRNS,		TRNS,	TRNS,	TRNS,	TRNS,	TRNS,	TRNS,	TRNS,	\
			TRNS,	TRNS,	TRNS,	FN25,	FN26,	FN27,	TRNS,	TRNS,		TRNS,		TRNS,	TRNS,	TRNS,	TRNS,	TRNS,	TRNS,	TRNS,	\
			TRNS,	TRNS,	TRNS,	TRNS,	TRNS,	TRNS,	TRNS,	TRNS,		TRNS,		TRNS,	TRNS,	TRNS,	TRNS,	TRNS,	TRNS,	TRNS,	\
			TRNS,	TRNS,	TRNS,	TRNS,	TRNS,	TRNS,	TRNS,	TRNS,		TRNS,		TRNS,	TRNS,	TRNS,	TRNS,	TRNS,	TRNS,	TRNS,	\
			TRNS,	TRNS,	TRNS,	TRNS,	TRNS,	TRNS,	TRNS,	TRNS,		TRNS,		TRNS,	TRNS,	TRNS,	TRNS)
		
		
};
const uint16_t PROGMEM fn_actions[] = {
    [0] = ACTION_LAYER_TAP_KEY(1,KC_GRV),
	[1] = ACTION_FUNCTION_TAP(BACKLIGHT_MODE1),
	[2] = ACTION_FUNCTION_TAP(BACKLIGHT_MODE2),
	[3] = ACTION_FUNCTION_TAP(BACKLIGHT_MODE3),
	[4] = ACTION_FUNCTION_TAP(BACKLIGHT_MODE4),
	[5] = ACTION_FUNCTION(BACKLIGHT_DECREASE),
	[6] = ACTION_FUNCTION(BACKLIGHT_INCREASE),
	[7] = ACTION_FUNCTION_TAP(BACKLIGHT_OFF),
	[8] = ACTION_FUNCTION_TAP(UNICORN_VOMIT),
	[9] = ACTION_FUNCTION_TAP(RAINBOW_FADE),
	[10] = ACTION_LAYER_TOGGLE(2),
	[11] = ACTION_FUNCTION_TAP(ROTATE),
	[25] = ACTION_MACRO(1),
	[26] = ACTION_MACRO(2),
	[27] = ACTION_MACRO(3)
};

enum macro_id {
	QUAS,
	WEX,
	EXORT
};

const macro_t *action_get_macro(keyrecord_t *record, uint8_t id, uint8_t opt)
{
    switch (id) {
        case 1:
        	invokerMode(0);
            return (record->event.pressed ?
                    MACRO( D(Q), END ) :
                    MACRO( U(Q), END ));
        case 2:
        	invokerMode(1);
            return (record->event.pressed ?
                    MACRO( D(W), END ) :
                    MACRO( U(W), END ));
        case 3:
        	invokerMode(2);
            return (record->event.pressed ?
                    MACRO( D(E), END ) :
                    MACRO( U(E), END ));
    }
    return MACRO_NONE;
}


void action_function(keyrecord_t *record, uint8_t id, uint8_t opt)
{
	switch (id)
	{
		case BACKLIGHT_MODE1:
		mRed = 69;
		mGreen = 39;
		mBlue = 160;
		showColor(mRed,mGreen,mBlue); //4527A0
		break;
		case BACKLIGHT_MODE2:
		mRed = 00;
		mGreen =204;
		mBlue =204;
		showColor(mRed,mGreen,mBlue); //HOLO YOLO
		break;
		case BACKLIGHT_MODE3:
		mRed = 0;
		mBlue = 255;
		mGreen = 0;
		showColor(mRed,mGreen,mBlue);
		break;
		case BACKLIGHT_MODE4:
		mRed = rand()%256;
		mBlue = rand()%256;
		mGreen = rand()%256;
		showColor(mRed,mGreen,mBlue);
		break;
		case BACKLIGHT_INCREASE:
		increase();
		break;
		case BACKLIGHT_DECREASE:
		decrease();
		break;
		case BACKLIGHT_OFF:
		showColor(0,0,0);
		break;
		case UNICORN_VOMIT:
		unicornVomit();
		break;
		case RAINBOW_FADE:
		lightMode = 2;
		rainbowOffset = 0;
		rainbowFade();
		break;
		case ROTATE:
		lightMode = 4;
		break;
		default:
		showColor(0,0,0);
		break;
	}
}
