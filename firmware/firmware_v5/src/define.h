#pragma once

#include <Arduino.h>

// Load the configuration details
// This is excluded from git VSC since it may contains credentials and secrets
#include "config/config.h"

// pin map
#include "config/pins.h"

// Global variables
#include "config/global_variables.h"

// *** Utilities ************************************************

#include "utilities/i2c/i2c.h"
#include "utilities/memory/memory.h"

// *** Modules **************************************************

// Motors
#include "modules/motors/motors.h"

// Motion Sensor
#include "sensors/motion/motion.h"