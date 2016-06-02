#include <pebble.h>
#include "Settings.h"
#include "Watchface.h"

// Set int setting
void Settings_Set(unsigned int setting, int value) {
	
	// Store in persistent settings
	persist_write_int(setting, value);
	
	// Trigger a watchface update
	Watchface_Refresh();
	
}

// Get int setting
int Settings_Get(unsigned int setting, int defaultValue) {
	
	// Check if exists
	if (!persist_exists(setting))
		return defaultValue;
	
	// Get value
	return persist_read_int(setting);
	
}