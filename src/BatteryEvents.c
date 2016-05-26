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
	
	// Check battery level
	if (charge.charge_percent <= 100) {
		
		// Low battery, create event
		Event* e = EventStore_Create("Low Battery");
		snprintf(e->subtitle, 128, "%i%% remaining", charge.charge_percent);
		e->eventID = BATTERY_EVENT_ID;
		e->time = time(0);
		e->duration = 60 * 60 * 24;
		e->hidden = true;
		e->color = EVENT_COLOR_RED;
		e->noSave = true;
		e->type = EVENT_TYPE_FAILED;
		EventStore_Add(e);
		
	}
	
}