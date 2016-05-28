#include <pebble.h>
#include "Resources.h"

struct _Resources Resources;

void Resources_Init() {
	
	// Load fonts
	Resources.fonts.time = fonts_get_system_font(FONT_KEY_LECO_26_BOLD_NUMBERS_AM_PM);
	Resources.fonts.eventTitle = fonts_get_system_font(FONT_KEY_GOTHIC_18);
	Resources.fonts.eventSubtitle = fonts_get_system_font(FONT_KEY_GOTHIC_14);
	Resources.fonts.logo = fonts_get_system_font(FONT_KEY_GOTHIC_14);
	Resources.fonts.dayOfMonth = fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD);
	
	// Load images
	Resources.icons.notification = gdraw_command_image_create_with_resource(RESOURCE_ID_IMAGE_GENERIC_NOTIFICATION);
	Resources.icons.warning = gdraw_command_image_create_with_resource(RESOURCE_ID_IMAGE_GENERIC_WARNING);
	Resources.icons.failed = gdraw_command_image_create_with_resource(RESOURCE_ID_IMAGE_GENERIC_FAILED);
	Resources.icons.confirmation = gdraw_command_image_create_with_resource(RESOURCE_ID_IMAGE_GENERIC_CONFIRMATION);
	Resources.icons.pin = gdraw_command_image_create_with_resource(RESOURCE_ID_IMAGE_GENERIC_PIN);
	Resources.icons.battery = gdraw_command_image_create_with_resource(RESOURCE_ID_IMAGE_BATTERY);
	Resources.icons.batteryCharging = gdraw_command_image_create_with_resource(RESOURCE_ID_IMAGE_BATTERY_CHARGING);
	Resources.icons.sent = gdraw_command_image_create_with_resource(RESOURCE_ID_IMAGE_SENT);
	Resources.icons.settings = gdraw_command_image_create_with_resource(RESOURCE_ID_IMAGE_SETTINGS);
	Resources.icons.event = gdraw_command_image_create_with_resource(RESOURCE_ID_IMAGE_EVENT);
	Resources.icons.rain = gdraw_command_image_create_with_resource(RESOURCE_ID_IMAGE_RAIN);
	Resources.icons.snow = gdraw_command_image_create_with_resource(RESOURCE_ID_IMAGE_SNOW);
	Resources.icons.cloudy = gdraw_command_image_create_with_resource(RESOURCE_ID_IMAGE_CLOUDY);
	Resources.icons.wind = gdraw_command_image_create_with_resource(RESOURCE_ID_IMAGE_WIND);
	
}