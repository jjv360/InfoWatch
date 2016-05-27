#pragma once
#include <pebble.h>

void Watchface_Init();
void Watchface_Show();
void Watchface_Refresh();
void Watchface_DeInit();

void Watchface_OnTimeChanged(struct tm *tick_time, TimeUnits units_changed);
void Watchface_DrawBackground(struct Layer *layer, GContext *ctx);