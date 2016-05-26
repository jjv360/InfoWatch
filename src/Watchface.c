#include <pebble.h>
#include "Watchface.h"
#include "MathUtil.h"
#include "EventStore.h"

static Window* window = 0;
static char timeLblBuffer[128] = {0};

static GColor backgroundColor = {0};
static GFont timeFont = 0;
static GFont eventFont = 0;
static struct tm now = {0};

static GDrawCommandImage* warningIcon = 0;
static GDrawCommandImage* failedIcon = 0;

// Set up the window
void Watchface_Init() {
	
	// Set defaults
	backgroundColor = GColorBlack;
	timeFont = fonts_get_system_font(FONT_KEY_LECO_26_BOLD_NUMBERS_AM_PM);
	eventFont = fonts_get_system_font(FONT_KEY_GOTHIC_14);
	
	// Load images
	warningIcon = gdraw_command_image_create_with_resource(RESOURCE_ID_IMAGE_GENERIC_WARNING);
	failedIcon = gdraw_command_image_create_with_resource(RESOURCE_ID_IMAGE_GENERIC_FAILED);
	
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
	time_t ignoreAfter = time(0) + 60 * 60 * 11;
	Event* e = EventStore_First();
	while (e) {
		
		// Get event
		Event* event = e;
		e = EventStore_Next(e);
		
		// Ignore if hidden
		if (event->hidden)
			continue;
		
		// Ignore if not within range
		if (event->time > ignoreAfter)
			continue;
		
		// Find angle for this item's time
		struct tm* time = localtime(&event->time);
		int angle = (TRIG_MAX_ANGLE * time->tm_hour / 12) + (TRIG_MAX_ANGLE * time->tm_min / 60) / 12;
		
		// Find center point of item's circle
		GPoint itemCenter = calculateBoxAngleIntersection(GRect(bounds.origin.x + pointOffset, bounds.origin.y + pointOffset, bounds.size.w - pointOffset*2, bounds.size.h - pointOffset*2), angle, 0);
		
		// Draw icon
		graphics_context_set_fill_color(ctx, EventStore_Color(event->color));
		graphics_fill_rect(ctx, GRect(itemCenter.x - 3, itemCenter.y - 3, 6, 6), 2, GCornersAll);
		
	}
	
	// Find angle of minute hand line
	int angle = (TRIG_MAX_ANGLE * now.tm_hour / 12) + (TRIG_MAX_ANGLE * now.tm_min / 60) / 12;
	
	// Find position of minute hand line
	int lineLength = 10;
	GPoint outerPoint = calculateBoxAngleIntersection(bounds, angle, 0);
	GPoint innerPoint = calculateBoxAngleIntersection(GRect(bounds.origin.x + lineLength, bounds.origin.y + lineLength, bounds.size.w - lineLength*2, bounds.size.h - lineLength*2), angle, 0);
	
	// Draw line
	graphics_context_set_antialiased(ctx, true);
	graphics_context_set_stroke_color(ctx, GColorRed);
	graphics_context_set_stroke_width(ctx, 2);
	graphics_draw_line(ctx, outerPoint, innerPoint);
	
	// Get next event
	Event* currentEvent = EventStore_Current();
	if (currentEvent) {
		
		// Check icon type
		int iconWidth = 8;
		if (currentEvent -> type == EVENT_TYPE_GENERIC) {
			
			// Draw colored dot
			graphics_context_set_fill_color(ctx, EventStore_Color(currentEvent->color));
			graphics_fill_rect(ctx, GRect(20, bounds.size.h / 2 / 2 - 20, 6, 6), 2, GCornersAll);
			
		} else if (currentEvent -> type == EVENT_TYPE_WARNING) {
			
			// Draw icon
			iconWidth = 28 - 11;
			gdraw_command_image_draw(ctx, warningIcon, GPoint(9, bounds.size.h / 2 / 2 - 20));
			
		} else if (currentEvent -> type == EVENT_TYPE_FAILED) {
			
			// Draw icon
			iconWidth = 28 - 11;
			gdraw_command_image_draw(ctx, failedIcon, GPoint(9, bounds.size.h / 2 / 2 - 20));
			
		}
		
		// Draw title
		graphics_context_set_text_color(ctx, GColorWhite);
		graphics_draw_text(ctx, currentEvent->name, eventFont, GRect(20 + iconWidth, bounds.size.h / 2 / 2 - 20 - 6, bounds.size.w - 20 - iconWidth - 20, 20), GTextOverflowModeTrailingEllipsis, GTextAlignmentLeft, 0);
		
		// Draw time delay
		graphics_context_set_text_color(ctx, GColorLightGray);
		char bfr[64];
		EventStore_CopySubtitle(currentEvent, bfr, 64);
		graphics_draw_text(ctx, bfr, eventFont, GRect(20 + iconWidth, bounds.size.h / 2 / 2 - 9, bounds.size.w - 22 - 20 - iconWidth, 40), GTextOverflowModeTrailingEllipsis, GTextAlignmentLeft, 0);
		
	}
	
	// Get size of time text
	GSize textSize = graphics_text_layout_get_content_size(timeLblBuffer, timeFont, bounds, GTextOverflowModeTrailingEllipsis, GTextAlignmentLeft);
	
	// Draw time text
	graphics_context_set_text_color(ctx, GColorWhite);
	graphics_draw_text(ctx, timeLblBuffer, timeFont, GRect(bounds.size.w / 2 - textSize.w / 2, bounds.size.h / 2 - textSize.h / 2, textSize.w, textSize.h), GTextOverflowModeTrailingEllipsis, GTextAlignmentLeft, 0);
	
}