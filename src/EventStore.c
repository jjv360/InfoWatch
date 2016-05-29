#include <pebble.h>
#include "EventStore.h"
#include "MathUtil.h"
#include "Resources.h"
#include "Watchface.h"
#include <math.h>

#define KEY_NUM_EVENTS					0x100
#define KEY_EVENTS_OFFSET				0x200
#define KEY_EVENT_SIZE					0x10
#define KEY_OFFSET_EVENT_NAME			0x00
#define KEY_OFFSET_EVENT_TIME			0x01
#define KEY_OFFSET_EVENT_COLOR			0x02
#define KEY_OFFSET_EVENT_HIDDEN			0x03
#define KEY_OFFSET_EVENT_DURATION		0x04
#define KEY_OFFSET_EVENT_SUBTITLE		0x05
#define KEY_OFFSET_EVENT_TYPE			0x06
#define KEY_OFFSET_EVENT_ID				0x07

#define MAX_EVENTS						40

static Event* firstEvent;


// Load events from storage
void EventStore_Init() {
	
	// Stop if no data
	if (!persist_exists(KEY_NUM_EVENTS))
		return;
	
	// Load values
	int numEvents = persist_read_int(KEY_NUM_EVENTS);
	for (int i = 0 ; i < MIN(numEvents, MAX_EVENTS) ; i++) {
		
		// Create event
		Event* e = malloc(sizeof(Event));
		
		// Read event data
		int keyOffset = KEY_EVENTS_OFFSET + KEY_EVENT_SIZE * i;
		persist_read_string(keyOffset + KEY_OFFSET_EVENT_NAME, e->name, 128);
		persist_read_string(keyOffset + KEY_OFFSET_EVENT_SUBTITLE, e->subtitle, 128);
		e->time = persist_read_int(keyOffset + KEY_OFFSET_EVENT_TIME);
		e->duration = persist_read_int(keyOffset + KEY_OFFSET_EVENT_DURATION);
		e->color = persist_read_int(keyOffset + KEY_OFFSET_EVENT_COLOR);
		e->hidden = persist_read_bool(keyOffset + KEY_OFFSET_EVENT_HIDDEN);
		e->type = persist_read_int(keyOffset + KEY_OFFSET_EVENT_TYPE);
		e->eventID = persist_read_int(keyOffset + KEY_OFFSET_EVENT_ID);
		
		// Add it
		EventStore_Add(e);
		
	}
	
}

// Save current event data
void EventStore_Save() {
	
	// First purge old items
	EventStore_Purge();
	
	// Store each item
	int numEvents = 0;
	Event* e = firstEvent;
	while (e) {
		
		// Move on
		Event* event = e;
		e = e -> _next;
		
		// Check if should save
		if (event -> noSave)
			continue;
		
		// Store event data
		int keyOffset = KEY_EVENTS_OFFSET + KEY_EVENT_SIZE * numEvents;
		persist_write_string(keyOffset + KEY_OFFSET_EVENT_NAME, event->name);
		persist_write_string(keyOffset + KEY_OFFSET_EVENT_SUBTITLE, event->subtitle);
		persist_write_int(keyOffset + KEY_OFFSET_EVENT_TIME, event->time);
		persist_write_int(keyOffset + KEY_OFFSET_EVENT_DURATION, event->duration);
		persist_write_int(keyOffset + KEY_OFFSET_EVENT_COLOR, event->color);
		persist_write_bool(keyOffset + KEY_OFFSET_EVENT_HIDDEN, event->hidden);
		persist_write_int(keyOffset + KEY_OFFSET_EVENT_TYPE, event->type);
		persist_write_int(keyOffset + KEY_OFFSET_EVENT_ID, event->eventID);
		
		// Increase count
		numEvents++;
		
	}
	
	// Store number of items
	persist_write_int(KEY_NUM_EVENTS, numEvents);
	
}

// Save soon. This is used to collapse multiple save requests within a second into one save request
static AppTimer* saveTimer = 0;
void EventStore_SaveSoon() {
	
	// Reschedule old save timer or create a new one
	if (saveTimer)
		app_timer_reschedule(saveTimer, 1000);
	else
		saveTimer = app_timer_register(1000, EventStore_SaveTick, 0);
	
}

