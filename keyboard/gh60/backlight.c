/*int bright=256;
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
	for(i=0;i<=LED_COUNT;i++)
	{
		led[i].r=red;
		led[i].g=green;
		led[i].b=blue;
	}
	ws2812_setleds(led,LED_COUNT);
}*/

/*void action_function(keyrecord_t *record, uint8_t id, uint8_t opt)
{
	switch (id)
	{
		case BACKLIGHT_MODE1:
		showColor(253*(bright/256),102*(bright/256),0); //Portal orange
		break;
		case BACKLIGHT_MODE2:
		showColor(0,120*(bright/256),255*bright/256); //Portal blue
		break;
		case BACKLIGHT_MODE3:
		showColor(255*(bright/256),255*(bright/256),255*(bright/256));
		break;
		case BACKLIGHT_MODE4:
		showColor(rand()%255*(bright/256),rand()%255*(bright/256),rand()%255*(bright/256));
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
		default:
		showColor(0,0,0);
		break;
	}
}*/

/*void decrease()
{
	bright-=50;
	if(bright==6)
		bright=6;
}

void increase()
{
	bright+=50;
	if(bright==256)
		bright=256;
}*/