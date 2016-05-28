#pragma once
#include <pebble.h>

struct _Resources {
	
	// Fonts
	struct {
		GFont time;
		GFont eventTitle;
		GFont eventSubtitle;
		GFont logo;
		GFont dayOfMonth;
	} fonts;
	
	// Icons
	struct {
		GDrawCommandImage* notification;
		GDrawCommandImage* calendar;
		GDrawCommandImage* warning;
		GDrawCommandImage* failed;
		GDrawCommandImage* confirmation;
		GDrawCommandImage* pin;
		GDrawCommandImage* battery;
		GDrawCommandImage* batteryCharging;
		GDrawCommandImage* sent;
		GDrawCommandImage* settings;
		GDrawCommandImage* event;
		GDrawCommandImage* rain;
		GDrawCommandImage* snow;
		GDrawCommandImage* cloudy;
		GDrawCommandImage* wind;
	} icons;
	
};

extern struct _Resources Resources;
	
void Resources_Init();