#include <pebble.h>
#include "AppMessage.h"
#include "EventStore.h"
#include "Settings.h"

#define DICT_KEY_ACTION		0
#define DICT_KEY_EVENTID	1
#define DICT_KEY_TIME		2
#define DICT_KEY_COLOR		3
#define DICT_KEY_TYPE		4
#define DICT_KEY_HIDDEN		5
#define DICT_KEY_NOSAVE		6
#define DICT_KEY_NAME		7
#define DICT_KEY_SUBTITLE	8
#define DICT_KEY_DURATION	9

#define ACTION_REMOVE_ID	0
#define ACTION_ADD_EVENT	1
#define ACTION_UPDATE_PING	2

#define UPDATE_PING_INTERVAL	1000 * 60 * 60

static AppTimer* pingTimer = 0;

void AppMessage_Init() {
	
	// Connect to app message service
	app_message_register_inbox_received(AppMessage_OnIncomingMessage);
	app_message_open(app_message_inbox_size_maximum(), APP_MESSAGE_OUTBOX_SIZE_MINIMUM);
	
	// Start update ping
	pingTimer = app_timer_register(UPDATE_PING_INTERVAL, AppMessage_OnUpdatePing, 0);
	
}

/** Called when the update ping timer fires */
void AppMessage_OnUpdatePing(void* data) {
	
	// Schedule another ping
	pingTimer = app_timer_register(UPDATE_PING_INTERVAL, AppMessage_OnUpdatePing, 0);
	
	// Create app message to send
	DictionaryIterator* dict = 0;
	app_message_outbox_begin(&dict);
	if (!dict)
		return;
	
	// Write action
	dict_write_uint8(dict, DICT_KEY_ACTION, ACTION_UPDATE_PING);
	
	// Send message
	app_message_outbox_send();
	
}

/** Called when a new message is received from the phone */
void AppMessage_OnIncomingMessage(DictionaryIterator* iterator, void *context) {
	
	// Get action
	Tuple* tuple = dict_find(iterator, DICT_KEY_ACTION);
	int action = (tuple ? tuple->value->int32 : -1);
	
	// Check what to do
	if (action == ACTION_REMOVE_ID) {
		
		// Get event ID to remove
		tuple = dict_find(iterator, DICT_KEY_EVENTID);
		int id = (tuple ? tuple->value->int32 : -1);
		
		// Remove it
		EventStore_RemoveID(id);
		
	} else if (action == ACTION_ADD_EVENT) {
		
		// Get event ID
		tuple = dict_find(iterator, DICT_KEY_EVENTID);
		int id = (tuple ? tuple->value->int32 : -1);
		if (!tuple)
			return;
		
		// Create event
		Event* event = EventStore_Create("Untitled Event");
		event->eventID = id;
		
		// Get time
		tuple = dict_find(iterator, DICT_KEY_TIME);
		event->time = (tuple ? tuple->value->uint32 : 0);
		
		// Get color
		tuple = dict_find(iterator, DICT_KEY_COLOR);
		event->color = (tuple ? tuple->value->uint32 : 0);
		
		// Get type
		tuple = dict_find(iterator, DICT_KEY_TYPE);
		event->type = (tuple ? tuple->value->uint32 : 0);
		
		// Get hidden
		tuple = dict_find(iterator, DICT_KEY_HIDDEN);
		event->hidden = (tuple ? tuple->value->uint32 : 0);
		
		// Get noSave
		tuple = dict_find(iterator, DICT_KEY_NOSAVE);
		event->noSave = (tuple ? tuple->value->uint32 : 0);
		
		// Get name
		tuple = dict_find(iterator, DICT_KEY_NAME);
		if (tuple && strlen(tuple->value->cstring) > 0)
			strncpy(event->name, tuple->value->cstring, EVENT_STRING_LENGTH);
		
		// Get subtitle
		tuple = dict_find(iterator, DICT_KEY_SUBTITLE);
		if (tuple && strlen(tuple->value->cstring) > 0)
			strncpy(event->subtitle, tuple->value->cstring, EVENT_STRING_LENGTH);
		
		// Get duration
		tuple = dict_find(iterator, DICT_KEY_DURATION);
		event->duration = (tuple ? tuple->value->uint32 : 0);
		
		// Add event
		EventStore_Add(event);
		
	}
	
	// Update hour hand setting
	tuple = dict_find(iterator, SETTING_HOUR_HAND_MODE);
	if (tuple && strcmp(tuple->value->cstring, "auto") == 0)
		Settings_Set(SETTING_HOUR_HAND_MODE, 0);
	else if (tuple && strcmp(tuple->value->cstring, "on") == 0)
		Settings_Set(SETTING_HOUR_HAND_MODE, 1);
	else if (tuple && strcmp(tuple->value->cstring, "off") == 0)
		Settings_Set(SETTING_HOUR_HAND_MODE, 2);
	
	// Update event dot setting
	tuple = dict_find(iterator, SETTING_SHOW_EVENT_DOTS);
	if (tuple)
		Settings_Set(SETTING_SHOW_EVENT_DOTS, tuple->value->int32);
	
	// Update vibrate on new event setting
	tuple = dict_find(iterator, SETTING_VIBRATE_ON_NEW_EVENT);
	if (tuple)
		Settings_Set(SETTING_VIBRATE_ON_NEW_EVENT, tuple->value->int32);
	
}