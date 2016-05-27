#include <pebble.h>
#include "Watchface.h"
#include "EventStore.h"
#include "BatteryEvents.h"
#include "Resources.h"
#include "BluetoothEvents.h"

// Main app entry point
int main() {
	
	// Load resources
	Resources_Init();
	
	// Load event store
	EventStore_Init();
	
	// Create example event
	//Event* event = EventStore_Create("Test event");
	//EventStore_Add(event);
	
	// Load event monitor services
	BatteryEvents_Init();
	BluetoothEvents_Init();
	
	// Show window
	Watchface_Show();
	
	// Run event loop
  	app_event_loop();
	
	// Remove window
	Watchface_DeInit();
	
}
