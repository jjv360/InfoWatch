#include <pebble.h>
#include "Watchface.h"
#include "MathUtil.h"
#include "EventStore.h"
#include "Resources.h"

static Window* window = 0;
static char timeLblBuffer[128] = {0};

static GColor backgroundColor = {0};
static struct tm now = {0};


// Set up the window
void Watchface_Init() {
	
	// Set defaults
	backgroundColor = GColorBlack;
	
	// Create window
	window = window_create();
	window_set_background_color(window, GColorBlack);
	
	// Get window root layer
	Layer* root = window_get_root_layer(window);
	
	// Set custom background drawing function
	layer_set_update_proc(root, Watchface_DrawBackground);
	
	// Subscribe to the tick timer service, and update every minute
	tick_timer_service_subscribe(MINUTE_UNIT, Watchface_OnTimeChanged);
	
}

// Show the window
void Watchface_Show() {
	
	// Setup if needed
	if (!window)
		Watchface_Init();
	
	// Push the window onto the stack
	window_stack_push(window, false);
	
}

// Destroy the window
void Watchface_DeInit() {
	
	// Destroy window
	window_destroy(window);
	window = 0;
	
}



// Called when the time changes
void Watchface_OnTimeChanged(struct tm *tick_time, TimeUnits units_changed) {
	
	// Store new time
	if (tick_time)
		memcpy(&now, tick_time, sizeof(now));
	
	// Refresh the digital time
	clock_copy_time_string(timeLblBuffer, 128);
	
	// Redraw the background
	layer_mark_dirty(window_get_root_layer(window));
	
}


// Update the watchface
void Watchface_Refresh() {
	
	// Check if set up
	if (!window)
		return;
	
	// Redraw the background
	layer_mark_dirty(window_get_root_layer(window));
	
}


// Calculates the intersection point of a box and a line from the center of the box with a specific angle
GPoint calculateBoxAngleIntersection(GRect box, int angle, int granularity) {
	
	// Check granularity
	if (granularity <= 0)
		granularity = 1;
	
	// Create point
	GPoint p;
	p.x = box.origin.x + box.size.w / 2;
	p.y = box.origin.y + box.size.h / 2;
	
	// Loop through sizes of line until it's outside the box
	int lineLength = 10;
	int centerX = p.x;
	int centerY = p.y;
	int angleThetaX = sin_lookup(angle);
	int angleThetaY = -cos_lookup(angle);
	while (true) {
		
		// Calculate new line point
		lineLength += granularity;
		int x = angleThetaX * lineLength / TRIG_MAX_RATIO + centerX;
		int y = angleThetaY * lineLength / TRIG_MAX_RATIO + centerY;
		
		// Check if still inside
		GPoint np = GPoint(x, y);
		if (!grect_contains_point(&box, &np))
			break;
			
		// Update point
		p = np;
		
	}
				  
	// Return point
	return p;
	
}

// Calculate a point on a circle
GPoint calculatePointOnCircle(GPoint center, int angle, int radius) {
	
	// Calculate new line point
	GPoint p;
	p.x = sin_lookup(angle) * radius / TRIG_MAX_RATIO + center.x;
	p.y = -cos_lookup(angle) * radius / TRIG_MAX_RATIO + center.y;
	return p;
	
}



