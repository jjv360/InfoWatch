#pragma once


#define SETTING_HOUR_HAND_MODE		40
#define SETTING_SHOW_EVENT_DOTS		41

int 	Settings_Get(unsigned int setting, int defaultValue);
void	Settings_Set(unsigned int setting, int value);