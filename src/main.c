#include <pebble.h>
#include "Watchface.h"
#include "EventStore.h"
#include "BatteryEvents.h"
#include "Resources.h"
#include "BluetoothEvents.h"
#include "AppMessage.h"

// Main app entry point
int main() {
	
	// Load resources
	Resources_Init();
	
	// Load event store
	EventStore_Init();
	
	// Create example event
	for (int i = 0 ; i < 12 ; i++) {
		Event* event = EventStore_Create("Test event");
		event->time = time(0) + rand() % (60 * 60 * 12);
		event->color = rand() % 6 + 1;
		EventStore_Add(event);
	}
	
	// Load event monitor services
	BatteryEvents_Init();
	BluetoothEvents_Init();
	
	// Start connection to phone
	AppMessage_Init();
	
	// Show window
	Watchface_Show();
	
	// Run event loop
  	app_event_loop();
	
	// Remove window
	Watchface_DeInit();
	
}
