#pragma once

void AppMessage_Init();
void AppMessage_OnIncomingMessage(DictionaryIterator* iterator, void *context);
void AppMessage_OnUpdatePing(void* data);