// Called to draw the background of the window
void Watchface_DrawBackground(struct Layer *layer, GContext *ctx) {
	
	// Get window size
	GRect bounds = layer_get_bounds(layer);
	
	// Fill with background color
	graphics_context_set_fill_color(ctx, backgroundColor);
	graphics_fill_rect(ctx, bounds, 0, 0);
	
	// Purge old events
	EventStore_Purge();
	
	// Draw events around the circle
	int pointOffset = 4;
	time_t ignoreBefore = time(0) - 60 * 10;
	time_t ignoreAfter = time(0) + 60 * 60 * 11;
	Event* e = EventStore_First();
	bool didDrawEventDot = false;
	while (e) {
		
		// Get event
		Event* event = e;
		e = EventStore_Next(e);
		
		// Ignore if hidden
		if (event->hidden)
			continue;
		
		// Ignore if not within range
		if (event->time < ignoreBefore || event->time > ignoreAfter)
			continue;
		
		// Find angle for this item's time
		struct tm* time = localtime(&event->time);
		int angle = (TRIG_MAX_ANGLE * time->tm_hour / 12) + (TRIG_MAX_ANGLE * time->tm_min / 60) / 12;
		
		// Check if using a round screen
		#ifdef PBL_ROUND
			
			// Find center point of item's circle
			GPoint itemCenter = calculatePointOnCircle(GPoint(bounds.size.w / 2, bounds.size.h / 2), angle, MIN(bounds.size.w, bounds.size.h) / 2 - pointOffset);
			
		#else
		
			// Find center point of item's circle
			GPoint itemCenter = calculateBoxAngleIntersection(GRect(bounds.origin.x + pointOffset, bounds.origin.y + pointOffset, bounds.size.w - pointOffset*2, bounds.size.h - pointOffset*2), angle, 0);

		#endif
		
		// Draw icon
		graphics_context_set_fill_color(ctx, EventStore_Color(event->color));
		graphics_fill_rect(ctx, GRect(itemCenter.x - 3, itemCenter.y - 3, 6, 6), 2, GCornersAll);
		didDrawEventDot = true;
		
	}
	
	// Draw hour hand line if there are event dots drawn
	if (didDrawEventDot) {
		
		// Set line color
		#if defined(PBL_COLOR)
			graphics_context_set_stroke_color(ctx, GColorRed);
		#else
			graphics_context_set_stroke_color(ctx, GColorWhite);
		#endif
	
		// Find angle of minute hand line
		int angle = (TRIG_MAX_ANGLE * now.tm_hour / 12) + (TRIG_MAX_ANGLE * now.tm_min / 60) / 12;
	
		// Get line length
		int lineLength = 15;
		
		// Check if using a round screen
		#ifdef PBL_ROUND
		
			// Find position of minute hand line
			GPoint outerPoint = calculatePointOnCircle(GPoint(bounds.size.w / 2, bounds.size.h / 2), angle, MIN(bounds.size.w, bounds.size.h) / 2);
			GPoint innerPoint = calculatePointOnCircle(GPoint(bounds.size.w / 2, bounds.size.h / 2), angle, MIN(bounds.size.w, bounds.size.h) / 2 - lineLength);
	
		#else
			
			// Find position of minute hand line
			GPoint outerPoint = calculateBoxAngleIntersection(bounds, angle, 0);
			GPoint innerPoint = calculateBoxAngleIntersection(GRect(bounds.origin.x + lineLength, bounds.origin.y + lineLength, bounds.size.w - lineLength*2, bounds.size.h - lineLength*2), angle, 0);
			
		#endif
		
		// Draw line
		graphics_context_set_antialiased(ctx, true);
		graphics_context_set_stroke_width(ctx, 2);
		graphics_draw_line(ctx, outerPoint, innerPoint);
		
	}
	
	// Get next event
	Event* currentEvent = EventStore_Current();
	if (currentEvent) {
		
		// Draw event
		EventStore_DrawEvent(currentEvent, ctx, GRect(15, 12, bounds.size.w - 30, bounds.size.h / 2 - 24));
	
	} else {
		
		// No event, get placeholder size
		const char* bfr = "InfoWatch";
		GSize textSize = graphics_text_layout_get_content_size(bfr, Resources.fonts.logo, bounds, GTextOverflowModeTrailingEllipsis, GTextAlignmentCenter);
		
		// Draw placeholder
		graphics_context_set_text_color(ctx, GColorDarkGray);
		graphics_draw_text(ctx, bfr, Resources.fonts.logo, GRect(0, bounds.size.h / 4 - textSize.h / 2, bounds.size.w, textSize.h), GTextOverflowModeTrailingEllipsis, GTextAlignmentCenter, 0);
		
	}
	
	// Get size of time text
	GSize textSize = graphics_text_layout_get_content_size(timeLblBuffer, Resources.fonts.time, bounds, GTextOverflowModeTrailingEllipsis, GTextAlignmentLeft);
	
	// Draw time text
	graphics_context_set_text_color(ctx, GColorWhite);
	graphics_draw_text(ctx, timeLblBuffer, Resources.fonts.time, GRect(bounds.size.w / 2 - textSize.w / 2, bounds.size.h / 2 - textSize.h / 2, textSize.w, textSize.h), GTextOverflowModeTrailingEllipsis, GTextAlignmentLeft, 0);
	
	// Get day of week
	char* dayName = "-";
	if (now.tm_wday == 0)
		dayName = "Sunday";
	if (now.tm_wday == 1)
		dayName = "Monday";
	if (now.tm_wday == 2)
		dayName = "Tuesday";
	if (now.tm_wday == 3)
		dayName = "Wednesday";
	if (now.tm_wday == 4)
		dayName = "Thursday";
	if (now.tm_wday == 5)
		dayName = "Friday";
	if (now.tm_wday == 6)
		dayName = "Saturday";
	
	// Get month name
	char* monthName = "-";
	if (now.tm_mon == 0)
		monthName = "January";
	if (now.tm_mon == 1)
		monthName = "February";
	if (now.tm_mon == 2)
		monthName = "March";
	if (now.tm_mon == 3)
		monthName = "April";
	if (now.tm_mon == 4)
		monthName = "May";
	if (now.tm_mon == 5)
		monthName = "June";
	if (now.tm_mon == 6)
		monthName = "July";
	if (now.tm_mon == 7)
		monthName = "August";
	if (now.tm_mon == 8)
		monthName = "September";
	if (now.tm_mon == 9)
		monthName = "October";
	if (now.tm_mon == 10)
		monthName = "November";
	if (now.tm_mon == 11)
		monthName = "December";
	
	// Draw month name and year
	char bfr2[32];
	snprintf(bfr2, 32, "%s %i", monthName, 1900 + now.tm_year);
	
	// Get day info width
	GSize upperSize = graphics_text_layout_get_content_size(dayName, Resources.fonts.eventSubtitle, GRect(0, 0, 100, 100), GTextOverflowModeTrailingEllipsis, GTextAlignmentLeft);
	GSize lowerSize = graphics_text_layout_get_content_size(bfr2, Resources.fonts.eventSubtitle, GRect(0, 0, 100, 100), GTextOverflowModeTrailingEllipsis, GTextAlignmentLeft);
	int dayInfoWidth = 25 + MAX(upperSize.w, lowerSize.w);
	
	// Draw day of month
	char bfr[32];
	snprintf(bfr, 32, "%02i", now.tm_mday);
	graphics_context_set_text_color(ctx, COLOR_FALLBACK(GColorLightGray, GColorWhite));
	graphics_draw_text(ctx, bfr, Resources.fonts.dayOfMonth, GRect(bounds.size.w / 2 - dayInfoWidth / 2, bounds.size.h / 2 + bounds.size.h / 4 - 40 / 2, 100, 40), GTextOverflowModeTrailingEllipsis, GTextAlignmentLeft, 0);
	
	// Draw day of week
	graphics_context_set_text_color(ctx, COLOR_FALLBACK(GColorDarkGray, GColorWhite));
	graphics_draw_text(ctx, dayName, Resources.fonts.eventSubtitle, GRect(bounds.size.w / 2 - dayInfoWidth / 2 + 25, bounds.size.h / 2 + bounds.size.h / 4 - 40 / 2 + 3, 100, 40), GTextOverflowModeTrailingEllipsis, GTextAlignmentLeft, 0);
	
	// Draw month name and year
	graphics_context_set_text_color(ctx, COLOR_FALLBACK(GColorDarkGray, GColorWhite));
	graphics_draw_text(ctx, bfr2, Resources.fonts.eventSubtitle, GRect(bounds.size.w / 2 - dayInfoWidth / 2 + 25, bounds.size.h / 2 + bounds.size.h / 4 - 40 / 2 + 16, 100, 40), GTextOverflowModeTrailingEllipsis, GTextAlignmentLeft, 0);
	
	
}