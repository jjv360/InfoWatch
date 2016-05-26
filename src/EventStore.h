#pragma once
#include <pebble.h>

#define EVENT_COLOR_WHITE		1
#define EVENT_COLOR_GREEN		2
#define EVENT_COLOR_RED			3
#define EVENT_COLOR_BLUE		4
#define EVENT_COLOR_BROWN		5

#define EVENT_TYPE_GENERIC		0
#define EVENT_TYPE_WARNING		1
#define EVENT_TYPE_FAILED		2

typedef struct {
	int eventID;
	char name[128];
	char subtitle[128];
	time_t time;
	time_t duration;
	int color;
	bool hidden;
	bool noSave;
	int type;
	void* _next;
	void* _prev;
} Event;

void 	EventStore_Init();
void 	EventStore_Save();
void 	EventStore_Purge();

Event* 	EventStore_Create(char* name);
void	EventStore_Add(Event* event);
int  	EventStore_Count();
Event* 	EventStore_First();
Event* 	EventStore_Next(Event* event);
void 	EventStore_Remove(Event* e);
void	EventStore_RemoveID(int id);
Event*	EventStore_Current();

GColor 	EventStore_Color(int eventColor);
void 	EventStore_CopyRelativeTimeText(const Event* event, char* bfr, int length);
void	EventStore_CopySubtitle(const Event* event, char* bfr, int length);