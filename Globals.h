#pragma once

#include "ChestManager.h"
#include "Inventory.h"
#include "MessageWindow.h"
#include "DeltaTime.h"

// These DO NOT create the objects.
// They merely announce that they exist.
extern ChestManager chestManager;
extern Inventory inventory;
extern MessageWindow messageWindow;
extern DeltaTime dTime;

extern uint8_t currentInterior;