#include <pebble.h>
#include "Watchface.h"
#include "EventStore.h"
#include "BatteryEvents.h"

// Main app entry point
int main() {
	
	// Load event store
	EventStore_Init();
	
	// Load event monitor services
	BatteryEvents_Init();
	
	// Show window
	Watchface_Show();
	
	// Run event loop
  	app_event_loop();
	
	// Remove window
	Watchface_DeInit();
	
}
