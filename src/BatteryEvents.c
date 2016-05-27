#include <pebble.h>
#include "BatteryEvents.h"
#include "EventStore.h"

#define BATTERY_EVENT_ID	0x01

// Start the battery monitor service
void BatteryEvents_Init() {
	
	// Subscribe to battery state changes
	battery_state_service_subscribe(BatteryEvents_OnChange);
	
	// Trigger first check
	BatteryEvents_OnChange(battery_state_service_peek());
	
}

// Called when the battery state changes
void BatteryEvents_OnChange(BatteryChargeState charge) {
	
	// Remove old event if there is one
	EventStore_RemoveID(BATTERY_EVENT_ID);
	
	// Check if charging
	if (charge.is_charging) {
		
		// Still charging, create event
		Event* e = EventStore_Create("Charging");
		snprintf(e->subtitle, EVENT_STRING_LENGTH, "%i%% remaining", charge.charge_percent);
		e->eventID = BATTERY_EVENT_ID;
		e->time = time(0);
		e->duration = 60 * 60 * 24;
		e->hidden = true;
		e->color = EVENT_COLOR_YELLOW;
		e->noSave = true;
		e->type = EVENT_TYPE_GENERIC;
		EventStore_Add(e);
		return;
		
	}
	
	// Check if plugged in but not charging
	if (charge.is_plugged) {
		
		// Still charging, create event
		Event* e = EventStore_Create("Charge Complete");
		strncpy(e->subtitle, " ", EVENT_STRING_LENGTH);
		e->eventID = BATTERY_EVENT_ID;
		e->time = time(0);
		e->duration = 60 * 60 * 24;
		e->hidden = true;
		e->color = EVENT_COLOR_GREEN;
		e->noSave = true;
		e->type = EVENT_TYPE_CONFIRM;
		EventStore_Add(e);
		return;
		
	}
	
	// Check battery level
	if (charge.charge_percent <= 10) {
		
		// Low battery, create event
		Event* e = EventStore_Create("Low Battery");
		snprintf(e->subtitle, EVENT_STRING_LENGTH, "%i%% remaining", charge.charge_percent);
		e->eventID = BATTERY_EVENT_ID;
		e->time = time(0);
		e->duration = 60 * 60 * 24;
		e->hidden = true;
		e->color = EVENT_COLOR_RED;
		e->noSave = true;
		e->type = EVENT_TYPE_WARNING;
		EventStore_Add(e);
		return;
		
	}
	
}