// Called when the save timer executes
void EventStore_SaveTick(void* data) {
	
	// Clear save timer
	saveTimer = 0;
	
	// Save
	EventStore_Save();
	
	/* Log events
	APP_LOG(APP_LOG_LEVEL_INFO, "Events: ");
	Event* e = firstEvent;
	while (e) {
		APP_LOG(APP_LOG_LEVEL_INFO, "- (%u) %s at %u", e->eventID, e->name, (unsigned int) e->time);
		e = e -> _next;
	}*/
	
}

// Purge old items from the event store
void EventStore_Purge() {
	
	// Go through each item
	int purgeFrom = time(0) - 60 * 15;
	Event* e = firstEvent;
	while (e) {
		
		// Move on
		Event* event = e;
		e = e -> _next;
		
		// Check if it's later than purge time
		if (event->time + event->duration > purgeFrom)
			continue;
		
		// This entry is too old, remove it
		EventStore_Remove(event);
		
	}
	
}

// Remove the event at the specified index
void EventStore_Remove(Event* event) {
	
	// Check for null
	if (!event)
		return;
	
	// Check if there's a previous one
	Event* prev = (Event*) event -> _prev;
	Event* next = (Event*) event -> _next;
	if (prev) {
		
		// Take this event out of the linked list
		prev -> _next = next;
		if (next)
			next -> _prev = prev;
		
	} else {
	
		// This was the first event
		firstEvent = next;
		if (firstEvent)
			firstEvent -> _prev = 0;
		
	}
	
	// Free memory
	free(event);
	
	// Update UI
	Watchface_Refresh();
	
	// Save soon
	EventStore_SaveSoon();
	
}

// Remove the event with the specified ID
void EventStore_RemoveID(int id) {
	
	// Go through events
	Event* e = firstEvent;
	while (e) {
		
		// Move on
		Event* event = e;
		e = e -> _next;
		
		// Check ID
		if (event->eventID != id)
			continue;
		
		// Match found, remove it
		EventStore_Remove(event);
		
	}
	
}

// Remove the furthest entry
void EventStore_RemoveFurthest() {
	
	// Find furthest entry
	Event* e = firstEvent;
	Event* oldest = firstEvent;
	while (e) {
		
		// Check date
		if (oldest->time < e->time)
			oldest = e;
		
		// Move on
		e = e -> _next;
		
	}
	
	// Remove it if found
	if (oldest)
		EventStore_Remove(oldest);
	
}

// Create a new event
Event* EventStore_Create(char* name) {
	
	// Create event
	Event* event = malloc(sizeof(Event));
	memset(event, 0, sizeof(Event));
	
	// Generate event ID
	int id = rand();
	event->eventID = id;
	
	// Copy event name
	strncpy(event->name, name, EVENT_STRING_LENGTH);
	
	// Set defaults
	event->time = time(0);
	
	// Done
	return event;
	
}

// Adds an event to the event store
void EventStore_Add(Event* event) {
	
	// Check for null
	if (!event)
		return;
	
	// Remove other events with this event ID
	EventStore_RemoveID(event->eventID);
	
	// Make space
	while (EventStore_Count() >= MAX_EVENTS)
		EventStore_RemoveFurthest();
	
	// Get last event
	Event* lastEvent = firstEvent;
	while (lastEvent && lastEvent->_next)
		lastEvent = lastEvent -> _next;
	
	// Add it
	if (lastEvent) {
	
		// Add to end of linked list
		lastEvent -> _next = event;
		event -> _prev = lastEvent;
		
	} else {
		
		// This is the first event
		firstEvent = event;
		
	}
	
	// Update UI
	Watchface_Refresh();
	
	// Save soon
	EventStore_SaveSoon();
	
}

// Count number of items in the event store
int EventStore_Count() {
	
	// Count events
	int numEvents = 0;
	Event* e = firstEvent;
	while (e) {
		numEvents++;
		e = e -> _next;
	}
	
	// Done
	return numEvents;
	
}

// Get the first event in the store
Event* EventStore_First() {
	return firstEvent;
}

// Get the next event after the specified event
Event* EventStore_Next(Event* event) {
	return event ? event->_next : 0;
}

