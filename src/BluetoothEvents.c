#include <pebble.h>
#include "BluetoothEvents.h"
#include "EventStore.h"

#define BLUETOOTH_EVENT_ID		0x02

void BluetoothEvents_Init() {
	
	// Add listener for connection service
	ConnectionHandlers handlers = {0, 0};
	handlers.pebble_app_connection_handler = BluetoothEvents_OnChange;
	connection_service_subscribe(handlers);
	
}

void BluetoothEvents_OnChange(bool connected) {
	
	// Remove old event if there is one
	EventStore_RemoveID(BLUETOOTH_EVENT_ID);
	
	// Stop if connected
	if (connected)
		return;
	
	// Create disconnection event
	Event* e = EventStore_Create("Disconnected");
	strncpy(e->subtitle, "Check phone", EVENT_STRING_LENGTH);
	e->eventID = BLUETOOTH_EVENT_ID;
	e->time = time(0);
	e->duration = 60 * 60 * 24 * 365;
	e->hidden = true;
	e->color = EVENT_COLOR_RED;
	e->noSave = true;
	e->type = EVENT_TYPE_WARNING;
	EventStore_Add(e);
	
}