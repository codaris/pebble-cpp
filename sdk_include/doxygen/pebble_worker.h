#pragma once

#include <locale.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#include "pebble_warn_unsupported_functions.h"
#include "pebble_worker_sdk_version.h"

#ifndef __FILE_NAME__
#define __FILE_NAME__ __FILE__
#endif

//! @addtogroup Worker
//! @{

//! The event loop for workers, to be used in worker's main(). Will block until the worker is ready to exit.
//! @see \ref App
void worker_event_loop(void);

//! Launch the foreground app for this worker
void worker_launch_app(void);

//! @} // group Worker