// Get the GColor for a specified event color
GColor EventStore_Color(int eventColor) {
	
	// Check if got a color display
	#if defined(PBL_COLOR)
	
		// Choose the right color
		if (eventColor == EVENT_COLOR_GREEN)
			return GColorGreen;
		else if (eventColor == EVENT_COLOR_RED)
			return GColorRed;
		else if (eventColor == EVENT_COLOR_BLUE)
			return GColorBlue;
		else if (eventColor == EVENT_COLOR_BROWN)
			return GColorBrass;
		else if (eventColor == EVENT_COLOR_YELLOW)
			return GColorYellow;
		else
			return GColorWhite;
		
	#else
		
		// Only black and white supported
		if (eventColor == EVENT_COLOR_WHITE)
			return GColorWhite;
		else
			return GColorLightGray;
		
	#endif
	
}

// Get the current event, either in progress or next to happen
Event* EventStore_Current() {
	
	// Go through events to get the current one
	Event* closestEvent = 0;
	time_t now = time(0);
	time_t limit = now + 60 * 60 * 24 * 31;
	Event* e = firstEvent;
	while (e) {
		
		// Move on
		Event* event = e;
		e = e -> _next;
		
		// Check if inside event
		if (now >= event->time && now <= event->time + event->duration) {
			
			// This is it
			return event;
			
		}
		
		// Check if event is too far away
		if (event->time > limit)
			continue;
		
		// Check if we have an event already
		if (!closestEvent) {
			closestEvent = event;
			continue;
		}
		
		// Check if our event is closer
		if (event->time < closestEvent->time) {
			closestEvent = event;
			continue;
		}
		
	}
	
	// Done
	return closestEvent;
	
}

// Get relative time to event
void EventStore_CopyRelativeTimeText(const Event* event, char* bfr, int length) {
	
	// Check for null
	if (!event)
		return;
	
	// Check if in the future
	time_t now = time(0);
	if (now < event->time) {
		
		// In the future, get delay
		int delay = (event->time) - now;
		if (delay < 60 * 60) {
			
			// Show time
			int numMins = round((double) delay / 60);
			if (numMins == 0)
				snprintf(bfr, length, "Now");
			else if (numMins == 1)
				snprintf(bfr, length, "In one minute");
			else
				snprintf(bfr, length, "In %i minutes", numMins);
			
		} else if (delay < 60 * 60 * 24) {
			
			// Show time in hours
			int numHours = round((double) delay / 60 / 60);
			if (numHours == 1)
				snprintf(bfr, length, "In one hour");
			else
				snprintf(bfr, length, "In %i hours", numHours);
			
		} else {
		
			// Show time in days
			int numDays = round((double) delay / 60 / 60 / 24);
			if (numDays == 1)
				snprintf(bfr, length, "In one day");
			else
				snprintf(bfr, length, "In %i days", numDays);
			
		}
		
	} else if (now >= event->time && now < event->time + event->duration) {
		
		// Currently inside the event, get delay
		int delay = (event->time + event->duration) - now;
		if (delay < 60 * 60) {
			
			// Show time
			int numMins = round((double) delay / 60);
			if (numMins == 0)
				snprintf(bfr, length, "Just ended");
			else if (numMins == 1)
				snprintf(bfr, length, "Ends in one minute");
			else
				snprintf(bfr, length, "Ends in %i minutes", numMins);
			
		} else if (delay < 60 * 60 * 24) {
			
			// Show time in hours
			int numHours = round((double) delay / 60 / 60);
			if (numHours == 1)
				snprintf(bfr, length, "Ends in one hour");
			else
				snprintf(bfr, length, "Ends in %i hours", numHours);
			
		} else {
		
			// Show time in days
			int numDays = round((double) delay / 60 / 60 / 24);
			if (numDays == 1)
				snprintf(bfr, length, "Ends in one day");
			else
				snprintf(bfr, length, "Ends in %i days", numDays);
			
		}
		
	} else {
		
		// In the past, get delay
		int delay = now - (event->time + event->duration);
		if (delay < 60 * 60) {
			
			// Show time
			int numMins = round((double) delay / 60);
			if (numMins == 0)
				snprintf(bfr, length, "Now");
			else if (numMins == 1)
				snprintf(bfr, length, "One minute ago");
			else
				snprintf(bfr, length, "%i minutes ago", numMins);
			
		} else if (delay < 60 * 60 * 24) {
			
			// Show time in hours
			int numHours = round((double) delay / 60 / 60);
			if (numHours == 1)
				snprintf(bfr, length, "One hour ago");
			else
				snprintf(bfr, length, "%i hours ago", numHours);
			
		} else {
		
			// Show time in days
			int numDays = round((double) delay / 60 / 60 / 24);
			if (numDays == 1)
				snprintf(bfr, length, "One day ago");
			else
				snprintf(bfr, length, "%i days ago", numDays);
			
		}
		
	}
	
}

// Copy the event's subtitle text
void EventStore_CopySubtitle(const Event* event, char* bfr, int length) {
	
	// Check if got a subtitle
	if (strlen(event->subtitle) == 0) {
		
		// No subtitle, use relative time
		EventStore_CopyRelativeTimeText(event, bfr, length);
		
	} else {
		
		// Copy subtitle
		strncpy(bfr, event->subtitle, length);
		
	}
	
}


// Get image for event type
GDrawCommandImage* EventStore_SmallIcon(Event* event) {
	
	// Find correct image
	if (event -> type == EVENT_TYPE_WARNING)
		return Resources.icons.warning;
	else if (event -> type == EVENT_TYPE_FAILED)
		return Resources.icons.failed;
	else if (event -> type == EVENT_TYPE_CONFIRM)
		return Resources.icons.confirmation;
	else if (event -> type == EVENT_TYPE_PIN)
		return Resources.icons.pin;
	else if (event -> type == EVENT_TYPE_BATTERY)
		return Resources.icons.battery;
	else if (event -> type == EVENT_TYPE_BATTERY_CHARGING)
		return Resources.icons.batteryCharging;
	else if (event -> type == EVENT_TYPE_SENT)
		return Resources.icons.sent;
	else if (event -> type == EVENT_TYPE_SETTINGS)
		return Resources.icons.settings;
	else if (event -> type == EVENT_TYPE_EVENT)
		return Resources.icons.event;
	else if (event -> type == EVENT_TYPE_RAIN)
		return Resources.icons.rain;
	else if (event -> type == EVENT_TYPE_SNOW)
		return Resources.icons.snow;
	else if (event -> type == EVENT_TYPE_CLOUDY)
		return Resources.icons.cloudy;
	else if (event -> type == EVENT_TYPE_WIND)
		return Resources.icons.wind;
	else
		return Resources.icons.notification;
	
}



// Draw the event in the specified area on-screen
void EventStore_DrawEvent(Event* event, GContext* ctx, GRect bounds) {
	
	// Draw colored background
	int boxSize = 28;
	GColor color = EventStore_Color(event->color);
	graphics_context_set_fill_color(ctx, color);
	graphics_fill_rect(ctx, GRect(bounds.origin.x + bounds.size.w / 2 - boxSize / 2, bounds.origin.y + 25 / 2 - boxSize / 2, boxSize, boxSize), 4, GCornersAll);
	
	// Draw image
	GDrawCommandImage* img = EventStore_SmallIcon(event);
	int offset = bounds.origin.y;
	gdraw_command_image_draw(ctx, img, GPoint(bounds.origin.x + bounds.size.w / 2 - 25 / 2, offset));
	offset += 24;
	
	// Draw title
	graphics_context_set_text_color(ctx, GColorWhite);
	graphics_draw_text(ctx, event->name, Resources.fonts.eventTitle, GRect(bounds.origin.x, offset, bounds.size.w, 20), GTextOverflowModeTrailingEllipsis, GTextAlignmentCenter, 0);
	offset += 18;
	
	// Get subtitle
	char bfr[64];
	EventStore_CopySubtitle(event, bfr, 64);
	
	// Draw subtitle
	graphics_context_set_text_color(ctx, GColorLightGray);
	graphics_draw_text(ctx, bfr, Resources.fonts.eventSubtitle, GRect(bounds.origin.x, offset, bounds.size.w, 20), GTextOverflowModeTrailingEllipsis, GTextAlignmentCenter, 0);
	offset += 14;
	
}